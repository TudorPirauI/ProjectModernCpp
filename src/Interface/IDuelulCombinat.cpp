#include "Interface/IDuelulCombinat.h"
#include "Interface/AlertWidget.h"
#include "Interface/ElementDialog.h"
#include "Interface/ExplosionDialog.h"
#include "Interface/VrajitorDialog.h"

IDuelulCombinat::IDuelulCombinat(const std::string &nameOne, const std::string &nameTwo,
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
            &IDuelulCombinat::OnPositionSelected);
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
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IDuelulCombinat::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    m_SpecialOptions = new SpecialOptions(this);

    m_SpecialOptions->SetPowers(options[0], options[1], options[2]);

    connect(m_SpecialOptions, &SpecialOptions::OptionSelected, this,
            &IDuelulCombinat::OnModifierSelected);

    mainLayout->addWidget(m_SpecialOptions);

    m_ElementWidget = new ElementWidget(m_CurrentGame.GetElementalAbilityPlayer1(), this);
    m_ElementWidget->setFixedSize(200, 50);
    connect(m_ElementWidget, &ElementWidget::clicked, this, [this] {
        const auto power  = m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                    ? m_CurrentGame.GetElementalAbilityPlayer1()
                                    : m_CurrentGame.GetElementalAbilityPlayer2();
        const auto dialog = new ElementDialog(power);
        connect(dialog, &ElementDialog::DialogAccepted, this, &IDuelulCombinat::OnDialogAccepted);
        dialog->exec();
    });

    auto *elementLayout = new QHBoxLayout();
    elementLayout->addStretch();
    elementLayout->addWidget(m_ElementWidget);
    elementLayout->addStretch();

    mainLayout->addLayout(elementLayout);

    m_WizardWidget = new VrajitorWidget(m_CurrentGame.GetWizardAbilityPlayer1(), this);
    m_WizardWidget->setFixedSize(200, 50);
    connect(m_WizardWidget, &VrajitorWidget::clicked, this, [this] {
        const auto power  = m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                    ? m_CurrentGame.GetWizardAbilityPlayer1()
                                    : m_CurrentGame.GetWizardAbilityPlayer2();
        const auto dialog = new VrajitorDialog(power);
        connect(dialog, &VrajitorDialog::DialogAccepted, this, &IDuelulCombinat::OnDialogAccepted);
        dialog->exec();
    });

    auto *wizardLayout = new QHBoxLayout();
    wizardLayout->addStretch();
    wizardLayout->addWidget(m_WizardWidget);
    wizardLayout->addStretch();

    mainLayout->addLayout(wizardLayout);

    parent->setLayout(mainLayout);
}

void IDuelulCombinat::OnCardSelected(const int cardIndex) {
    const auto currentPlayer = m_CurrentGame.GetCurrentPlayer();

    if (cardIndex >= currentPlayer.GetHand().size() || cardIndex < 0) {
        std::cerr << "Invalid card index: " << cardIndex << '\n';
        return;
    }

    m_SelectedCard = currentPlayer.GetHand()[cardIndex];
}

void IDuelulCombinat::OnPositionSelected(const int x, const int y) {
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

void IDuelulCombinat::OnModifierSelected(const int modifier) {
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

void IDuelulCombinat::OnExplosion() { m_CurrentGame.ApplyExplosion(m_CurrentExplosion); }

void IDuelulCombinat::OnDialogAccepted(const std::vector<QString> &info, bool isWizardPower) {
    // Handle the received information
    Position positionOne{-200, -200};
    Position positionTwo{-200, -200};
    int      number = -200;
    Card     card{10, m_CurrentGame.GetCurrentPlayerTurn()};
    for (auto &data : info) {
        QString copy = data;

        // todo: swap this for regex in the future
        if (data.contains("Position: ") && positionOne.first == -200) {
            copy.replace("Position: ", "");
            const auto positions = copy.split(" ");

            std::cout << "Position 1: ";
            for (const auto &pos : positions) {
                std::cout << pos.toStdString() << ' ';
            }
            std::cout << '\n';

            positionOne.first  = positions[0].toInt();
            positionOne.second = positions[1].toInt();
        } else if (data.contains("Position: ")) {
            copy.replace("Position: ", "");
            const auto positions = copy.split(" ");

            std::cout << "Position 2: ";
            for (const auto &pos : positions) {
                std::cout << pos.toStdString() << ' ';
            }
            std::cout << '\n';

            positionTwo.first  = positions[0].toInt();
            positionTwo.second = positions[1].toInt();
        } else if (data.contains("Card")) {
            card.SetValue(data.split(" ")[1].toInt());
        } else if (data.contains("Number")) {
            number = data.split(" ")[1].toInt();
        }
    }

    if (isWizardPower) {
        const auto result = m_CurrentGame.VerifyWizardPower(
                m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                        ? m_CurrentGame.GetWizardAbilityPlayer1().GetType()
                        : m_CurrentGame.GetWizardAbilityPlayer2().GetType(),
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
                                             ? m_CurrentGame.GetWizardAbilityPlayer1()
                                             : m_CurrentGame.GetWizardAbilityPlayer2());

            std::cout << "Power activated\n";
        }
    } else {
        switch (m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                        ? m_CurrentGame.GetElementalAbilityPlayer1().GetPower()
                        : m_CurrentGame.GetElementalAbilityPlayer2().GetPower()) {
            case ElementIndexPower::FromAshes: {
                // sets your hand to one of your removed cards
                m_HandWidget->SetCards(m_CurrentGame.GetCurrentPlayer().GetRemovedCards());
                break;
            }
            // case ElementIndexPower::Sparks: { // you'll just select the position from where it is
            // play one of your own covered cards on another position
            // m_HandWidget->SetCards(m_CurrentGame.GetCurrentPlayer());
            // break;
            // }
            case ElementIndexPower::Fog: {
                // reset the illusion boolean
                m_CurrentGame.GetCurrentPlayer().SetHasIllusionInGame(false);
                break;
            }
            case ElementIndexPower::Granite: {
                auto graniteCard = Card{10, PlayerTurn::Granite};
                graniteCard.SetIsGranite(true);
                m_HandWidget->SetCards({graniteCard});
                break;
            }
            default:
                break;
        }

        const auto result = m_CurrentGame.VerifyElementalPower(
                m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                        ? m_CurrentGame.GetElementalAbilityPlayer1().GetPower()
                        : m_CurrentGame.GetElementalAbilityPlayer2().GetPower(),
                positionOne, positionTwo, card, m_CurrentGame.GetCurrentPlayerTurn(), number);

        if (result) {
            m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
            m_HandWidget->SetCards(m_CurrentGame.GetCurrentPlayer().GetHand());
            if (m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1) {
                m_PlayerOneUsedPower = true;
            } else {
                m_PlayerTwoUsedPower = true;
            }

            m_ElementWidget->SetPower(m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                              ? m_CurrentGame.GetElementalAbilityPlayer1()
                                              : m_CurrentGame.GetElementalAbilityPlayer2(),
                                      m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                              ? !m_PlayerOneUsedPower
                                              : !m_PlayerTwoUsedPower);
        }
    }
}

void IDuelulCombinat::SwitchTurn() {
    m_IsIllusionSelected = false;

    m_SelectedCard.reset();

    if (m_CurrentGame.ExplosionEnabled() && m_CurrentGame.CheckExplosion()) {
        m_CurrentExplosion         = m_CurrentGame.GenerateExplosion();
        const auto explosionDialog = new ExplosionDialog(m_CurrentExplosion, this);

        connect(explosionDialog, &ExplosionDialog::ExplosionTriggered, this,
                &IDuelulCombinat::OnExplosion);

        explosionDialog->exec();
    }

    const auto winningReason = m_CurrentGame.CheckWinningConditions();

    if (winningReason == Game::WinningCondition::NoWin) {
        m_CurrentGame.SwapTurn();

        const auto &nextPlayer = m_CurrentGame.GetCurrentPlayer();

        m_ElementWidget->SetPower(m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                          ? m_CurrentGame.GetElementalAbilityPlayer1()
                                          : m_CurrentGame.GetElementalAbilityPlayer2(),
                                  m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                          ? !m_PlayerOneUsedPower
                                          : !m_PlayerTwoUsedPower);

        m_WizardWidget->SetPower(m_CurrentGame.GetCurrentPlayerTurn() == PlayerTurn::Player1
                                         ? m_CurrentGame.GetWizardAbilityPlayer1()
                                         : m_CurrentGame.GetWizardAbilityPlayer2());

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

        QTimer::singleShot(1500, this, &IDuelulCombinat::GameFinished);
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
