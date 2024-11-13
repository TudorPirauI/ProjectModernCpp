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

    [[nodiscard]] bool isPositionValid(const Position &pos) const;
    [[nodiscard]] bool checkProximity(const Position &pos) const;
    void               updateCorners(const Position &pos);
    void               checkIsLocked();

public:
    [[nodiscard]] bool insertCard(const Card &card, const Position &pos);
    void               printTable() const;
};


#endif // BOARD_H
