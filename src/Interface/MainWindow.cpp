//
// Created by mavri on 05.12.2024.
//

#include "MainWindow.h"

#include "../Game/Antrenament.h"
#include "../GameBoard/Board.h"

// m_MediaPlayer = new QMediaPlayer(this);
// m_MediaPlayer->setSource(QUrl::fromLocalFile("../background.mp3"));
//
// const auto audioOutput = new QAudioOutput(this);
// m_MediaPlayer->setAudioOutput(audioOutput);
// audioOutput->setVolume(0.5);
//
// m_MediaPlayer->play();

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_CurrentState(GameState::MainMenu) {
    m_StackedWidget = new QStackedWidget(this);

    const auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    const auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(m_StackedWidget);

    setMinimumSize(1200, 600);

    if (m_FullScreen) {
        showFullScreen();
    } else {
        showNormal();
    }

    // Initialize the score label
    m_ScoreLabel = new QLabel(this);
    m_ScoreLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    QFont scoreFont = m_ScoreLabel->font();
    scoreFont.setPointSize(12);
    m_ScoreLabel->setFont(scoreFont);
    UpdateScoreLabel();
    m_ScoreLabel->show();

    DrawMenu();
}

void MainWindow::AddBackButton(const QWidget *widget) {
    const auto backButton = new QPushButton("<", this);
    backButton->setFixedSize(30, 30);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::OnBackClicked);

    const auto layout = qobject_cast<QVBoxLayout *>(widget->layout());
    if (layout) {
        layout->insertWidget(0, backButton, 0, Qt::AlignLeft);
    }
}

void MainWindow::DrawMenu() {
    const auto menuWidget = new QWidget(this);
    const auto layout     = new QVBoxLayout(menuWidget);

    const auto title     = new QLabel("ETER", this);
    QFont      titleFont = title->font();

    titleFont.setPointSize(48);
    titleFont.setBold(true);

    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);

    const auto createButton = [this, layout](const QString &text, void (MainWindow::*slot)()) {
        const auto button     = new QPushButton(text, this);
        QFont      buttonFont = button->font();

        buttonFont.setPointSize(20);
        button->setFont(buttonFont);

        connect(button, &QPushButton::clicked, this, [this, slot]() { (this->*slot)(); });
        layout->addWidget(button);
    };

    createButton("New Game", &MainWindow::OnNewGameClicked);
    createButton("Resume Game", &MainWindow::OnResumeGameClicked);
    createButton("Options", &MainWindow::OnOptionsClicked);
    createButton("Exit", &MainWindow::OnExitClicked);

    layout->setAlignment(Qt::AlignCenter);

    m_StackedWidget->addWidget(menuWidget);
    m_StackedWidget->setCurrentWidget(menuWidget);

    m_ScoreLabel->hide();
}

QPixmap LoadCardImage(const int cardValue) {
    // QString imagePath = QString(":/images/card%1.png").arg(cardValue);
    const QString imagePath = QString("../images/%1.png").arg(cardValue);
    const QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image for card" << cardValue;
    }
    return pixmap;
}

QGridLayout *MainWindow::GenerateBoard(const Board                         &board,
                                       const std::function<void(Position)> &cellClickedCallback) {
    const auto boardLayout = new QGridLayout();
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    auto left  = board.GetLeft();
    auto right = board.GetRight();
    auto up    = board.GetUp();
    auto down  = board.GetDown();

    if (!board.IsBoardLocked()) {
        ++right.first;
        ++down.second;
        --left.first;
        --up.second;
    }

    const auto boardElement = board.GetGameBoard();

    for (int j = up.second; j <= down.second; ++j) {
        for (int i = left.first; i <= right.first; ++i) {
            const auto it = boardElement.find({i, j});
            QString    cellContent;
            QString    cellStyle = "border: 1px solid black;";

            if (it == boardElement.end()) {
                if (board.IsPositionValid({i, j}, Card(2, PlayerTurn::Player1))) {
                    cellContent = " V ";
                    cellStyle += "background-color: green;";
                } else {
                    cellContent = " X ";
                    cellStyle += "background-color: red;";
                }
            } else {
                const auto card = it->second.top();

                if (card.GetIsFlipped()) {
                    cellContent = " H ";
                    cellStyle += "background-color: #FFFF99;"; // toned down yellow
                } else if (card.GetIsEter()) {
                    cellContent = QString(" %1 ").arg(card.GetValue());
                    cellStyle += "background-color: #00FFFF;"; // different color for eter card
                } else {
                    cellContent = QString(" %1 ").arg(card.GetValue());
                    if (card.GetPlacedBy() == PlayerTurn::Player1) {
                        cellStyle += QString("background-color: %1;").arg(m_Player1Color.name());
                    } else {
                        cellStyle += QString("background-color: %1;").arg(m_Player2Color.name());
                    }
                }

                if (card.GetIsEter()) {
                    if (card.GetPlacedBy() == PlayerTurn::Player1) {
                        cellStyle += QString("border: 2px solid %1;").arg(m_Player1Color.name());
                    } else {
                        cellStyle += QString("border: 2px solid %1;").arg(m_Player2Color.name());
                    }
                }
            }

            const auto cellButton = new QPushButton(cellContent, this);
            cellButton->setFixedSize(100, 100);
            cellButton->setStyleSheet(cellStyle);

            connect(cellButton, &QPushButton::clicked,
                    [cellClickedCallback, i, j] { cellClickedCallback({i, j}); });
            boardLayout->addWidget(cellButton, j - up.second, i - left.first);
        }
    }

    boardLayout->setRowStretch(down.second - up.second + 1, 1);
    boardLayout->setColumnStretch(right.first - left.first + 1, 1);

    return boardLayout;
}

QHBoxLayout *MainWindow::GenerateHand(const Hand                      &hand,
                                      const std::function<void(Card)> &cellClickedCallback) {
    const auto cardsLayout    = new QHBoxLayout();
    auto       selectedButton = std::make_shared<QPushButton *>(nullptr);

    for (const auto &card : hand) {
        const auto button = new QPushButton();
        button->setFixedSize(100, 150); // Adjust size as needed

        QPixmap cardImage = LoadCardImage(card.GetValue());
        if (!cardImage.isNull()) {
            button->setIcon(QIcon(cardImage));
            button->setIconSize(button->size());
        } else {
            button->setText(QString::number(card.GetValue()));
        }

        button->setStyleSheet("border: 1px solid black;");
        connect(button, &QPushButton::clicked,
                [this, selectedButton, button, card, cellClickedCallback] {
                    if (*selectedButton) {
                        (*selectedButton)->setStyleSheet("border: 1px solid black;");
                    }
                    *selectedButton = button;

                    QColor highlightColor =
                            (m_CurrentGame->GetCurrentPlayer() == PlayerTurn::Player1)
                                    ? m_Player1Color
                                    : m_Player2Color;
                    button->setStyleSheet(
                            QString("border: 2px solid %1;").arg(highlightColor.name()));

                    cellClickedCallback(card);
                });

        cardsLayout->addWidget(button);
    }

    return cardsLayout;
}
void MainWindow::ShowWinningMessage(const QString &winnerName) {
    const auto winningWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(winningWidget);

    const auto label     = new QLabel(QString("%1 has won the game!").arg(winnerName), this);
    QFont      labelFont = label->font();
    labelFont.setPointSize(24);
    labelFont.setBold(true);
    label->setFont(labelFont);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    const auto okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, [this, winningWidget, winnerName] {
        m_StackedWidget->removeWidget(winningWidget);
        if (winnerName == QString::fromStdString(m_CurrentGame->GetPlayer1().GetUserName())) {
            ++m_Player1Score;
        } else {
            ++m_Player2Score;
        }
        UpdateScoreLabel();
        if (++m_GamesPlayed < 3) {
            StartNewGame();
        } else {
            const auto finalScoreWidget = new QWidget(this);
            const auto finalLayout      = new QVBoxLayout(finalScoreWidget);

            const auto finalScoreLabel = new QLabel(
                    QString("Final Score:\n%1: %2\n%3: %4")
                            .arg(QString::fromStdString(m_CurrentGame->GetPlayer1().GetUserName()))
                            .arg(m_Player1Score)
                            .arg(QString::fromStdString(m_CurrentGame->GetPlayer2().GetUserName()))
                            .arg(m_Player2Score),
                    this);
            QFont finalScoreFont = finalScoreLabel->font();
            finalScoreFont.setPointSize(24);
            finalScoreFont.setBold(true);
            finalScoreLabel->setFont(finalScoreFont);
            finalScoreLabel->setAlignment(Qt::AlignCenter);
            finalLayout->addWidget(finalScoreLabel);

            const auto finalOkButton = new QPushButton("OK", this);
            finalLayout->addWidget(finalOkButton);
            connect(finalOkButton, &QPushButton::clicked, this, [this, finalScoreWidget] {
                m_StackedWidget->removeWidget(finalScoreWidget);
                DrawMenu();
            });

            finalLayout->setAlignment(Qt::AlignCenter);

            m_StackedWidget->addWidget(finalScoreWidget);
            m_StackedWidget->setCurrentWidget(finalScoreWidget);
        }
    });

    layout->setAlignment(Qt::AlignCenter);

    m_StackedWidget->addWidget(winningWidget);
    m_StackedWidget->setCurrentWidget(winningWidget);
}

void MainWindow::StartNewGame() {
    m_CurrentGame = std::make_unique<Antrenament>(m_CurrentGame->GetPlayer1().GetUserName(),
                                                  m_CurrentGame->GetPlayer2().GetUserName());
    DrawAntrenament();
}

void MainWindow::UpdateScoreLabel() const {
    if (m_CurrentState == GameState::InGame) {
        m_ScoreLabel->setText(
                QString("Player 1: %1 - Player 2: %2").arg(m_Player1Score).arg(m_Player2Score));
        m_ScoreLabel->show();
    } else {
        m_ScoreLabel->hide();
    }
}

void MainWindow::DrawAntrenament() {
    const auto antrenamentWidget = new QWidget(this);
    const auto layout            = new QVBoxLayout(antrenamentWidget);

    const auto  currentGame   = dynamic_cast<Antrenament *>(m_CurrentGame.get());
    const auto  currentTurn   = currentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? currentGame->GetPlayer1()
                                                                   : currentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    layout->addWidget(turnLabel);

    // Add the score label to the layout
    layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(currentGame->GetBoard(), [this, selectedCard, currentTurn, currentGame,
                                                    currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }
                auto &board = currentGame->GetBoard();

                const auto properCard = selectedCard->value();

                if (board.InsertCard(properCard, pos, currentTurn)) {
                    if (currentGame->CheckWinningConditions()) {
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    currentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                           ? PlayerTurn::Player2
                                                           : PlayerTurn::Player1);
                    DrawAntrenament();
                    m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    const auto handLayout = GenerateHand(
            currentHand, [this, selectedCard](const Card &card) { *selectedCard = card; });

    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(antrenamentWidget);
    m_StackedWidget->setCurrentWidget(antrenamentWidget);
}

void MainWindow::DrawDuelulVrajitorilor() {
    const auto duelulVrajitorilorWidget = new QWidget(this);
    const auto layout                   = new QVBoxLayout(duelulVrajitorilorWidget);

    const auto  currentGame   = dynamic_cast<Antrenament *>(m_CurrentGame.get());
    const auto  currentTurn   = currentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? currentGame->GetPlayer1()
                                                                   : currentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    layout->addWidget(turnLabel);

    layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(currentGame->GetBoard(), [this, selectedCard, currentTurn, currentGame,
                                                    currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }
                auto &board = currentGame->GetBoard();

                const auto properCard = selectedCard->value();

                if (board.InsertCard(properCard, pos, currentTurn)) {
                    if (currentGame->CheckWinningConditions()) {
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    currentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                           ? PlayerTurn::Player2
                                                           : PlayerTurn::Player1);
                    DrawDuelulVrajitorilor();
                    m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    const auto handLayout = GenerateHand(
            currentHand, [this, selectedCard](const Card &card) { *selectedCard = card; });

    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(duelulVrajitorilorWidget);
    m_StackedWidget->setCurrentWidget(duelulVrajitorilorWidget);
}

void MainWindow::DrawDuelulElementelor() {
    const auto duelulElementelorWidget = new QWidget(this);
    const auto layout                  = new QVBoxLayout(duelulElementelorWidget);

    const auto  currentGame   = dynamic_cast<Antrenament *>(m_CurrentGame.get());
    const auto  currentTurn   = currentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? currentGame->GetPlayer1()
                                                                   : currentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    layout->addWidget(turnLabel);

    layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(currentGame->GetBoard(), [this, selectedCard, currentTurn, currentGame,
                                                    currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }
                auto &board = currentGame->GetBoard();

                const auto properCard = selectedCard->value();

                if (board.InsertCard(properCard, pos, currentTurn)) {
                    if (currentGame->CheckWinningConditions()) {
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    currentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                           ? PlayerTurn::Player2
                                                           : PlayerTurn::Player1);
                    DrawDuelulElementelor();
                    m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    const auto handLayout = GenerateHand(
            currentHand, [this, selectedCard](const Card &card) { *selectedCard = card; });

    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(duelulElementelorWidget);
    m_StackedWidget->setCurrentWidget(duelulElementelorWidget);
}

void MainWindow::DrawTurneu() {
    const auto turneuWidget = new QWidget(this);
    const auto layout       = new QVBoxLayout(turneuWidget);

    const auto  currentGame   = dynamic_cast<Antrenament *>(m_CurrentGame.get());
    const auto  currentTurn   = currentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? currentGame->GetPlayer1()
                                                                   : currentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    layout->addWidget(turnLabel);

    layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(currentGame->GetBoard(), [this, selectedCard, currentTurn, currentGame,
                                                    currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }
                auto &board = currentGame->GetBoard();

                const auto properCard = selectedCard->value();

                if (board.InsertCard(properCard, pos, currentTurn)) {
                    if (currentGame->CheckWinningConditions()) {
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    currentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                           ? PlayerTurn::Player2
                                                           : PlayerTurn::Player1);
                    DrawTurneu();
                    m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    const auto handLayout = GenerateHand(
            currentHand, [this, selectedCard](const Card &card) { *selectedCard = card; });

    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(turneuWidget);
    m_StackedWidget->setCurrentWidget(turneuWidget);
}

void MainWindow::DrawRapid() {
    const auto rapidWidget = new QWidget(this);
    const auto layout      = new QVBoxLayout(rapidWidget);

    const auto  currentGame   = dynamic_cast<Antrenament *>(m_CurrentGame.get());
    const auto  currentTurn   = currentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? currentGame->GetPlayer1()
                                                                   : currentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    layout->addWidget(turnLabel);

    layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(currentGame->GetBoard(), [this, selectedCard, currentTurn, currentGame,
                                                    currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }
                auto &board = currentGame->GetBoard();

                const auto properCard = selectedCard->value();

                if (board.InsertCard(properCard, pos, currentTurn)) {
                    if (currentGame->CheckWinningConditions()) {
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    currentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                           ? PlayerTurn::Player2
                                                           : PlayerTurn::Player1);
                    DrawRapid();
                    m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    const auto handLayout = GenerateHand(
            currentHand, [this, selectedCard](const Card &card) { *selectedCard = card; });

    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(rapidWidget);
    m_StackedWidget->setCurrentWidget(rapidWidget);
}

void MainWindow::DrawNewGame() {
    const auto newGameWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(newGameWidget);

    const auto label     = new QLabel("New Game Options", this);
    QFont      labelFont = label->font();
    labelFont.setPointSize(16);
    label->setFont(labelFont);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    CreateLabel("Player 1 Name", newGameWidget);
    const auto player1Input = new QLineEdit(this);
    layout->addWidget(player1Input);

    CreateLabel("Player 2 Name", newGameWidget);
    const auto player2Input = new QLineEdit(this);
    layout->addWidget(player2Input);

    CreateLabel("Select Game Mode:", newGameWidget);
    auto                          *buttonGroup = new QButtonGroup(this);
    const std::vector<std::string> gameModes   = {"Antrenament", "Duelul Vrajitorilor",
                                                  "Duelul Elementelor", "Turneu", "Rapid"};

    for (const auto &mode : gameModes) {
        const auto radioButton = new QRadioButton(QString::fromStdString(mode), this);
        buttonGroup->addButton(radioButton);
        layout->addWidget(radioButton);
    }

    const auto startGameButton = new QPushButton("Start Game", this);
    layout->addWidget(startGameButton);

    connect(startGameButton, &QPushButton::clicked, this,
            [this, player1Input, player2Input, buttonGroup] {
                const auto player1Name = player1Input->text();
                const auto player2Name = player2Input->text();

                if (player1Name.isEmpty() || player2Name.isEmpty()) {
                    qDebug() << "Player names cannot be empty!\n";
                    return;
                }

                if (buttonGroup->checkedButton() == nullptr) {
                    qDebug() << "Please select a game mode!\n";
                    return;
                }

                m_CurrentGameMode = buttonGroup->checkedButton()->text().toStdString();
                std::cout << "Selected game mode: " << m_CurrentGameMode << '\n';
                std::map<std::string, std::function<void()>> gameModeMap = {
                        {"&Antrenament",
                         [this, player1Name, player2Name] {
                             m_CurrentGame = std::make_unique<Antrenament>(
                                     player1Name.toStdString(), player2Name.toStdString());
                             DrawAntrenament();
                         }},
                        {"&Duelul Vrajitorilor",
                         [this, player1Name, player2Name] {
                             m_CurrentGame = std::make_unique<Antrenament>(
                                     player1Name.toStdString(), player2Name.toStdString());
                             DrawDuelulVrajitorilor();
                         }},
                        {"&Duelul Elementelor",
                         [this, player1Name, player2Name] {
                             m_CurrentGame = std::make_unique<Antrenament>(
                                     player1Name.toStdString(), player2Name.toStdString());
                             DrawDuelulElementelor();
                         }},
                        {"&Turneu",
                         [this, player1Name, player2Name] {
                             m_CurrentGame = std::make_unique<Antrenament>(
                                     player1Name.toStdString(), player2Name.toStdString());
                             DrawTurneu();
                         }},
                        {"&Rapid",
                         [this, player1Name, player2Name] {
                             m_CurrentGame = std::make_unique<Antrenament>(
                                     player1Name.toStdString(), player2Name.toStdString());
                             DrawRapid();
                         }},
                };

                if (gameModeMap.contains(m_CurrentGameMode)) {
                    m_CurrentState = GameState::InGame;
                    gameModeMap[m_CurrentGameMode]();
                } else {
                    qDebug() << "Selected game mode is not implemented!\n";
                }
            });

    layout->setSpacing(5);

    AddBackButton(newGameWidget);

    const auto mainLayout = CreateLimitedLayout(newGameWidget);

    m_StackedWidget->removeWidget(m_StackedWidget->currentWidget());

    const auto containerWidget = new QWidget(this);
    containerWidget->setLayout(mainLayout);
    m_StackedWidget->addWidget(containerWidget);
    m_StackedWidget->setCurrentWidget(containerWidget);
}

void MainWindow::DrawResumeGame() {
    const auto resumeGameWidget = new QWidget(this);
    const auto layout           = new QVBoxLayout(resumeGameWidget);

    const auto label = new QLabel("Resume Game Screen", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    AddBackButton(resumeGameWidget);

    m_StackedWidget->addWidget(resumeGameWidget);
    m_StackedWidget->setCurrentWidget(resumeGameWidget);
}

void MainWindow::DrawOptions() {
    const auto optionsWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(optionsWidget);

    const auto topLabel = new QLabel("Options Screen", this);
    topLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(topLabel);

    CreateLabel("Game Resolution:", optionsWidget);
    const auto resolutionComboBox = new QComboBox(this);
    resolutionComboBox->addItems({"1920x1080", "1280x720", "1024x768"});
    resolutionComboBox->setCurrentText(m_GameResolution.isEmpty() ? "1920x1080" : m_GameResolution);
    layout->addWidget(resolutionComboBox);
    connect(resolutionComboBox, &QComboBox::currentTextChanged, this,
            [this](const QString &text) { m_GameResolution = text; });

    CreateLabel("Full Screen:", optionsWidget);
    const auto fullScreenCheckBox = new QCheckBox("Full Screen", this);
    fullScreenCheckBox->setChecked(m_FullScreen);
    layout->addWidget(fullScreenCheckBox);
    connect(fullScreenCheckBox, &QCheckBox::toggled, this, [this](const bool checked) {
        m_FullScreen = checked;

        if (checked) {
            showFullScreen();
        } else {
            showNormal();
        }
    });

    CreateColorPicker("Player 1 Color:", m_Player1Color, optionsWidget);
    CreateColorPicker("Player 2 Color:", m_Player2Color, optionsWidget);

    CreateLabel("Master Volume:", optionsWidget);
    CreateSlider(m_MasterVolume, optionsWidget);

    CreateLabel("Music Volume:", optionsWidget);
    CreateSlider(m_MusicVolume, optionsWidget);

    CreateLabel("SFX Volume:", optionsWidget);
    CreateSlider(m_SfxVolume, optionsWidget);

    const auto clearSavedGamesButton = new QPushButton("Clear Saved Games", this);
    layout->addWidget(clearSavedGamesButton);

    AddBackButton(optionsWidget);

    const auto mainLayout = CreateLimitedLayout(optionsWidget);

    m_StackedWidget->removeWidget(m_StackedWidget->currentWidget());

    const auto containerWidget = new QWidget(this);
    containerWidget->setLayout(mainLayout);

    m_StackedWidget->addWidget(containerWidget);
    m_StackedWidget->setCurrentWidget(containerWidget);
}

void MainWindow::CreateLabel(const QString &text, QWidget *parent) {
    const auto layout = qobject_cast<QVBoxLayout *>(parent->layout());
    if (layout) {
        const auto label = new QLabel(text, parent);
        layout->addWidget(label);
    }
}

void MainWindow::CreateSlider(int &value, QWidget *parent) {
    const auto layout = qobject_cast<QVBoxLayout *>(parent->layout());
    if (layout) {
        const auto slider = new QSlider(Qt::Horizontal, parent);
        slider->setRange(0, 100);
        slider->setValue(value);
        layout->addWidget(slider);
        connect(slider, &QSlider::valueChanged, this,
                [&value](const int newValue) { value = newValue; });
    }
}

void MainWindow::CreateColorPicker(const QString &labelText, QColor &color, QWidget *parent) {
    const auto layout = qobject_cast<QVBoxLayout *>(parent->layout());
    if (layout) {
        CreateLabel(labelText, parent);

        const auto colorPicker = new QPushButton("Choose Color", parent);
        const auto colorFrame  = new QFrame(parent);
        colorFrame->setFixedSize(20, 20);
        colorFrame->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                          .arg(color.isValid() ? color.name() : "#FFFFFF"));

        connect(colorPicker, &QPushButton::clicked, this, [this, &color, colorFrame]() {
            const auto newColor = QColorDialog::getColor(color, this, "Select Color");
            if (newColor.isValid()) {
                color = newColor;
                colorFrame->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                                  .arg(newColor.name()));
            }
        });

        const auto colorLayout = new QHBoxLayout();
        colorLayout->addWidget(colorPicker);
        colorLayout->addWidget(colorFrame);
        layout->addLayout(colorLayout);
    }
}

QVBoxLayout *MainWindow::CreateLimitedLayout(QWidget *widget) {
    const auto mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    const auto hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(widget);
    hLayout->addStretch();
    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();
    widget->setMaximumWidth(600); // Set the maximum width
    return mainLayout;
}

void MainWindow::OnNewGameClicked() {
    m_CurrentState = GameState::NewGame;
    DrawNewGame();
}

void MainWindow::OnResumeGameClicked() {
    m_CurrentState = GameState::ResumeGame;
    DrawResumeGame();
}

void MainWindow::OnOptionsClicked() {
    m_CurrentState = GameState::Options;
    DrawOptions();
}

void MainWindow::OnExitClicked() {
    m_CurrentState = GameState::Exit;
    QApplication::exit();
}

void MainWindow::OnBackClicked() {
    m_CurrentState = GameState::MainMenu;
    DrawMenu();
}
