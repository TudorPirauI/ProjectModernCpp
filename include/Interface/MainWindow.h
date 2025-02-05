#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void AddBackButton(const QWidget *widget);

    QColor GetPlayer1Color() const;
    void   SetPlayer1Color(const QColor &color);

    QColor GetPlayer2Color() const;
    void   SetPlayer2Color(const QColor &color);

    static QColor m_Player1Color;
    static QColor m_Player2Color;

private slots:
    void OnResolutionChanged(const QString &resolution);
    void OnNewGameClicked();
    void OnResumeGameClicked();
    void OnOptionsClicked();
    void OnExitClicked();
    void OnBackClicked();
    void OnGameFinished();

private:
    enum class GameState { MainMenu, NewGame, InGame, ResumeGame, Options, Exit };

    GameState       m_CurrentState;
    QStackedWidget *m_StackedWidget;
    
    QString m_GameResolution{"2560x1440"};
    bool    m_FullScreen{true};
    int     m_MasterVolume{100};
    int     m_MusicVolume{100};
    int     m_SfxVolume{100};

    QMediaPlayer *m_MediaPlayer;
    QAudioOutput *m_AudioOutput;

    void DrawMenu();
    void DrawNewGame();
    void DrawResumeGame();
    void DrawOptions();

    // Helper Functions
    static void         CreateLabel(const QString &text, QWidget *parent);
    void                CreateSlider(int &value, QWidget *parent);
    void                CreateColorPicker(const QString &labelText, QColor &color, QWidget *parent);
    static QVBoxLayout *CreateLimitedLayout(QWidget *widget);
    QListWidget        *m_GameListWidget;

    std::string m_CurrentGameMode;
};

#endif // MAINWINDOW_H
