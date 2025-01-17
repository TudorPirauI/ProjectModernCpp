//
// Created by Miruna on 1/17/2025.
//

#include "../../include/GameComponents/Arena.h"

Arena::Arena(int size) :
    Board(size), size_(size), board_(size, std::vector<Player>(size, Player::None)) {}

void Arena::PlacePiece(int x, int y, Player player) {
    if (x >= 0 && x < size_ && y >= 0 && y < size_) {
        if (board_[x][y] == Player::None) {
            board_[x][y] = player;
        } else if (board_[x][y] != player) {
            board_[x][y] = player;
        }
    }
}

bool Arena::CheckWin(Player player) const {
    for (int i = 0; i < size_; ++i) {
        if (std::all_of(board_[i].begin(), board_[i].end(),
                        [player](Player p) { return p == player; })) {
            return true;
        }
        if (std::all_of(board_.begin(), board_.end(),
                        [i, player](const std::vector<Player> &row) { return row[i] == player; })) {
            return true;
        }
    }
    if (std::all_of(board_.begin(), board_.end(),
                    [n = 0, player](const std::vector<Player> &row) mutable {
                        return row[n++] == player;
                    })) {
        return true;
    }
    if (std::all_of(board_.begin(), board_.end(),
                    [n = size_ - 1, player](const std::vector<Player> &row) mutable {
                        return row[n--] == player;
                    })) {
        return true;
    }
    return false;
}

int Arena::CountPieces(Player player) const {
    int count = 0;
    for (const auto &row : board_) {
        count += std::count(row.begin(), row.end(), player);
    }
    return count;
}
