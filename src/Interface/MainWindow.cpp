#include "Interface/MainWindow.h"

#include "GameComponents/JsonUtils.h"
#include "Interface/AlertWidget.h"
#include "Interface/IAntrenament.h"
#include "Interface/IDuelulCombinat.h"
#include "Interface/IDuelulElementelor.h"
#include "Interface/IDuelulVrajitorilor.h"

QColor MainWindow::m_Player1Color{QColor(173, 216, 230)};
QColor MainWindow::m_Player2Color{QColor(255, 105, 97)};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_CurrentState(GameState::MainMenu) {
    m_StackedWidget = new QStackedWidget(this);

    const auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    const auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(m_StackedWidget);

    setMinimumSize(1920, 1080);

    if (m_FullScreen) {
        showFullScreen();
    } else {
        showNormal();
    }

    m_MediaPlayer = new QMediaPlayer(this);
    m_AudioOutput = new QAudioOutput(this);

    m_MediaPlayer->setAudioOutput(m_AudioOutput);
    m_AudioOutput->setVolume(m_MusicVolume / 700.0);

    const QString musicFilePath =
            "/home/mavri/Documents/GitHub/ProjectModernCpp/resources/sounds/background_music.wav";
    if (!QFile::exists(musicFilePath)) {
        qDebug() << "Music file not found:" << musicFilePath << '\n';
    } else {
        m_MediaPlayer->setSource(QUrl::fromLocalFile(musicFilePath));
        m_MediaPlayer->setLoops(QMediaPlayer::Infinite);
        std::cout << "Playing music\n";
    }

    m_MediaPlayer->play();

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

        connect(button, &QPushButton::clicked, this, [this, slot] { (this->*slot)(); });
        layout->addWidget(button);
    };

    createButton("New Game", &MainWindow::OnNewGameClicked);
    createButton("Resume Game", &MainWindow::OnResumeGameClicked);
    createButton("Options", &MainWindow::OnOptionsClicked);
    createButton("Exit", &MainWindow::OnExitClicked);

    layout->setAlignment(Qt::AlignCenter);

    m_StackedWidget->addWidget(menuWidget);
    m_StackedWidget->setCurrentWidget(menuWidget);

    // m_ScoreLabel->hide();
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
                                                  "Duelul Elementelor", "Duelul Combinat"};

    const auto modeLayout = new QHBoxLayout();
    const auto modeWidget = new QWidget(this);
    modeWidget->setLayout(modeLayout);

    const auto radioButtonsLayout = new QVBoxLayout();
    for (const auto &mode : gameModes) {
        const auto radioButton = new QRadioButton(QString::fromStdString(mode), this);
        buttonGroup->addButton(radioButton);
        radioButtonsLayout->addWidget(radioButton);
    }
    modeLayout->addLayout(radioButtonsLayout);

    const auto optionsLayout = new QVBoxLayout();
    const auto optionsWidget = new QWidget(this);
    optionsWidget->setLayout(optionsLayout);

    CreateLabel("Gamemode Options", optionsWidget);

    const auto eterCheckBox = new QCheckBox("Eter", this);
    optionsLayout->addWidget(eterCheckBox);

    const auto illusionCheckBox = new QCheckBox("Illusion", this);
    optionsLayout->addWidget(illusionCheckBox);

    const auto explosionCheckBox = new QCheckBox("Explosion", this);
    optionsLayout->addWidget(explosionCheckBox);

    CreateLabel("Time Limit:", optionsWidget);
    const auto timeLimitComboBox = new QComboBox(this);
    timeLimitComboBox->addItem("None", 0);
    timeLimitComboBox->addItem("60 seconds", 60);
    timeLimitComboBox->addItem("90 seconds", 90);
    timeLimitComboBox->addItem("120 seconds", 120);
    optionsLayout->addWidget(timeLimitComboBox);

    modeLayout->addWidget(optionsWidget);
    layout->addWidget(modeWidget);

    const auto startGameButton = new QPushButton("Start Game", this);
    layout->addWidget(startGameButton);

    connect(startGameButton, &QPushButton::clicked, this,
            [this, player1Input, player2Input, buttonGroup, eterCheckBox, illusionCheckBox,
             explosionCheckBox, timeLimitComboBox] {
                const auto player1Name = player1Input->text();
                const auto player2Name = player2Input->text();

                if (player1Name.isEmpty() || player2Name.isEmpty()) {
                    qDebug() << "Player names cannot be empty!\n";
                    const auto alertWidget = new AlertWidget(this);

                    alertWidget->ShowAlert("Player names cannot be empty!");

                    return;
                }

                if (buttonGroup->checkedButton() == nullptr) {
                    qDebug() << "Please select a game mode!\n";

                    const auto alertWidget = new AlertWidget(this);

                    alertWidget->ShowAlert("You must select a game mode!");

                    return;
                }

                const bool eterResponse      = eterCheckBox->isChecked();
                const bool illusionResponse  = illusionCheckBox->isChecked();
                const bool explosionResponse = explosionCheckBox->isChecked();
                const int  timeLimit         = timeLimitComboBox->currentData().toInt();

                m_CurrentGameMode =
                        buttonGroup->checkedButton()->text().replace("&", "").toStdString();
                // this removes the mnemonic character from the text (thanks QT)
                std::cout << "Selected game mode: " << m_CurrentGameMode << '\n';

                const auto gameWidget = new QWidget(this);

                if (m_CurrentGameMode == "Antrenament") {
                    const std::array options = {eterResponse, illusionResponse, explosionResponse};
                    const auto      *antrenamentGame =
                            new IAntrenament(player1Name.toStdString(), player2Name.toStdString(),
                                             options, gameWidget, timeLimit);

                    connect(antrenamentGame, &IAntrenament::GameFinished, this,
                            &MainWindow::OnGameFinished);
                } else if (m_CurrentGameMode == "Duelul Elementelor") {
                    const std::array options = {eterResponse, illusionResponse, explosionResponse};
                    const auto      *duelulElementelorGame =
                            new IDuelulElementelor(player1Name.toStdString(),
                                                   player2Name.toStdString(), options, gameWidget);

                    connect(duelulElementelorGame, &IDuelulElementelor::GameFinished, this,
                            &MainWindow::OnGameFinished);
                } else if (m_CurrentGameMode == "Duelul Vrajitorilor") {
                    const std::array options = {eterResponse, illusionResponse, explosionResponse};
                    const auto      *duelulVrajitorilorGame =
                            new IDuelulVrajitorilor(player1Name.toStdString(),
                                                    player2Name.toStdString(), options, gameWidget);

                    connect(duelulVrajitorilorGame, &IDuelulVrajitorilor::GameFinished, this,
                            &MainWindow::OnGameFinished);

                } else if (m_CurrentGameMode == "Duelul Combinat") {
                    const std::array options = {eterResponse, illusionResponse, explosionResponse};
                    const auto      *duelulVrajitorilorGame =
                            new IDuelulCombinat(player1Name.toStdString(),
                                                player2Name.toStdString(), options, gameWidget);

                    connect(duelulVrajitorilorGame, &IDuelulCombinat::GameFinished, this,
                            &MainWindow::OnGameFinished);
                }

                m_StackedWidget->removeWidget(m_StackedWidget->currentWidget());
                m_StackedWidget->addWidget(gameWidget);
                m_StackedWidget->setCurrentWidget(gameWidget);
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

    const auto label     = new QLabel("Resume Game Screen", this);
    QFont      labelFont = label->font();
    labelFont.setPointSize(24);
    label->setFont(labelFont);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_GameListWidget = new QListWidget(this);
    QDir savesDir(QCoreApplication::applicationDirPath() + "/saves");
    if (!savesDir.exists()) {
        savesDir.mkpath(".");
    }

    const auto gameList = savesDir.entryList(QStringList() << "*.json", QDir::Files);

    m_GameListWidget->addItems(gameList);
    m_GameListWidget->setFixedSize(400, 300);
    layout->addWidget(m_GameListWidget, 0, Qt::AlignCenter);

    QFont listFont = m_GameListWidget->font();
    listFont.setPointSize(18);
    m_GameListWidget->setFont(listFont);

    const auto buttonLayout = new QHBoxLayout();

    const auto loadGameButton = new QPushButton("Load Game", this);
    QFont      buttonFont     = loadGameButton->font();
    buttonFont.setPointSize(18);
    loadGameButton->setFont(buttonFont);
    buttonLayout->addWidget(loadGameButton);

    connect(loadGameButton, &QPushButton::clicked, this, [this] {
        const auto selectedItem = m_GameListWidget->currentItem();
        if (selectedItem) {
            const QString selectedGame = selectedItem->text();
            qDebug() << "Loading game:" << selectedGame;

            if (selectedItem->text().contains("antrenament")) {
                QFile file(QCoreApplication::applicationDirPath() + "/saves/" +
                           selectedItem->text());
                if (!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::warning(this, "Error", "Could not open save file.");
                    return;
                }

                const QByteArray    saveData = file.readAll();
                const QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
                const QJsonObject   json = jsonDoc.object();

                Antrenament game = JsonUtils::JsonToTrainingMode(json);

                const auto  gameWidget      = new QWidget(this);
                const auto *antrenamentGame = new IAntrenament(game, gameWidget);

                connect(antrenamentGame, &IAntrenament::GameFinished, this,
                        &MainWindow::OnGameFinished);

                m_StackedWidget->removeWidget(m_StackedWidget->currentWidget());
                m_StackedWidget->addWidget(gameWidget);
                m_StackedWidget->setCurrentWidget(gameWidget);
            }
        } else {
            qDebug() << "No game selected!";
        }
    });

    const auto removeGameButton = new QPushButton("Remove Game", this);
    removeGameButton->setFont(buttonFont);
    buttonLayout->addWidget(removeGameButton);

    connect(removeGameButton, &QPushButton::clicked, this, [this] {
        const auto selectedItem = m_GameListWidget->currentItem();
        if (selectedItem) {
            const QString selectedGame = selectedItem->text();
            delete m_GameListWidget->takeItem(m_GameListWidget->row(selectedItem));
            qDebug() << "Removed game:" << selectedGame;
        } else {
            qDebug() << "No game selected!";
        }
    });

    layout->addLayout(buttonLayout);

    AddBackButton(resumeGameWidget);

    m_StackedWidget->addWidget(resumeGameWidget);
    m_StackedWidget->setCurrentWidget(resumeGameWidget);
}

void MainWindow::OnResolutionChanged(const QString &resolution) {
    const QStringList dimensions = resolution.split('x');
    if (dimensions.size() == 2) {
        const int width  = dimensions[0].toInt();
        const int height = dimensions[1].toInt();
        setFixedSize(width, height);
    }
}

void MainWindow::DrawOptions() {
    const auto optionsWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(optionsWidget);

    const auto topLabel = new QLabel("Options Screen", this);
    topLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(topLabel);

    CreateLabel("Game Resolution:", optionsWidget);
    const auto resolutionComboBox = new QComboBox(this);
    resolutionComboBox->addItems({"2560x1440", "1280x720", "1024x768"});
    resolutionComboBox->setCurrentText(m_GameResolution.isEmpty() ? "2560x1440" : m_GameResolution);
    layout->addWidget(resolutionComboBox);
    connect(resolutionComboBox, &QComboBox::currentTextChanged, this,
            &MainWindow::OnResolutionChanged);

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

        connect(colorPicker, &QPushButton::clicked, this, [this, &color, colorFrame] {
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

void MainWindow::OnGameFinished() {
    m_CurrentState = GameState::MainMenu;
    DrawMenu();
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

QColor MainWindow::GetPlayer1Color() const { return m_Player1Color; }

void MainWindow::SetPlayer1Color(const QColor &color) { m_Player1Color = color; }

QColor MainWindow::GetPlayer2Color() const { return m_Player2Color; }

void MainWindow::SetPlayer2Color(const QColor &color) { m_Player2Color = color; }
