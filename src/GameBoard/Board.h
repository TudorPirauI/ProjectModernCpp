#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <array>
#include <map>
#include <stack>

#include "../Card/Card.h"

using GameBoard = std::map<Position, std::stack<Card>>;

class Board {
    GameBoard m_Board;
    int       m_MaxBoardSize{3};
    bool      m_IsLocked{false};

    std::array<Position, 4> m_Corners{
            Position{0, 0}, Position{0, 0}, Position{0, 0},
            Position{0, 0}}; // abuz de limbaj ca nu sunt sigur ce reprezinta acestea (mersi chat gepele de clarificare)

    [[nodiscard]] bool IsPositionValid(const Position &pos, const Card &card) const;
    [[nodiscard]] bool CheckProximity(const Position &pos) const;
    [[nodiscard]] bool IsBoardFull() const;

    void UpdateCorners(const Position &pos);
    void CheckIsLocked();


public:
    [[nodiscard]] bool InsertCard(const Card &card, const Position &pos);
    [[nodiscard]] bool IsBoardLocked() const;
    void               PrintTable() const;
};


#endif // BOARD_H
