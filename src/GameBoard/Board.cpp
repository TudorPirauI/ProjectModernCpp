#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "ftxui-grid-container/grid-container.hpp"

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
        const auto &[left, up, down, right] = m_Corners;

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

    auto &[left, up, down, right] = m_Corners;

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
        const auto xDiff = std::abs(position.first - pos.first);
        const auto yDiff = std::abs(position.second - pos.second);

        return xDiff <= 1 && yDiff <= 1;
    });
}

void Board::UpdateCorners(const Position &pos) {
    auto &[left, up, down, right] = m_Corners;

    if (pos.first < left.first) {
        left = pos;
    }
    if (pos.second < up.second) {
        up = pos;
    }
    if (pos.second > down.second) {
        down = pos;
    }
    if (pos.first > right.first) {
        right = pos;
    }

    /*
    std::cout << "Left: " << +left.first << ' ' << +left.second << '\n';
    std::cout << "Up: " << +up.first << ' ' << +up.second << '\n';
    std::cout << "Down: " << +down.first << ' ' << +down.second << '\n';
    std::cout << "Right: " << +right.first << ' ' << +right.second << '\n';
    */
}

void Board::CheckIsLocked() {
    if (m_IsLocked) {
        return;
    }

    auto &[left, up, down, right] = m_Corners;

    if (std::abs(left.first - right.first) == m_MaxBoardSize - 1 &&
        std::abs(up.second - down.second) == m_MaxBoardSize - 1) {
        m_IsLocked = true;
        std::cout << "Board is locked\n";
    }
}

int Board::GetMaxBoardSize() const { return m_MaxBoardSize; }

std::array<Position, 4> Board::GetCorners() const { return m_Corners; }
GameBoard               Board::GetGameBoard() const { return m_Board; }

Board::Board(const int maxBoardSize) : m_MaxBoardSize(maxBoardSize) {}

bool Board::IsBoardLocked() const { return m_IsLocked; }

bool Board::IsBoardFull() const {
    if (m_Board.size() != m_MaxBoardSize * m_MaxBoardSize) {
        std::cout << "There are still positions open on the board\n";
        return false;
    }

    std::cout << "Board is full\n";

    return false;
}

bool Board::InsertCard(const Card &card, const Position &pos) {
    if (!IsPositionValid(pos, card)) {
        std::cout << "Invalid position\n";
        return false;
    }

    m_Board[pos].push(card);

    UpdateCorners(pos);

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
