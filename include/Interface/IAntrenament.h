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
                 const std::array<bool, 3> &options, QWidget *parent, int rapid = 0);

    IAntrenament(Antrenament &other, QWidget *parent);

public slots:
    void OnCardSelected(int cardIndex);
    void OnPositionSelected(int x, int y);
    void OnModifierSelected(int modifier);
    void OnExplosion();
    void ShowHintPopup();
    void OnTurnTimeExpired(); // New slot for handling turn time expiration

signals:
    void GameFinished();

private:
    void SwitchTurn();
    void StartTurnTimer();
    void StopTurnTimer();

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
    QTimer *m_TurnTimer; // New QTimer for turn time limit
    int     m_Rapid; // Time limit for each turn in seconds
};
#endif // IANTRENAMENT_H
