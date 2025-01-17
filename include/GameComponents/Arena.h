//
// Created by Miruna on 1/17/2025.
//

#ifndef ARENA_H
#define ARENA_H

#include "Board.h"

enum class PlayerPiece { None, Player1, Player2 };
using MatrixPieces = std::vector<std::vector<PlayerPiece>>;

class Arena : public Board {
public:
    explicit Arena(int size);

    void               PlacePiece(int x, int y, const PlayerPiece &player);
    [[nodiscard]] bool CheckWin(const PlayerPiece &player) const;
    [[nodiscard]] int  CountPieces(const PlayerPiece &player) const;

private:
    int          m_Size;
    MatrixPieces m_Board;
};

#endif // ARENA_H
