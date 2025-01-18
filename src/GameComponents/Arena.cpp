//
// Created by Miruna on 1/17/2025.
//

#include "../../include/GameComponents/Arena.h"
#include "../../include/GameComponents/Board.h"

Arena::Arena(const int size) :
    Board(size), m_Size(size), m_Board(size, std::vector(size, PlayerPiece::None)) {}

void Arena::PlacePiece(const int x, const int y, const PlayerPiece &player) {
    if (m_Board[x][y] != PlayerPiece::None && m_Board[x][y] != player) {
        m_Board[x][y] = player;
    } else {
        m_Board[x][y] = player;
    }
}

// todo: use getleft, getright, getdown, getup to use the actual board corners
bool Arena::CheckWin(const PlayerPiece &player) const {
    const Position &left  = GetLeft();
    const Position &right = GetRight();
    const Position &up    = GetUp();
    const Position &down  = GetDown();

    for (int i = left.second; i <= right.second; ++i) {
        if (std::ranges::all_of(m_Board[i],
                                [player](const PlayerPiece &p) { return p == player; })) {
            return true;
        }
    }
    for (int j = up.first; j <= down.first; ++j) {
        if (std::ranges::all_of(m_Board, [j, player](const std::vector<PlayerPiece> &row) {
                return row[j] == player;
            })) {
            return true;
        }
    }

    if (std::ranges::all_of(m_Board,
                            [n = left.second, player](const std::vector<PlayerPiece> &row) mutable {
                                return row[n++] == player;
                            })) {
        return true;
    }
    if (std::ranges::all_of(
                m_Board, [n = right.second, player](const std::vector<PlayerPiece> &row) mutable {
                    return row[n--] == player;
                })) {
        return true;
    }
    return false;
}

int Arena::CountPieces(const PlayerPiece &player) const {
    int count = 0;
    for (const auto &row : m_Board) {
        count += std::ranges::count(row, player);
    }
    return count;
}
