//
// Created by Tudor on 1/9/2025.
//

#ifndef IANTRENAMENT_H
#define IANTRENAMENT_H

#include "GameComponents/Card.h"
#include "GameModes/Antrenament.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"

#include <pch.h>

class IAntrenament final : public QObject {
    Q_OBJECT

public:
    IAntrenament(const std::string &nameOne, const std::string &nameTwo, QWidget *parent);
    void StartGame();

public slots:
    void OnCardSelected(const Card &card);
    void OnCardPlaced(int row, int col);

private:
    void SwitchTurn();

    Antrenament         m_CurrentGame;
    PlayerTurn          m_CurrentPlayer;
    std::optional<Card> m_SelectedCard;
    QWidget            *m_ParentWidget;
    BoardWidget        *m_BoardWidget;
    HandWidget         *m_HandWidget;
};

#endif // IANTRENAMENT_H
