#include "Interface/MainWindow.h"
#include "Interface/IAntrenament.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_CurrentState(GameState::MainMenu) {
    m_StackedWidget = new QStackedWidget(this);
    // TODO: Add the music back
    //  Play music
    //      m_MediaPlayer   = new QMediaPlayer(this);
    //      m_AudioOutput   = new QAudioOutput(this);
    //      m_MediaPlayer->setAudioOutput(m_AudioOutput);

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

                const auto gameWidget = new QWidget(this);

                // TODO: Re-Add the switch case that handled the multiple game modes

                // IT IS IMPERATIVE THAT THIS REMAINS A POINTER, OTHERWISE THE SLOTS AND SIGNALS
                // WON'T WORK
                auto *antrenamentGame = new IAntrenament(player1Name.toStdString(),
                                                         player2Name.toStdString(), gameWidget);

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

    // TODO: Fix music playing
    //  Play music
    //      m_MediaPlayer->setSource(QUrl::fromLocalFile("path/to/your/media/file.mp3"));
    //      m_AudioOutput->setVolume(m_MusicVolume / 100.0);
    //      m_MediaPlayer->play();
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
