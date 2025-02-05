#include "Interface/IDuelulVrajitorilor.h"
#include "Interface/AlertWidget.h"
#include "Interface/ExplosionDialog.h"
#include "Interface/VrajitorDialog.h"

IDuelulVrajitorilor::IDuelulVrajitorilor(const std::string &nameOne, const std::string &nameTwo,
                                         const std::array<bool, 3> &options, QWidget *parent) :
    QWidget(parent), m_CurrentGame(nameOne, nameTwo, options), m_SelectedCard(std::nullopt),
    m_ParentWidget(parent) {

    const auto mainLayout = new QVBoxLayout(this);

    const auto backButton = new QPushButton("<", this);
    backButton->setFixedSize(30, 30);
    connect(backButton, &QPushButton::clicked, this, [this] { emit GameFinished(); });
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);

    m_BoardWidget = new BoardWidget(this, 3);
    m_BoardWidget->setFixedSize(800, 800);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this,
            &IDuelulVrajitorilor::OnPositionSelected);
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

    m_CurrentGame.SetPlayerTurn(PlayerTurn::Player1);

    m_HandWidget = new HandWidget(this);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_HandWidget->setFixedSize(m_HandWidget->GetIdealWidth(), 200);
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IDuelulVrajitorilor::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    m_SpecialOptions = new SpecialOptions(this);

    m_SpecialOptions->SetPowers(options[0], options[1], options[2]);

    connect(m_SpecialOptions, &SpecialOptions::OptionSelected, this,
            &IDuelulVrajitorilor::OnModifierSelected);

    mainLayout->addWidget(m_SpecialOptions);

    m_WizardWidget = new VrajitorWidget(m_CurrentGame.GetPlayerAbility1(), this);
    m_WizardWidget->setFixedSize(200, 50);
    connect(m_WizardWidget, &VrajitorWidget::clicked, this, [this] {
        const auto power  = m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                    ? m_CurrentGame.GetPlayerAbility1()
                                    : m_CurrentGame.GetPlayerAbility2();
        const auto dialog = new VrajitorDialog(power);
        connect(dialog, &VrajitorDialog::DialogAccepted, this,
                &IDuelulVrajitorilor::OnDialogAccepted);
        dialog->exec();
    });

    auto *elementLayout = new QHBoxLayout();
    elementLayout->addStretch();
    elementLayout->addWidget(m_WizardWidget);
    elementLayout->addStretch();

    mainLayout->addLayout(elementLayout);

    // TODO: When a power insists on placing something specific (a single eter card, one of the past
    // cards etc.) Simply set the hand to those cards via HandWidget (do not modify the current
    // player's hand) :P

    parent->setLayout(mainLayout);
}

void IDuelulVrajitorilor::OnCardSelected(const int cardIndex) {
    const auto currentPlayer = m_CurrentGame.GetCurrentPlayer();

    if (cardIndex >= currentPlayer.GetHand().size() || cardIndex < 0) {
        std::cerr << "Invalid card index: " << cardIndex << '\n';
        return;
    }

    m_SelectedCard = currentPlayer.GetHand()[cardIndex];
}

void IDuelulVrajitorilor::OnPositionSelected(const int x, const int y) {
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

    const auto success = m_CurrentGame.GetBoard().InsertCard(m_SelectedCard.value(), {x, y},
                                                             m_CurrentGame.GetCurrentPlayerTurn(),
                                                             cardType, m_CurrentGame);

    auto &currentPlayer = m_CurrentGame.GetCurrentPlayer();

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

void IDuelulVrajitorilor::OnModifierSelected(const int modifier) {
    // 1 -> Illusion
    // 2 -> Explosion (should be triggered instantly)
    switch (modifier) {
        case 1: {
            if (!m_CurrentGame.GetCurrentPlayer().GetHasIllusionInGame()) {
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

void IDuelulVrajitorilor::OnExplosion() { m_CurrentGame.ApplyExplosion(m_CurrentExplosion); }

void IDuelulVrajitorilor::OnDialogAccepted(const std::vector<QString> &info) {
    // Handle the received information
    Position positionOne{-200, -200};
    Position positionTwo{-200, -200};
    int      number = -200;
    Card     card{10, m_CurrentGame.GetCurrentPlayerTurn()};

    for (auto &data : info) {
        std::regex positionRegex(R"(Position: (\d+) (\d+))");
        std::regex cardRegex(R"(Card (\d+))");
        std::regex numberRegex(R"(Number (\d+))");

        std::smatch match;
        std::string dataStr = data.toStdString();

        if (std::regex_search(dataStr, match, positionRegex)) {
            if (positionOne.first == -200) {
                std::cout << "Position 1: " << match[1] << ' ' << match[2] << '\n';
                positionOne.first  = std::stoi(match[1]);
                positionOne.second = std::stoi(match[2]);
            } else {
                std::cout << "Position 2: " << match[1] << ' ' << match[2] << '\n';
                positionTwo.first  = std::stoi(match[1]);
                positionTwo.second = std::stoi(match[2]);
            }
        } else if (std::regex_search(dataStr, match, cardRegex)) {
            card.SetValue(std::stoi(match[1]));
        } else if (std::regex_search(dataStr, match, numberRegex)) {
            number = std::stoi(match[1]);
        }
    }

    // todo: switch case with the power type

    const auto result = m_CurrentGame.VerifyWizardPower(
            m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                    ? m_CurrentGame.GetPlayerAbility1().GetType()
                    : m_CurrentGame.GetPlayerAbility2().GetType(),
            positionOne, positionTwo, card, m_CurrentGame.GetCurrentPlayerTurn());

    if (result) {
        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
        m_HandWidget->SetCards(m_CurrentGame.GetCurrentPlayer().GetHand());
        if (m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1) {
            m_PlayerOneUsedPower = true;
        } else {
            m_PlayerTwoUsedPower = true;
        }

        m_WizardWidget->SetPower(m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                         ? m_CurrentGame.GetPlayerAbility1()
                                         : m_CurrentGame.GetPlayerAbility2());

        std::cout << "Power activated\n";
    }
}

void IDuelulVrajitorilor::SwitchTurn() {
    m_IsIllusionSelected = false;

    m_SelectedCard.reset();

    if (m_CurrentGame.ExplosionEnabled() && m_CurrentGame.CheckExplosion()) {
        m_CurrentExplosion         = m_CurrentGame.GenerateExplosion();
        const auto explosionDialog = new ExplosionDialog(m_CurrentExplosion, this);

        connect(explosionDialog, &ExplosionDialog::ExplosionTriggered, this,
                &IDuelulVrajitorilor::OnExplosion);

        explosionDialog->exec();
    }

    const auto winningReason = m_CurrentGame.CheckWinningConditions();

    if (winningReason == Game::WinningCondition::NoWin) {
        m_CurrentGame.SwapTurn();

        const auto &nextPlayer = m_CurrentGame.GetCurrentPlayer();

        m_WizardWidget->SetPower(m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                         ? m_CurrentGame.GetPlayerAbility1()
                                         : m_CurrentGame.GetPlayerAbility2());

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

    const auto &winner = m_CurrentGame.GetCurrentPlayer();

    m_CurrentGame.IncreasePlayerScore(m_CurrentGame.GetCurrentPlayerTurn());

    const auto currentScore = m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                      ? m_CurrentGame.GetPlayer1Score()
                                      : m_CurrentGame.GetPlayer2Score();

    const auto alertWidget = new AlertWidget(m_ParentWidget);

    if (currentScore >= m_CurrentGame.GetScoreToWin()) {
        alertWidget->ShowAlert(QString::fromStdString(winner.GetUserName() + " has won the game!"));

        QTimer::singleShot(1500, this, &IDuelulVrajitorilor::GameFinished);
    } else {
        const auto playerOneStats = m_CurrentGame.GetPlayer1().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer1Score());
        const auto playerTwoStats = m_CurrentGame.GetPlayer2().GetUserName() + " - " +
                                    std::to_string(m_CurrentGame.GetPlayer2Score());
        alertWidget->ShowAlert(
                QString::fromStdString(winner.GetUserName() + " has won the round!\n\n" +
                                       "Current score\n" + playerOneStats + "\n" + playerTwoStats));

        m_CurrentGame.SetPlayerTurn(PlayerTurn::Player1);
        m_CurrentGame.SetNewCards();
        m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
        m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());

        m_HandWidget->update();
        m_BoardWidget->update();
    }
}
