//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"
#include "Interface/AlertWidget.h"
#include "Interface/SpecialOptions.h"

IAntrenament::IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                           const std::array<bool, 3> &options, QWidget *parent) :
    QWidget(parent), m_CurrentGame(nameOne, nameTwo, options), m_CurrentTurn(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    const auto mainLayout = new QVBoxLayout(this);

    const auto backButton = new QPushButton("<", this);
    backButton->setFixedSize(30, 30);
    connect(backButton, &QPushButton::clicked, this, [this] { emit GameFinished(); });
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);

    m_BoardWidget = new BoardWidget(this, 3);
    m_BoardWidget->setFixedSize(800, 800);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this, &IAntrenament::OnPositionSelected);
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    auto *boardLayout = new QHBoxLayout();
    boardLayout->addStretch();
    boardLayout->addWidget(m_BoardWidget);
    boardLayout->addStretch();

    mainLayout->addLayout(boardLayout);

    if (options[0]) {
        m_CurrentGame.GetPlayer1().GiveEterCard(PlayerTurn::Player1);
        m_CurrentGame.GetPlayer2().GiveEterCard(PlayerTurn::Player2);
    }

    m_HandWidget = new HandWidget(this);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_HandWidget->setFixedSize(m_HandWidget->GetIdealWidth(), 200);
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    m_SpecialOptions = new SpecialOptions(this);

    m_SpecialOptions->SetPowers(options[0], options[1], options[2]);

    mainLayout->addWidget(m_SpecialOptions);

    parent->setLayout(mainLayout);
}

void IAntrenament::OnCardSelected(const int cardIndex) {
    const auto currentPlayer = m_CurrentTurn == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                                    : m_CurrentGame.GetPlayer2();

    if (cardIndex >= currentPlayer.GetHand().size() || cardIndex < 0) {
        std::cerr << "Invalid card index: " << cardIndex << '\n';
        return;
    }

    m_SelectedCard = currentPlayer.GetHand()[cardIndex];
}

void IAntrenament::OnPositionSelected(const int x, const int y) {
    if (!m_SelectedCard.has_value()) {
        std::cerr << "Please select a card first!\n";
        return;
    }

    if (!m_CurrentGame.GetBoard().IsPositionValid({x, y}, m_SelectedCard.value())) {
        std::cerr << "You cannot place a card there!\n";
        return;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(
            m_SelectedCard.value(), {x, y}, m_CurrentTurn, CardType::Normal, m_CurrentGame);

    if (success != InsertOutputs::Success) {
        std::cerr << "Could not place card on board\n";
        return;
    }

    auto &currentPlayer = m_CurrentTurn == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                               : m_CurrentGame.GetPlayer2();

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IAntrenament::SwitchTurn() {
    // TODO: Pretty up the hand and the board

    // TODO: Add Eter, Illusion and explosion (also pop up for this) options

    m_SelectedCard.reset();

    const auto winningReason = m_CurrentGame.CheckWinningConditions();

    if (winningReason == Game::WinningCondition::NoWin) {
        m_CurrentTurn =
                m_CurrentTurn == PlayerTurn::Player1 ? PlayerTurn::Player2 : PlayerTurn::Player1;

        const auto &nextPlayer = m_CurrentTurn == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();

        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
        m_HandWidget->SetCards(nextPlayer.GetHand());

        m_HandWidget->update();
        m_BoardWidget->update();

        const auto shouldHaveEter = m_CurrentGame.GetEterEnabled();
        const auto shouldHaveIllusion =
                m_CurrentGame.GetIllusionEnabled() && !nextPlayer.GetHasIllusionInGame();
        const auto shouldHaveExplosion =
                m_CurrentGame.ExplosionEnabled() && nextPlayer.GetHasExplosion();

        m_SpecialOptions->SetPowers(shouldHaveEter, shouldHaveIllusion, shouldHaveExplosion);

        return;
    }

    const auto &winner = m_CurrentTurn == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                              : m_CurrentGame.GetPlayer2();

    m_CurrentGame.IncreasePlayerScore(m_CurrentTurn);

    const auto currentScore = m_CurrentTurn == PlayerTurn::Player1
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

        QTimer::singleShot(1500, this, &IAntrenament::GameFinished);
    } else {
        const auto playerOneStats = m_CurrentGame.GetPlayer1().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer1Score());
        const auto playerTwoStats = m_CurrentGame.GetPlayer2().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer2Score());
        alertWidget->ShowAlert(
                QString::fromStdString(winner.GetUserName() + " has won the round!\n\n" +
                                       "Current score\n" + playerOneStats + "\n" + playerTwoStats));

        m_CurrentTurn = PlayerTurn::Player1;
        m_CurrentGame.SetNewCards();
        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
        m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());

        m_HandWidget->update();
        m_BoardWidget->update();
    }
}
