//
// Created by Tudor on 1/9/2025.
//

#ifndef IANTRENAMENT_H
#define IANTRENAMENT_H

#include "GameComponents/Card.h"
#include "GameModes/Antrenament.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"
#include "SpecialOptions.h"

class IAntrenament final : public QWidget {
    Q_OBJECT

public:
    IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                 const std::array<bool, 3> &options, QWidget *parent, bool rapid = false);

    IAntrenament(Antrenament &other, QWidget *parent);

public slots:
    void OnCardSelected(int cardIndex);
    void OnPositionSelected(int x, int y);
    void OnModifierSelected(int modifier);
    void OnExplosion();
    void ShowHintPopup();

signals:
    void GameFinished();

private:
    void SwitchTurn();

    Antrenament         m_CurrentGame;
    Explosion           m_CurrentExplosion;
    std::optional<Card> m_SelectedCard;

    bool m_IsIllusionSelected{false};
    bool m_IsRapid{false};

    QWidget        *m_ParentWidget;
    BoardWidget    *m_BoardWidget;
    HandWidget     *m_HandWidget;
    SpecialOptions *m_SpecialOptions;

    QTimer *m_InactivityTimer;
};

#endif // IANTRENAMENT_H
