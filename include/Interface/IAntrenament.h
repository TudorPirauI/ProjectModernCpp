//
// Created by Tudor on 1/9/2025.
//

#ifndef ETER_IANTRENAMENT_H
#define ETER_IANTRENAMENT_H

#include <QObject>
#include "GameModes/Antrenament.h"
#include "Interface/DisplayHand.h"
#include "Interface/DisplayBoard.h"

class IAntrenament final : public QObject {
Q_OBJECT

public:
    explicit IAntrenament(QWidget *parent = nullptr);
    void StartGame();

signals:
    void GameFinished(PlayerTurn winner);

private slots:
    void OnCardSelected(const Card &card);
    void OnCardPlaced();

private:
    Antrenament m_CurrentGame;
    DisplayHand *m_DisplayHand;
    DisplayBoard *m_DisplayBoard;
    PlayerTurn m_CurrentPlayer;
    Card m_SelectedCard;
    QWidget *m_ParentWidget;
};

#endif // ETER_IANTRENAMENT_H