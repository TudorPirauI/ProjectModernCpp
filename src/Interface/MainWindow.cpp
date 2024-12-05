//
// Created by mavri on 05.12.2024.
//

#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_CurrentState(GameState::MainMenu) {
    m_StackedWidget = new QStackedWidget(this);
    setCentralWidget(m_StackedWidget);
    DrawMenu();
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

    const auto label = new QLabel("New Game Screen", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_StackedWidget->addWidget(newGameWidget);
    m_StackedWidget->setCurrentWidget(newGameWidget);
}

void MainWindow::DrawResumeGame() {
    const auto resumeGameWidget = new QWidget(this);
    const auto layout           = new QVBoxLayout(resumeGameWidget);

    const auto label = new QLabel("Resume Game Screen", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_StackedWidget->addWidget(resumeGameWidget);
    m_StackedWidget->setCurrentWidget(resumeGameWidget);
}

void MainWindow::DrawOptions() {
    const auto optionsWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(optionsWidget);

    const auto label = new QLabel("Options Screen", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

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
    close();
}
