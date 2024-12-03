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

#include <fstream>

bool Game::CheckWinningConditions() {
    const auto &board                   = m_Board.GetGameBoard();
    const auto &[left, up, down, right] = m_Board.GetCorners();

    std::ofstream outFile("game_log.txt", std::ios_base::app);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file";
        return false;
    }

    // const auto firstResult  = abs(left.first - right.first) == m_ScoreToWin;
    const auto secondResult = abs(up.second - down.second) == m_ScoreToWin;

    const auto targetValue = std::abs(m_Board.GetMaxBoardSize());

    if (std::ranges::any_of(m_Lines | std::views::values,
                            [&](const auto &value) { return abs(value) == targetValue; })) {
        outFile << "Winning condition met: Line advantage\n";
        outFile.close();
        return true;
    }

    if (std::ranges::any_of(m_Columns | std::views::values,
                            [&](const auto &value) { return abs(value) == targetValue; })) {
        outFile << "Winning condition met: Column advantage\n";
        outFile.close();
        return true;
    }

    if (m_Board.IsBoardLocked() == false) {
        outFile << "No winning condition met: Board is not locked\n";
        outFile.close();
        return false;
    }

    bool notFound = true;

    for (int i = left.first, j = up.second; i <= right.first && j <= down.second; ++i, ++j) {
        const auto it = board.find({i, j});
        if (it == board.end() || it->second.top().GetPlacedBy() != m_PlayerTurn) {
            notFound = false;
            break;
        }
    }

    if (notFound == true) {
        outFile << "Winning condition met: Diagonal from top-left to bottom-right\n";
        outFile.close();
        return true;
    }

    notFound = true;
    for (int i = left.first, j = down.second; i <= right.first && j >= up.second; ++i, --j) {
        const auto it = board.find({i, j});

        if (it == board.end() || it->second.top().GetPlacedBy() != m_PlayerTurn) {
            notFound = false;
            break;
        }
    }
    if (notFound == true && secondResult) {
        outFile << "Winning condition met: Diagonal from bottom-left to top-right\n";
        outFile.close();
        return true;
    }

    outFile << "No winning condition met\n";
    outFile.close();
    return false;
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

std::unordered_map<int, int> &Game::GetLineAdvantage() { return m_Lines; }

std::unordered_map<int, int> &Game::GetColumnAdvantage() { return m_Columns; }

int Game::GetPlayer1Score() const { return m_ScorePlayer1; }

int Game::GetPlayer2Score() const { return m_ScorePlayer2; }

int Game::GetScoreToWin() const { return m_ScoreToWin; }
