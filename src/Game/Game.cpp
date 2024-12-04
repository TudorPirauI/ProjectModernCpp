//
// Created by edi on 28.11.2024.
//

#include "Game.h"

#include <iostream>
#include <ranges>

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne,
           const std::string &nameTwo) :
    m_Board(boardSize), m_Player1(nameOne, {}), m_Player2(nameTwo, {}), m_ScoreToWin(scoreToWin) {}

PlayerTurn Game::GetCurrentPlayer() const { return m_PlayerTurn; }

Game::Game() : m_Board(Board(0)), m_Player1(Player("", {})), m_Player2(Player("", {})) {}

bool Game::CheckWinningConditions(const PlayerTurn currentPlayerTurn) {
    const auto &board                   = m_Board.GetGameBoard();
    const auto &[left, up, down, right] = m_Board.GetCorners();

    const auto &lines   = m_Board.GetLineAdvantage();
    const auto &columns = m_Board.GetColumnAdvantage();

    const auto targetValue = m_Board.GetMaxBoardSize();

    if (std::ranges::any_of(lines | std::views::values,
                            [&](const auto &value) { return abs(value) == targetValue; })) {

        return true;
    }

    if (std::ranges::any_of(columns | std::views::values,
                            [&](const auto &value) { return abs(value) == targetValue; })) {
        return true;
    }

    if (m_Board.IsBoardLocked() == false) {
        return false;
    }

    bool notFound = true;

    for (int i = up.first, j = left.second; i <= down.first && j <= right.second; ++i, ++j) {
        const auto it = board.find({i, j});
        if (it == board.end() || it->second.top().GetPlacedBy() != currentPlayerTurn) {
            notFound = false;
            break;
        }
    }

    if (notFound == true) {
        return true;
    }

    notFound = true;

    for (int i = up.first, j = right.second; i <= down.first && j >= left.second; ++i, --j) {
        const auto it = board.find({i, j});

        if (it == board.end() || it->second.top().GetPlacedBy() != currentPlayerTurn) {
            notFound = false;
            break;
        }
    }

    return notFound;
}

void Game::SetGameState(const GameState gameState) { m_GameState = gameState; }

void Game::IncreasePlayerScore(const PlayerTurn turn) {
    if (turn == PlayerTurn::Player1) {
        ++m_ScorePlayer1;
    } else {
        ++m_ScorePlayer2;
    }
}

void Game::SetNextPlayerTurn(const PlayerTurn playerTurn) { m_PlayerTurn = playerTurn; }

Board &Game::GetBoard() { return m_Board; }

Player &Game::GetPlayer1() { return m_Player1; }

Player &Game::GetPlayer2() { return m_Player2; }

int Game::GetPlayer1Score() const { return m_ScorePlayer1; }

int Game::GetPlayer2Score() const { return m_ScorePlayer2; }

int Game::GetScoreToWin() const { return m_ScoreToWin; }
