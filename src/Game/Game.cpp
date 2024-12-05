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
    // const auto &board                   = m_Board.GetGameBoard();
    // const auto &[left, up, down, right] = m_Board.GetCorners();

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
    }

    if (m_Board.IsBoardLocked() == false) {
        std::cout << "Board not locked\n";
        return false;
    }

    std::cout << "Principal diagonal\n";
    for (const auto [fst, snd] : principalDiagonal) {
        std::cout << fst << " " << snd << '\n';
    }

    std::cout << "Secondary diagonal\n";
    for (const auto [fst, snd] : secondaryDiagonal) {
        std::cout << fst << " " << snd << '\n';
    }

    return hasWinning(principalDiagonal) or hasWinning(secondaryDiagonal);
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
