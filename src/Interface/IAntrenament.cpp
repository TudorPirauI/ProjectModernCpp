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

    // TODO: Simple recommendation system
    // Takes in -> Current board, current players, current hand of each players
    // first check: can we win in the next move? if yes, do it (override another card and make a
    // straight line, column or diagonal / just by placing a card)
    // second check: can the opponent win in the next move? if yes, block it
    // if we can't do either, prioritize placing cards that would make another card win in the next
    // if we can't do the above, try prioritizing having as many face cards up as possible (placing
    // cards over the opponent's)

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    m_SpecialOptions = new SpecialOptions(this);

    m_SpecialOptions->SetPowers(options[0], options[1], options[2]);

    connect(m_SpecialOptions, &SpecialOptions::OptionSelected, this,
            &IAntrenament::OnModifierSelected);

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
        const auto alertWidget = new AlertWidget(m_ParentWidget);

        alertWidget->ShowAlert("You must select a card first!");

        return;
    }

    if (!m_CurrentGame.GetBoard().IsPositionValid({x, y}, m_SelectedCard.value())) {
        const auto alertWidget = new AlertWidget(m_ParentWidget);

        alertWidget->ShowAlert("You can't place a card there!");

        return;
    }

    auto cardType = CardType::Normal;

    if (m_IsIllusionSelected) {
        cardType = CardType::Illusion;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(
            m_SelectedCard.value(), {x, y}, m_CurrentTurn, cardType, m_CurrentGame);

    auto &currentPlayer = m_CurrentTurn == PlayerTurn::Player1 ? m_CurrentGame.GetPlayer1()
                                                               : m_CurrentGame.GetPlayer2();

    if (success != InsertOutputs::Success) {
        std::cerr << "Could not place card on board\n";

        const auto alertWidget = new AlertWidget(m_ParentWidget);

        switch (success) {
            case InsertOutputs::PositionInvalid:
                alertWidget->ShowAlert("That's an invalid position!");
                break;
            case InsertOutputs::IllusionOccupied:
                alertWidget->ShowAlert("Your card was not powerful enough to cover the Illusion!");
                currentPlayer.RemoveCard(m_SelectedCard.value());
                SwitchTurn();
                return;
            case InsertOutputs::EterOccupied:
                alertWidget->ShowAlert("That position is occupied by an Eter card!");
                break;
            case InsertOutputs::GraniteOccupied:
                alertWidget->ShowAlert("That position is blocked off!");
                break;
            default:
                alertWidget->ShowAlert("Bruh");
        }
        return;
    }

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IAntrenament::OnModifierSelected(int modifier) {
    // 1 -> Illusion
    // 2 -> Explosion (should be triggered instantly)
    switch (modifier) {
        case 1: {
            const auto currentPlayer = m_CurrentTurn == PlayerTurn::Player1
                                               ? m_CurrentGame.GetPlayer1()
                                               : m_CurrentGame.GetPlayer2();

            if (!currentPlayer.GetHasIllusionInGame()) {
                m_IsIllusionSelected = !m_IsIllusionSelected;
            }
            break;
        }
        case 2: {
            // just explode the board I guess. also check if it's a valid call}
            break;
        }
        default: {
            std::cerr << "Invalid modifier selected: " << modifier << '\n';
        }
    }
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
