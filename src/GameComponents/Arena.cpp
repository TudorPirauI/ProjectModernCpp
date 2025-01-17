//
// Created by Miruna on 1/17/2025.
//

#include "../../include/GameComponents/Arena.h"

Arena::Arena(const int size) :
    Board(size), m_Size(size), m_Board(size, std::vector<PlayerPiece>(size, PlayerPiece::None)) {}

void Arena::PlacePiece(const int x, const int y, const PlayerPiece &player) {
    if (x >= 0 && x < m_Size && y >= 0 && y < m_Size) {
        m_Board[x][y] = player;
    }
}

// todo: use getleft, getright, getdown, getup to use the actual board corners
bool Arena::CheckWin(const PlayerPiece &player) const {
    for (int i = 0; i < m_Size; ++i) {
        if (std::ranges::all_of(m_Board[i],
                                [player](const PlayerPiece &p) { return p == player; })) {
            return true;
        }
        if (std::ranges::all_of(m_Board, [i, player](const std::vector<PlayerPiece> &row) {
                return row[i] == player;
            })) {
            return true;
        }
    }
    if (std::ranges::all_of(m_Board, [n = 0, player](const std::vector<PlayerPiece> &row) mutable {
            return row[n++] == player;
        })) {
        return true;
    }
    if (std::ranges::all_of(m_Board,
                            [n = m_Size - 1, player](const std::vector<PlayerPiece> &row) mutable {
                                return row[n--] == player;
                            })) {
        return true;
    }
    return false;
}

int Arena::CountPieces(const PlayerPiece &player) const {
    int count = 0;
    for (const auto &row : m_Board) {
        count += std::count(row.begin(), row.end(), player);
    }
    return count;
}
