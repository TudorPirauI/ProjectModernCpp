#include "Board.h"

#include <iostream>

bool Board::isPositionValid(const Position &pos) const {
    if (m_IsLocked) {
        auto &[left, up, down, right] = m_Corners;

        if (pos.first < left.first || pos.second < up.second || pos.second > down.second || pos.first > right.first) {
            std::cout << "[Locked] Card out of bounds\n";
            return false;
        }

        return true;
    }

    if (!checkProximity(pos)) {
        std::cout << "Card not adjacent to any other card\n";
        return false;
    }

    auto &[left, up, down, right] = m_Corners;

    if (std::abs(left.first - pos.first) >= m_MaxBoardSize || std::abs(right.first - pos.first) >= m_MaxBoardSize ||
        std::abs(up.second - pos.second) >= m_MaxBoardSize || std::abs(down.second - pos.second) >= m_MaxBoardSize) {
        std::cout << "Card out of bounds\n";
        return false;
    }

    return true;
}

bool Board::checkProximity(const Position &pos) const {
    if (m_Board.empty()) {
        return true;
    }

    for (const auto &[position, stack]: m_Board) {
        const auto xDiff = std::abs(position.first - pos.first);
        const auto yDiff = std::abs(position.second - pos.second);

        if (xDiff <= 1 && yDiff <= 1) {
            std::cout << "Card is adjacent to another card\n";
            return true;
        }

        std::cout << +xDiff << " | " << +yDiff << '\n';
    }

    return false;
}

void Board::updateCorners(const Position &pos) {
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

void Board::checkIsLocked() {
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
void Board::printTable() const {
    const auto &[left, up, down, right] = m_Corners;

    // for (const auto &[key, value]: m_Board) {
    //     std::cout << "Key: " << key.first << ' ' << key.second << '\n';
    //     std::cout << "Value: " << value.top().GetValue() << '\n';
    // }


    // std::cout << "Printing table\n";
    // std::cout << "Up.y: " << +up.second << " Down.y: " << +down.second << '\n';
    // std::cout << "Left.x: " << +left.first << " Right.x: " << +right.first << '\n';

    for (int j = up.second; j <= down.second; ++j) {
        for (int i = left.first; i <= right.first; ++i) {
            // std::cout << "Printing " << +i << ' ' << +j << '\n';
            const auto it = m_Board.find({i, j});

            if (it != m_Board.end()) {
                std::cout << +it->second.top().GetValue() << ' ';
            } else {
                std::cout << "X ";
            }
        }

        std::cout << '\n';
    }


    // std::cout << "Left: " << +left.first << ' ' << +left.second << '\n';
    // std::cout << "Up: " << +up.first << ' ' << +up.second << '\n';
    // std::cout << "Down: " << +down.first << ' ' << +down.second << '\n';
    // std::cout << "Right: " << +right.first << ' ' << +right.second << '\n';
}

bool Board::insertCard(const Card &card, const Position &pos) {
    if (!isPositionValid(pos)) {
        std::cout << "Invalid position\n";
        return false;
    }

    m_Board[pos].push(card);

    updateCorners(pos);

    checkIsLocked();

    return true;
}
