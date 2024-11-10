//
// Created by mavri on 08.11.2024.
//

#include "GameBoard.h"

#include <queue>
#include <unordered_set>
void GameBoard::growBoardIfNeeded() {
    const auto    rows    = m_GameBoard.size();
    const auto    cols    = m_GameBoard[0].size();
    constexpr int maxSize = 4; // Change depending on game mode

    bool expandTop = false, expandBottom = false, expandLeft = false, expandRight = false;

    // Check if we need to expand at the top
    for (int j = 0; j < cols; ++j) {
        if (m_GameBoard[0][j].getTopCard()) {
            expandTop = true;
            break;
        }
    }

    // Check if we need to expand at the bottom
    for (int j = 0; j < cols; ++j) {
        if (m_GameBoard[rows - 1][j].getTopCard()) {
            expandBottom = true;
            break;
        }
    }

    // Check if we need to expand to the left
    for (int i = 0; i < rows; ++i) {
        if (m_GameBoard[i][0].getTopCard()) {
            expandLeft = true;
            break;
        }
    }

    // Check if we need to expand to the right
    for (int i = 0; i < rows; ++i) {
        if (m_GameBoard[i][cols - 1].getTopCard()) {
            expandRight = true;
            break;
        }
    }

    // Expand as necessary without exceeding the maximum size
    if (expandTop && rows < maxSize) {
        m_GameBoard.insert(m_GameBoard.begin(), std::vector<std::optional<CardStack>>(cols));
    }
    if (expandBottom && rows < maxSize) {
        m_GameBoard.push_back(std::vector<std::optional<CardStack>>(cols));
    }
    if (expandLeft && cols < maxSize) {
        for (auto &row: m_GameBoard) {
            row.insert(row.begin(), std::nullopt);
        }
    }
    if (expandRight && cols < maxSize) {
        for (auto &row: m_GameBoard) {
            row.push_back(std::nullopt);
        }
    }
}
void GameBoard::placeCard(const Card &card, u_int8_t row, u_int8_t column) {}

struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const {
        // Combine the hash values of the two elements of the pair
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};

bool GameBoard::checkIsolation() const {
    // Get dimensions of the board
    const auto rows = m_GameBoard.size();
    const auto cols = m_GameBoard[0].size();

    // Track all positions that have cards
    std::vector<std::pair<int, int>> cardPositions;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (m_GameBoard[i][j].getTopCard()) { // if there is a card at this position
                cardPositions.emplace_back(i, j);
            }
        }
    }

    // Early return if no cards are placed
    if (cardPositions.empty())
        return false;

    // Perform BFS/DFS from the first card position to check connectivity
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::queue<std::pair<int, int>>                    toVisit;
    toVisit.push(cardPositions[0]);
    visited.insert(cardPositions[0]);

    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // N, S, W, E

    while (!toVisit.empty()) {
        auto [x, y] = toVisit.front();
        toVisit.pop();

        // Check all 4 adjacent cells
        for (const auto &[dx, dy]: directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && m_GameBoard[nx][ny].getTopCard() &&
                visited.insert({nx, ny}).second) {
                toVisit.emplace(nx, ny);
            }
        }
    }

    // If all card positions are reachable, no isolation; otherwise, there is isolation
    return visited.size() != cardPositions.size();
}
