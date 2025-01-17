//
// Created by Tudor on 1/17/2025.
//

#ifndef ETER_IDUELULELEMENTELOR_H
#define ETER_IDUELULELEMENTELOR_H

#include "GameComponents/Card.h"
#include "GameModes/Antrenament.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"

class IDuelulElementelor final : public QWidget {
    Q_OBJECT

public:
    IDuelulElementelor(const std::string &nameOne, const std::string &nameTwo, QWidget *parent);

public slots:
    void OnCardSelected(int cardValue);
    void OnPositionSelected(int x, int y);

private:
    void SwitchTurn();

    Antrenament         m_CurrentGame;
    PlayerTurn          m_CurrentPlayer;
    std::optional<Card> m_SelectedCard;

    QWidget     *m_ParentWidget;
    BoardWidget *m_BoardWidget;
    HandWidget  *m_HandWidget;
};

#endif // ETER_IDUELULELEMENTELOR_H
