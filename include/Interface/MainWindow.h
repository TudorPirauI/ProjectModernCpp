//
// Created by mavri on 05.12.2024.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
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
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QStackedWidget>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

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

    // TODO: We should load the settings from a file and only fall back to these defaults if the
    // file is missing or corrupted
    QString m_GameResolution{"1920x1080"};
    bool    m_FullScreen{false};
    QColor  m_Player1Color{QColor(173, 216, 230)};
    QColor  m_Player2Color{QColor(255, 105, 97)};
    int     m_MasterVolume{20};
    int     m_MusicVolume{20};
    int     m_SfxVolume{20};

    void DrawMenu();
    void DrawNewGame();
    void DrawResumeGame();
    void DrawOptions();

    // Helper Functions
    void         CreateLabel(const QString &text, QWidget *parent);
    void         CreateSlider(int &value, QWidget *parent);
    void         CreateColorPicker(const QString &labelText, QColor &color, QWidget *parent);
    QVBoxLayout *CreateLimitedLayout(QWidget *widget);

    std::string m_CurrentGameMode;
};

#endif // MAINWINDOW_H
