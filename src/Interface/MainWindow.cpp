//
// Created by mavri on 05.12.2024.
//

#include "MainWindow.h"
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_CurrentState(GameState::MainMenu) {
    m_StackedWidget = new QStackedWidget(this);
    setCentralWidget(m_StackedWidget);
    DrawMenu();
}

void MainWindow::AddBackButton(const QWidget *widget) {
    const auto backButton = new QPushButton("<", this);
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

    titleFont.setPointSize(32);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    const auto startButton = new QPushButton("New Game", this);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::OnNewGameClicked);
    layout->addWidget(startButton);

    const auto resumeButton = new QPushButton("Resume Game", this);
    connect(resumeButton, &QPushButton::clicked, this, &MainWindow::OnResumeGameClicked);
    layout->addWidget(resumeButton);

    const auto optionsButton = new QPushButton("Options", this);
    connect(optionsButton, &QPushButton::clicked, this, &MainWindow::OnOptionsClicked);
    layout->addWidget(optionsButton);

    const auto exitButton = new QPushButton("Exit", this);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::OnExitClicked);
    layout->addWidget(exitButton);

    layout->setAlignment(Qt::AlignCenter);

    m_StackedWidget->addWidget(menuWidget);
    m_StackedWidget->setCurrentWidget(menuWidget);
}

void MainWindow::DrawNewGame() {
    const auto newGameWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(newGameWidget);

    const auto label     = new QLabel("New Game Options", this);
    QFont      labelFont = label->font();
    labelFont.setPointSize(16); // Make the label font smaller
    label->setFont(labelFont);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    const auto player1Name = new QLabel("Player 1 Name", this);
    player1Name->setFont(labelFont);
    layout->addWidget(player1Name);

    const auto player1Input = new QLineEdit(this);
    layout->addWidget(player1Input);

    const auto player2Name = new QLabel("Player 2 Name", this);
    player2Name->setFont(labelFont);
    layout->addWidget(player2Name);

    const auto player2Input = new QLineEdit(this);
    layout->addWidget(player2Input);

    const auto gameModeLabel = new QLabel("Select Game Mode:", this);
    gameModeLabel->setFont(labelFont);
    layout->addWidget(gameModeLabel);

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

    layout->setSpacing(5);

    AddBackButton(newGameWidget);

    m_StackedWidget->addWidget(newGameWidget);
    m_StackedWidget->setCurrentWidget(newGameWidget);
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

    const auto label = new QLabel("Options Screen", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // Game Resolution
    const auto resolutionLabel = new QLabel("Game Resolution:", this);
    layout->addWidget(resolutionLabel);

    const auto resolutionComboBox = new QComboBox(this);
    resolutionComboBox->addItems({"1920x1080", "1280x720", "1024x768"});
    layout->addWidget(resolutionComboBox);

    // Full Screen
    const auto fullScreenCheckBox = new QCheckBox("Full Screen", this);
    layout->addWidget(fullScreenCheckBox);

    // Player Colors
    const auto player1ColorLabel = new QLabel("Player 1 Color:", this);
    layout->addWidget(player1ColorLabel);

    const auto player1ColorPicker = new QPushButton("Choose Color", this);
    const auto player1ColorFrame  = new QFrame(this);
    player1ColorFrame->setFixedSize(20, 20);
    player1ColorFrame->setStyleSheet(
            "background-color: #AEC6CF; border: 1px solid black;"); // Pastel blue

    connect(player1ColorPicker, &QPushButton::clicked, this, [this, player1ColorFrame]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Player 1 Color");
        if (color.isValid()) {
            player1ColorFrame->setStyleSheet(
                    QString("background-color: %1; border: 1px solid black;").arg(color.name()));
        }
    });

    auto player1Layout = new QHBoxLayout();
    player1Layout->addWidget(player1ColorPicker);
    player1Layout->addWidget(player1ColorFrame);
    layout->addLayout(player1Layout);

    const auto player2ColorLabel = new QLabel("Player 2 Color:", this);
    layout->addWidget(player2ColorLabel);

    const auto player2ColorPicker = new QPushButton("Choose Color", this);
    const auto player2ColorFrame  = new QFrame(this);
    player2ColorFrame->setFixedSize(20, 20);
    player2ColorFrame->setStyleSheet(
            "background-color: #FFB3BA; border: 1px solid black;"); // Pastel red

    connect(player2ColorPicker, &QPushButton::clicked, this, [this, player2ColorFrame]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Player 2 Color");
        if (color.isValid()) {
            player2ColorFrame->setStyleSheet(
                    QString("background-color: %1; border: 1px solid black;").arg(color.name()));
        }
    });

    auto player2Layout = new QHBoxLayout();
    player2Layout->addWidget(player2ColorPicker);
    player2Layout->addWidget(player2ColorFrame);
    layout->addLayout(player2Layout);

    // Master Volume
    const auto masterVolumeLabel = new QLabel("Master Volume:", this);
    layout->addWidget(masterVolumeLabel);

    const auto masterVolumeSlider = new QSlider(Qt::Horizontal, this);
    masterVolumeSlider->setRange(0, 100);
    layout->addWidget(masterVolumeSlider);

    // Music Volume
    const auto musicVolumeLabel = new QLabel("Music Volume:", this);
    layout->addWidget(musicVolumeLabel);

    const auto musicVolumeSlider = new QSlider(Qt::Horizontal, this);
    musicVolumeSlider->setRange(0, 100);
    layout->addWidget(musicVolumeSlider);

    // SFX Volume
    const auto sfxVolumeLabel = new QLabel("SFX Volume:", this);
    layout->addWidget(sfxVolumeLabel);

    const auto sfxVolumeSlider = new QSlider(Qt::Horizontal, this);
    sfxVolumeSlider->setRange(0, 100);
    layout->addWidget(sfxVolumeSlider);

    // Clear Saved Games Button
    const auto clearSavedGamesButton = new QPushButton("Clear Saved Games", this);
    layout->addWidget(clearSavedGamesButton);

    AddBackButton(optionsWidget);

    m_StackedWidget->addWidget(optionsWidget);
    m_StackedWidget->setCurrentWidget(optionsWidget);
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
