//
// Created by Tudor on 11/28/2024.
//

#ifndef DUELULVRAJITORILOR_H
#define DUELULVRAJITORILOR_H
#include <unordered_map>

#include "../GameBoard/Board.h"
#include "../MagicPower/Wizard.h"
#include "../Player/Player.h"
#include "Game.h"


class DuelulVrajitorilor : public Game {
    enum class PlayerTurn { Player1, Player2 };

private:
    int    m_ScoreWinning{2};
    Wizard m_WizardPlayer1;
    Wizard m_WizardPlayer2;

public:
    DuelulVrajitorilor();
    ~DuelulVrajitorilor() override;
};


#endif // DUELULVRAJITORILOR_H
