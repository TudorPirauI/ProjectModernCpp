//
// Created by mavri on 05.12.2024.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QAudioOutput>
#include <QButtonGroup>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFont>
#include <QGraphicsBlurEffect>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QStackedWidget>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include <iostream>

#include "../Game/Antrenament.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void AddBackButton(const QWidget *widget);

private slots:
    void OnNewGameClicked();
    void OnResumeGameClicked();
    void OnOptionsClicked();
    void OnExitClicked();
    void OnBackClicked();

private:
    enum class GameState { MainMenu, NewGame, InGame, ResumeGame, Options, Exit };

    GameState       m_CurrentState;
    QStackedWidget *m_StackedWidget;

    std::unique_ptr<Game> m_CurrentGame;
    std::optional<Card>   m_SelectedCard;

    // TODO: We should load the settings from a file and only fall back to these defaults if the
    // file is missing or corrupted
    QString m_GameResolution{"1920x1080"};
    bool    m_FullScreen{false};
    QColor  m_Player1Color{QColor(173, 216, 230)};
    QColor  m_Player2Color{QColor(255, 105, 97)};
    int     m_MasterVolume{20};
    int     m_MusicVolume{20};
    int     m_SfxVolume{20};

    int     m_GamesPlayed  = 0;
    int     m_Player1Score = 0;
    int     m_Player2Score = 0;
    QLabel *m_ScoreLabel   = nullptr;
    void    UpdateScoreLabel();
    void    StartNewGame();

    // QMediaPlayer *m_MediaPlayer;

    QGridLayout *GenerateBoard(const Board                         &board,
                               const std::function<void(Position)> &cellClickedCallback);
    QHBoxLayout *GenerateHand(const Hand                      &hand,
                              const std::function<void(Card)> &cellClickedCallback);
    void         ShowWinningMessage(const QString &winnerName);

    void DrawAntrenament();
    void DrawMenu();
    void DrawNewGame();
    void DrawResumeGame();
    void DrawOptions();

    // Helper Functions
    void         CreateLabel(const QString &text, QWidget *parent);
    void         CreateSlider(int &value, QWidget *parent);
    void         CreateColorPicker(const QString &labelText, QColor &color, QWidget *parent);
    QVBoxLayout *CreateLimitedLayout(QWidget *widget);
};

#endif // MAINWINDOW_H
