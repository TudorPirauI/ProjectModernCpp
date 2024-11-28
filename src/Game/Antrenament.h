//
// Created by edi on 21.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H
#include <unordered_map>


#include "../GameBoard/Board.h"
#include "../Player/Player.h"


class Antrenament {
public:
    enum class GameState { NotFinished, NormalWin, FinishedCards };
    enum class PlayerTurn { Player1, Player2 };

private:
    Board      m_Board;
    Player     m_Player1;
    Player     m_Player2;
    PlayerTurn m_PlayerTurn{PlayerTurn::Player1};
    GameState  m_GameState{GameState::NotFinished};
    int        m_ScorePlayer1{0};
    int        m_ScorePlayer2{0};
    int        m_ScoreWinning{3};

    // Board: addCard to the board
    // Antrenament: Add constructor to initialise players, board and current Player
    void CheckWinningConditions(); // to be rechecked after the implementation
    bool NoCardLeft();
    bool WinningOnDiagonal(); // to be implemented
    bool WinningOnLine(); // to be implemented
    bool WinningOnVertical(); // to be implemented
    void GameLoop(); // to be implemented
};


#endif // ANTRENAMENT_H
