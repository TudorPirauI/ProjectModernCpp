//
// Created by edi on 28.11.2024.
//

#include "Game.h"

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne, const std::string &nameTwo) :
    m_Board(boardSize), m_ScoreToWin(scoreToWin), m_Player1(nameOne, {}), m_Player2(nameTwo, {}) {}

bool Game::CheckWinningConditions() {
    for (const auto &[line, value]: m_Lines) {
        if (value == abs(m_Board.GetMaxBoardSize())) {
            return true;
        }
    }

    for (const auto &[column, value]: m_Columns) {
        if (value == abs(m_Board.GetMaxBoardSize())) {
            return true;
        }
    }

    // todo: sa facem sa mearga pentru 4x4

    // todo: sa facem sa mearga pentru 3x3
}
