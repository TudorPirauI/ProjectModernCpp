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

    // todo: revizuit
    const auto &board                   = m_Board.GetGameBoard();
    const auto &[left, up, down, right] = m_Board.GetCorners();
    for (int j = up.second, i = left.first; i <= right.first && j <= down.second; ++i, ++j) {
        const auto it = board.find({i, j});
        if (it == board.end()) {
            break;
        }
        if (it->second.top().GetPlacedBy() != m_PlayerTurn) {
            break;
        }
    }
    for (int j = down.second, i = left.first; i >= right.first && j <= up.second; --i, ++j) {
        const auto it = board.find({i, j});
        if (it == board.end()) {
            break;
        }
        if (it->second.top().GetPlacedBy() != m_PlayerTurn) {
            break;
        }
    }
}
