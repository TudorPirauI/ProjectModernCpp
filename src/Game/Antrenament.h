//
// Created by edi on 21.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H
#include "../GameBoard/Board.h"
#include "../Player/Player.h"


class Antrenament {
    Board  m_Board;
    Player m_Player1;
    Player m_Player2;
    int    m_ScorePlayer1{0};
    int    m_ScorePlayer2{0};
    int    m_ScoreWinning;

    int  CheckWinningConditions();
    bool NoCardLeft();
    bool WinningOnDiagonal();
    bool WinningOnLine();
    bool WinningOnVertical();
};


#endif // ANTRENAMENT_H
