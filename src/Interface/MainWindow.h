//
// Created by mavri on 05.12.2024.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>

#include <QMediaPlayer>

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
    enum class GameState { MainMenu, NewGame, ResumeGame, Options, Exit };

    GameState       m_CurrentState;
    QStackedWidget *m_StackedWidget;

    QString m_GameResolution{"1920x1080"};
    bool    m_FullScreen{true};
    QColor  m_Player1Color{};
    QColor  m_Player2Color{};
    int     m_MasterVolume{20};
    int     m_MusicVolume{20};
    int     m_SfxVolume{20};

    void DrawMenu();
    void DrawNewGame();
    void DrawResumeGame();
    void DrawOptions();
};

#endif // MAINWINDOW_H
