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

    setMinimumSize(800, 600);

    if (m_FullScreen) {
        showFullScreen();
    } else {
        showNormal();
    }

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

    titleFont.setPointSize(48); // Increase the font size
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
}

// todo: probably fix the backend, it's fucking fucked.

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
                if (board.IsPositionValid(
                            {i, j},
                            Card(2, PlayerTurn::Player1))) { // todo: this may cause problems, it's
                                                             // here so it compiles lol
                    cellContent = " V ";
                    cellStyle += "background-color: green;";
                } else {
                    cellContent = " X ";
                    cellStyle += "background-color: red;";
                }
            } else {
                const auto card = it->second.top();

                std::cout << std::format(
                        "(x: {}, y: {}) -> Card {} (Illusion: {} | Flipped: {} | Eter: {})\n",
                        it->first.first, it->first.second, card.GetValue(), card.GetIsIllusion(),
                        card.GetIsFlipped(), card.GetIsEter());

                if (card.GetIsFlipped()) {
                    cellContent = " H ";
                    cellStyle += "background-color: yellow;";
                } else {
                    cellContent = QString(" %1 ").arg(card.GetValue());
                    if (card.GetPlacedBy() == PlayerTurn::Player1) {
                        cellStyle += QString("background-color: %1;").arg(m_Player1Color.name());
                    } else {
                        cellStyle += QString("background-color: %1;").arg(m_Player2Color.name());
                    }
                }
            }

            const auto cellButton = new QPushButton(cellContent);
            cellButton->setFixedSize(100, 100);
            cellButton->setStyleSheet(cellStyle);

            connect(cellButton, &QPushButton::clicked,
                    [cellClickedCallback, i, j] { cellClickedCallback({i, j}); });
            boardLayout->addWidget(cellButton, j - up.second, i - left.first);
        }
    }

    return boardLayout;
}

QHBoxLayout *MainWindow::GenerateHand(const Hand                      &hand,
                                      const std::function<void(Card)> &cellClickedCallback) {
    const auto cardsLayout = new QHBoxLayout();

    for (const auto &card : hand) {
        QString cardContent = QString::number(card.GetValue());
        QString cardStyle   = "border: 1px solid black;";

        const auto cardButton = new QPushButton(cardContent);
        cardButton->setFixedSize(100, 100);
        cardButton->setStyleSheet(cardStyle);

        connect(cardButton, &QPushButton::clicked, [cellClickedCallback, &card]() {
            qDebug() << "Card button clicked, value:" << card.GetValue();
            cellClickedCallback(card);
        });

        cardsLayout->addWidget(cardButton);
    }

    return cardsLayout;
}

void MainWindow::DrawAntrenament() {
    const auto antrenamentWidget = new QWidget(this);
    const auto layout            = new QVBoxLayout(antrenamentWidget);

    auto      &currentBoard  = m_CurrentGame.get()->GetBoard();
    const auto playerTurn    = m_CurrentGame.get()->GetCurrentPlayer();
    const auto currentPlayer = playerTurn == PlayerTurn::Player1
                                       ? m_CurrentGame.get()->GetPlayer1()
                                       : m_CurrentGame.get()->GetPlayer2();
    const auto currentHand   = currentPlayer.GetHand();

    for (const auto &c : currentHand) {
        std::cout << c.GetValue() << " ";
    }

    auto selectedCard = std::optional<Card>();

    // Generate and add the board layout
    const auto boardLayout = GenerateBoard(
            currentBoard, [this, &selectedCard, &currentBoard, &playerTurn](const Position &pos) {
                qDebug() << "Cell clicked at position:" << pos.first << "," << pos.second;

                if (!selectedCard || !selectedCard.has_value()) {
                    qDebug() << "No card selected!";
                    return;
                }

                const auto selected = selectedCard.value();

                qDebug() << "Selected card value:" << selected.GetValue()
                         << "Flipped:" << selected.GetIsFlipped();

                const auto value = selected.GetValue();

                std::cout << std::format("Selected card: {} at position: ({}, {})\n", value,
                                         pos.first, pos.second);

                if (currentBoard.InsertCard({value, playerTurn}, pos, playerTurn)) {
                    qDebug() << "Card placed successfully!";

                    if (m_CurrentGame->CheckWinningConditions(playerTurn)) {
                        qDebug() << "Player"
                                 << (playerTurn == PlayerTurn::Player1
                                             ? m_CurrentGame->GetPlayer1().GetUserName()
                                             : m_CurrentGame->GetPlayer2().GetUserName())
                                 << "won!";
                        return; // todo: draw winning screen
                    }

                    m_CurrentGame->SetNextPlayerTurn(m_CurrentGame.get()->GetCurrentPlayer() ==
                                                                     PlayerTurn::Player1
                                                             ? PlayerTurn::Player2
                                                             : PlayerTurn::Player1);
                    DrawAntrenament(); // Redraw the board for the next turn
                } else {
                    qDebug() << "Invalid move!";
                }
            });

    layout->addLayout(boardLayout);

    // Generate and add the hand layout
    const auto handLayout = GenerateHand(currentHand, [&selectedCard](const Card &card) mutable {
        qDebug() << "Card clicked with value:" << card.GetValue();
        selectedCard = std::make_optional(card);
        std::cout << "Selected card: " << selectedCard.value() << '\n';
    });
    layout->addLayout(handLayout);

    m_StackedWidget->addWidget(antrenamentWidget);
    m_StackedWidget->setCurrentWidget(antrenamentWidget);
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

                // const auto gameType = buttonGroup->checkedButton()->text();

                // todo: text -> class type

                m_CurrentState = GameState::InGame;
                m_CurrentGame  = std::make_unique<Antrenament>(player1Name.toStdString(),
                                                               player2Name.toStdString());
                DrawAntrenament();
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
