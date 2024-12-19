#ifndef ANTRENAMENTWIDGET_H
#define ANTRENAMENTWIDGET_H
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "../Game/Antrenament.h"

class AntrenamentWidget final : public QWidget {
    Q_OBJECT

    QWidget        *m_AntrenamentWidget;
    QVBoxLayout    *m_Layout;
    QStackedWidget *m_StackedWidget;
    QWidget        *m_Parent;

    std::unique_ptr<Antrenament> m_CurrentGame;
    std::optional<Card>          m_SelectedCard;

    int     m_GamesPlayed  = 0;
    int     m_Player1Score = 0;
    int     m_Player2Score = 0;
    QLabel *m_ScoreLabel   = nullptr;

    QGridLayout *GenerateBoard(const Board                         &board,
                               const std::function<void(Position)> &cellClickedCallback);
    QHBoxLayout *GenerateHand(const Hand                      &hand,
                              const std::function<void(Card)> &cellClickedCallback);

    void ShowWinningMessage(const QString &winnerName);
    void UpdateScoreLabel(const std::string &nameOne, const std::string &nameTwo) const;
    void DrawGame();

public:
    AntrenamentWidget(const std::string &nameOne, const std::string &nameTwo,
                      QStackedWidget *stackedWidget, QWidget *parent = nullptr);
};

#endif // ANTRENAMENTWIDGET_H
