//
// Created by Miruna on 1/17/2025.
//

#ifndef ARENA_H
#define ARENA_H

#include "Board.h"

class Arena : public Board {
public:
    enum class Player { None, Player1, Player2 };

    explicit Arena(int size);

    void PlacePiece(int x, int y, Player player);
    bool CheckWin(Player player) const;
    int  CountPieces(Player player) const;

private:
    int                              size_;
    std::vector<std::vector<Player>> board_;
};

#endif // ARENA_H
