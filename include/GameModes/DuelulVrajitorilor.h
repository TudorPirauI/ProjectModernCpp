//
// Created by Tudor on 11/28/2024.
//

#ifndef DUELULVRAJITORILOR_H
#define DUELULVRAJITORILOR_H

#include "GameModes/Game.h"
#include "Powers/Wizard.h"

class DuelulVrajitorilor : public Game {
    int    m_ScoreWinning{2};
    Wizard m_AbilityPlayer1;
    Wizard m_AbilityPlayer2;

public:
    Wizard GetPlayerAbility1();
    Wizard GetPlayerAbility2();
    DuelulVrajitorilor(const DuelulVrajitorilor &other)                = default;
    DuelulVrajitorilor(DuelulVrajitorilor &&other) noexcept            = default;
    DuelulVrajitorilor &operator=(const DuelulVrajitorilor &other)     = default;
    DuelulVrajitorilor &operator=(DuelulVrajitorilor &&other) noexcept = default;

    DuelulVrajitorilor();
    ~DuelulVrajitorilor() override = default;
};

#endif // DUELULVRAJITORILOR_H
