//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"

#include "Interface/AlertWidget.h"

IAntrenament::IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                           QWidget *parent) :
    QWidget(parent), m_CurrentGame(nameOne, nameTwo), m_CurrentPlayer(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    const auto mainLayout = new QVBoxLayout(this);

    m_BoardWidget = new BoardWidget(this, 3);
    m_BoardWidget->setFixedSize(800, 800);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this, &IAntrenament::OnPositionSelected);
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    auto *boardLayout = new QHBoxLayout();
    boardLayout->addStretch();
    boardLayout->addWidget(m_BoardWidget);
    boardLayout->addStretch();

    mainLayout->addLayout(boardLayout);

    m_HandWidget = new HandWidget(this);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_HandWidget->setFixedSize(m_HandWidget->GetIdealWidth(), 200);
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    parent->setLayout(mainLayout);
}

void IAntrenament::OnCardSelected(const int cardValue) {
    m_SelectedCard = Card(cardValue);
    m_SelectedCard->SetPlacedBy(m_CurrentPlayer);
}

void IAntrenament::OnPositionSelected(const int x, const int y) {
    if (!m_SelectedCard.has_value()) {
        std::cerr << "Please select a card first!\n";
        return;
    }

    if (!m_CurrentGame.GetBoard().IsPositionValid({x, y}, m_SelectedCard.value())) {
        std::cerr << "You cannot place a card there!";
        return;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(
            m_SelectedCard.value(), {x, y}, m_CurrentPlayer, CardType::Normal, m_CurrentGame);

    if (success != InsertOutputs::Success) {
        std::cerr << "Could not place card on board\n";
        return;
    }

    auto &currentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                   : m_CurrentGame.GetPlayer2();

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IAntrenament::SwitchTurn() {
    // TODO: Pretty up the hand and the board
    // The board should have consistent spacings between the slots
    // The cards should be held like a bridge hand, slightly circular
    // When you hover over the cards in your hands they should increase in size a bit and move
    // according to your cursor

    // TODO: Add Eter, Illusion and explosion (also pop up for this) options

    m_SelectedCard.reset();

    const auto winningReason = m_CurrentGame.CheckWinningConditions();

    if (winningReason == Game::WinningCondition::NoWin) {

        m_CurrentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2
                                                                   : PlayerTurn::Player1;

        const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1)
                                         ? m_CurrentGame.GetPlayer1()
                                         : m_CurrentGame.GetPlayer2();

        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
        m_HandWidget->SetCards(nextPlayer.GetHand());

        m_HandWidget->update();
        m_BoardWidget->update();
        return;
    }

    const auto &winner = m_CurrentPlayer == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                                : m_CurrentGame.GetPlayer2();

    m_CurrentGame.IncreasePlayerScore(m_CurrentPlayer);

    const auto currentScore = m_CurrentPlayer == PlayerTurn::Player1
                                      ? m_CurrentGame.GetPlayer1Score()
                                      : m_CurrentGame.GetPlayer2Score();

    std::cout << winner.GetUserName() << " has won the round!\n" << "Reason: ";
    switch (winningReason) {
        case Game::WinningCondition::LineWin:
            std::cout << "Line win";
            break;
        case Game::WinningCondition::ColumnWin:
            std::cout << "Column win";
            break;
        case Game::WinningCondition::DiagonalPrincipalWin:
            std::cout << "Diagonal principal win";
            break;
        case Game::WinningCondition::DiagonalSecondaryWin:
            std::cout << "Diagonal secondary win";
            break;
        default:
            std::cout << "Bruh";
    }

    std::cout << '\n';

    const auto alertWidget = new AlertWidget(m_ParentWidget);

    if (currentScore >= m_CurrentGame.GetScoreToWin()) {
        alertWidget->ShowAlert(QString::fromStdString(winner.GetUserName() + " has won the game!"));

        if (m_ParentWidget) {
            m_ParentWidget->close();
        }
    } else {
        const auto playerOneStats = m_CurrentGame.GetPlayer1().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer1Score());
        const auto playerTwoStats = m_CurrentGame.GetPlayer2().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer2Score());
        alertWidget->ShowAlert(
                QString::fromStdString(winner.GetUserName() + " has won the round!\n\n" +
                                       "Current score\n" + playerOneStats + "\n" + playerTwoStats));

        m_CurrentGame.SetNewCards();
        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

        const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1)
                                         ? m_CurrentGame.GetPlayer2()
                                         : m_CurrentGame.GetPlayer1();
        m_HandWidget->SetCards(nextPlayer.GetHand());

        m_HandWidget->update();
        m_BoardWidget->update();
    }
}
