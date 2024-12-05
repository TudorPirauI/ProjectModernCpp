#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "Board.h"

#include <fstream>
#include <iostream>
#include <ranges>

#include "../Player/Player.h"

bool Board::IsPositionValid(const Position &pos, const Card &card) const {
    const auto cardOnPosition = m_Board.find(pos);
    // todo: Un jucător nu poate să își acopere propria iluzie
    if (cardOnPosition != m_Board.end()) {
        const auto cardOnTop = cardOnPosition->second.top();
        if (cardOnTop.GetValue() >= card.GetValue()) {
            // std::cout << "Can't place card on top of a card with a larger
            // value\n";
            return false;
        }

        if (cardOnTop.GetIsEter() == true) {
            // std::cout << "Can't place on top of Eter card\n";
            return false;
        }

        if (cardOnTop.GetIsFlipped()) {
            if (cardOnTop.GetValue() >= card.GetValue()) {
                // std::cout << "Can't place card on top of a card with a larger
                // value\n";
                return false;
            }
        }
    }

    if (m_IsLocked) {
        const auto &left  = GetLeft();
        const auto &right = GetRight();
        const auto &down  = GetDown();
        const auto &up    = GetUp();

        if (pos.first < left.first || pos.second < up.second || pos.second > down.second ||
            pos.first > right.first) {
            // std::cout << "[Locked] Card out of bounds\n";
            return false;
        }

        return true;
    }

    if (!CheckProximity(pos)) {
        // std::cout << "Card not adjacent to any other card\n";
        return false;
    }

    const auto &left  = GetLeft();
    const auto &right = GetRight();
    const auto &down  = GetDown();
    const auto &up    = GetUp();

    if (std::abs(left.first - pos.first) >= m_MaxBoardSize ||
        std::abs(right.first - pos.first) >= m_MaxBoardSize ||
        std::abs(up.second - pos.second) >= m_MaxBoardSize ||
        std::abs(down.second - pos.second) >= m_MaxBoardSize) {
        // std::cout << "Card out of bounds\n";
        return false;
    }

    return true;
}

bool Board::CheckProximity(const Position &pos) const {
    if (m_Board.empty()) {
        return true;
    }

    return std::ranges::any_of(m_Board | std::views::keys, [&](const auto &position) {
        const auto xDiff = std::abs(pos.first - position.first);
        const auto yDiff = std::abs(position.second - pos.second);

        return xDiff <= 1 and yDiff <= 1;
    });
}

bool Board::UpdateCorners(const Position &pos) {
    bool        wasUpdated = false;
    const auto &left       = GetLeft();
    const auto &right      = GetRight();
    const auto &down       = GetDown();
    const auto &up         = GetUp();

    if (pos.first < left.first) {
        SetLeft(pos);
        wasUpdated = true;
    }
    if (pos.second < up.second) {
        SetUp(pos);
        wasUpdated = true;
    }
    if (pos.second > down.second) {
        SetDown(pos);
        wasUpdated = true;
    }
    if (pos.first > right.first) {
        SetRight(pos);
        wasUpdated = true;
    }

    return wasUpdated;
}

void Board::CheckIsLocked() {
    if (m_IsLocked) {
        return;
    }

    const auto &left  = GetLeft();
    const auto &right = GetRight();
    const auto &down  = GetDown();
    const auto &up    = GetUp();

    if (std::abs(left.first - right.first) == m_MaxBoardSize - 1 &&
        std::abs(up.second - down.second) == m_MaxBoardSize - 1) {
        m_IsLocked = true;
        std::cout << "Board is locked\n";
    }
}

bool Board::CheckPlacedCard(const Position &pos, const PlayerTurn playerTurn) {
    const auto &it = m_Board.find(pos);

    if (it == m_Board.end())
        return true;

    return m_Board[pos].top().GetPlacedBy() == playerTurn;
}

void Board::CleanUpBoard() {
    m_Board.clear();
    m_Corners[0] = m_Corners[1] = m_Corners[2] = m_Corners[3] = std::make_pair(0, 0);
    m_IsLocked                                                = false;
    m_Lines.clear();
    m_Columns.clear();
}
Position Board::GetLeft() const { return m_Corners[0]; }
Position Board::GetRight() const { return m_Corners[1]; }
Position Board::GetUp() const { return m_Corners[2]; }
Position Board::GetDown() const { return m_Corners[3]; }

void Board::SetLeft(const Position &position) { m_Corners[0] = position; }
void Board::SetRight(const Position &position) { m_Corners[1] = position; }
void Board::SetUp(const Position &position) { m_Corners[2] = position; }
void Board::SetDown(const Position &position) { m_Corners[3] = position; }

void Board::UpdateDiagonals(PlayerTurn playerTurn) {
    m_PrincipalDiagonal.clear();
    m_SecondaryDiagonal.clear();

    const auto &left  = GetLeft();
    const auto &right = GetRight();
    const auto &down  = GetDown();
    const auto &up    = GetUp();

    static int currentPlayer;
    currentPlayer = (playerTurn == PlayerTurn::Player1) ? 1 : -1;

    auto updateDiagonal = [&](auto &diagonal, auto condition) {
        for (const auto &position : m_Board | std::views::keys) {
            if (condition(position)) {
                if (CheckPlacedCard(position, playerTurn) == false) {
                    diagonal[position.first] += (2 * currentPlayer);
                } else {
                    diagonal[position.first] += (1 * currentPlayer);
                }
            }
        }
    };

    auto isOnPrincipalDiagonal = [&](const Position &pos) {
        return pos.first - pos.second == left.second - up.first;
    };

    auto isOnSecondaryDiagonal = [&](const Position &pos) {
        return pos.first + pos.second == left.second + down.first;
    };

    updateDiagonal(m_PrincipalDiagonal, isOnPrincipalDiagonal);
    updateDiagonal(m_SecondaryDiagonal, isOnSecondaryDiagonal);
}

int Board::GetMaxBoardSize() const { return m_MaxBoardSize; }

std::array<Position, 4> Board::GetCorners() const { return m_Corners; }
GameBoard               Board::GetGameBoard() const { return m_Board; }

Board::Board(const int maxBoardSize) : m_MaxBoardSize(maxBoardSize), m_Lines({}), m_Columns({}) {}

bool Board::IsBoardLocked() const { return m_IsLocked; }

bool Board::IsBoardFull() const {
    if (m_Board.size() != m_MaxBoardSize * m_MaxBoardSize) {
        std::cout << "There are still positions open on the board\n";
        return false;
    }

    std::cout << "Board is full\n";

    return true; // todo: check if cards can be placed on top of other cards.
}

bool Board::InsertCard(Card &card, const Position &pos, const PlayerTurn playerTurn) {
    if (!IsPositionValid(pos, card)) {
        std::cout << "Invalid position\n";
        return false;
    }

    static int playerVariation;

    if (playerTurn != PlayerTurn::Player1) {
        playerVariation = -1;
    } else {
        playerVariation = 1;
    }

    if (CheckPlacedCard(pos, playerTurn) == false) {
        m_Lines[pos.first] += (2 * playerVariation);
        m_Columns[pos.second] += (2 * playerVariation);
    } else {
        m_Lines[pos.first] += (1 * playerVariation);
        m_Columns[pos.second] += (1 * playerVariation);
    }

    m_Board[pos].push(card);

    if (UpdateCorners(pos))
        UpdateDiagonals(playerTurn);
    else {
        const auto &left  = GetLeft();
        const auto &right = GetRight();
        const auto &down  = GetDown();
        const auto &up    = GetUp();
        if (pos.first - pos.second == left.first - up.second) {
            if (CheckPlacedCard(pos, playerTurn) == false) {
                m_PrincipalDiagonal[pos.first] += (2 * playerVariation);
            } else {
                m_PrincipalDiagonal[pos.first] += (1 * playerVariation);
            }
        }
        if (pos.first + pos.second == left.first + down.second) {
            if (CheckPlacedCard(pos, playerTurn) == false) {
                m_SecondaryDiagonal[pos.first] += (2 * playerVariation);
            } else {
                m_SecondaryDiagonal[pos.first] += (1 * playerVariation);
            }
        }
    }

    CheckIsLocked();

    return true;
}

bool Board::InsertIllusion(Card &card, const Position &pos) {
    if (m_Board[pos].empty() == false) {
        std::cout << "Can't place an illusion on an occupied place\n";
        return false;
    }
    card.SetIllusion(true);
    return true;
}

std::unordered_map<int, int> &Board::GetLineAdvantage() { return m_Lines; }

std::unordered_map<int, int> &Board::GetColumnAdvantage() { return m_Columns; }

std::unordered_map<int, int> &Board::GetPrincipalDiagonalAdvantage() { return m_PrincipalDiagonal; }

std::unordered_map<int, int> &Board::GetSecondaryDiagonalAdvantage() { return m_SecondaryDiagonal; }

bool Board::CoverIllusion(const Card &cardOpponent, const Position &pos) {
    m_Board[pos].top().SetIsFlipped(true);
    if (cardOpponent.GetValue() <= m_Board[pos].top().GetValue()) {
        std::cout << "The card of the player who has the illusion has a "
                     "greater value\n";
        return false;
    }
    std::cout << "The card of the player who tried to cover the illusion has a "
                 "greater value\n";
    m_Board[pos].pop();
    m_Board[pos].emplace(cardOpponent);
    return true;
}
