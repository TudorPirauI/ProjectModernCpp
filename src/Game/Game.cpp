//
// Created by edi on 28.11.2024.
//

#include "Game.h"

#include <ranges>

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne,
           const std::string &nameTwo) :
    m_Board(boardSize), m_Player1(nameOne, {}), m_Player2(nameTwo, {}), m_ScoreToWin(scoreToWin) {}

PlayerTurn Game::GetCurrentPlayer() const { return m_PlayerTurn; }

bool Game::CheckWinningConditions() {
    const auto targetValue = std::abs(m_Board.GetMaxBoardSize());

    if (std::ranges::any_of(m_Lines | std::views::values,
                            [&](const auto &value) { return value == targetValue; })) {
        return true;
    }

    if (std::ranges::any_of(m_Columns | std::views::values,
                            [&](const auto &value) { return value == targetValue; })) {
        return true;
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
