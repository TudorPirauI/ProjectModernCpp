//
// Created by Tudor on 1/9/2025.
//

#ifndef IANTRENAMENT_H
#define IANTRENAMENT_H

#include "GameComponents/Card.h"
#include "GameModes/Antrenament.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"

class IAntrenament final : public QWidget {
    Q_OBJECT

public:
    IAntrenament(const std::string &nameOne, const std::string &nameTwo, QWidget *parent);

public slots:
    void OnCardSelected(int cardValue);
    void OnPositionSelected(int x, int y);

signals:
    void GameFinished();

private:
    void SwitchTurn();

    Antrenament         m_CurrentGame;
    PlayerTurn          m_CurrentTurn;
    std::optional<Card> m_SelectedCard;

    QWidget     *m_ParentWidget;
    BoardWidget *m_BoardWidget;
    HandWidget  *m_HandWidget;
};

#endif // IANTRENAMENT_H
