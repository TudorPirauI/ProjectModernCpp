//
// Created by edi on 28.11.2024.
//

#include "Game.h"

#include <iostream>
#include <numeric>
#include <ranges>

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne,
           const std::string &nameTwo) :
    m_Board(boardSize), m_Player1(nameOne, {}), m_Player2(nameTwo, {}), m_ScoreToWin(scoreToWin) {}

PlayerTurn Game::GetCurrentPlayer() const { return m_PlayerTurn; }

Game::Game() : m_Board(Board(0)), m_Player1(Player("", {})), m_Player2(Player("", {})) {}

bool Game::CheckWinningConditions() {
    const auto &lines             = m_Board.GetLineAdvantage();
    const auto &columns           = m_Board.GetColumnAdvantage();
    const auto &principalDiagonal = m_Board.GetPrincipalDiagonalAdvantage();
    const auto &secondaryDiagonal = m_Board.GetSecondaryDiagonalAdvantage();

    const auto targetValue = m_Board.GetMaxBoardSize();

    auto hasWinning = [&](const auto &data) {
        return std::ranges::any_of(data | std::views::values,
                                   [&](const auto &value) { return abs(value) == targetValue; });
    };

    if (hasWinning(lines) or hasWinning(columns)) {
        std::cout << "Lines or columns\n";
        return true;
    }

    if (m_Board.IsBoardLocked() == false) {
        std::cout << "Board not locked\n";
        return false;
    }

    // std::cout << "Principal diagonal\n";
    // for (const auto [fst, snd] : principalDiagonal) {
    //     std::cout << fst << " " << snd << '\n';
    // }
    //
    // std::cout << "Secondary diagonal\n";
    // for (const auto [fst, snd] : secondaryDiagonal) {
    //     std::cout << fst << " " << snd << '\n';
    // }

    // todo: this is wrong, it wins with 2 cards on the diagonal from 2 players
    auto sumValues = [&](const auto &data) {
        auto values = data | std::views::values;
        return std::accumulate(values.begin(), values.end(), 0,
                               [](int sum, const auto &value) { return sum + abs(value); });
    };

    return sumValues(principalDiagonal) or sumValues(secondaryDiagonal);
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

void Game::SwitchPlayerTurn() {
    if (m_PlayerTurn == PlayerTurn::Player1)
        m_PlayerTurn = PlayerTurn::Player2;
    else
        m_PlayerTurn = PlayerTurn::Player1;
}

int Game::GetPlayer1Score() const { return m_ScorePlayer1; }

int Game::GetPlayer2Score() const { return m_ScorePlayer2; }

int Game::GetScoreToWin() const { return m_ScoreToWin; }
