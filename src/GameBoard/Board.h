#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <array>
#include <map>
#include <stack>

#include "../Card/Card.h"
#include "../Player/Player.h"

using GameBoard = std::map<Position, std::stack<Card>>;

class Board {
    GameBoard m_Board;
    int       m_MaxBoardSize;
    bool      m_IsLocked{false};

    std::array<Position, 4> m_Corners{
            Position{0, 0}, Position{0, 0}, Position{0, 0},
            Position{0, 0}}; // abuz de limbaj ca nu sunt sigur ce reprezinta acestea (mersi chat
                             // gepele de clarificare)

    [[nodiscard]] bool CheckProximity(const Position &pos) const;
    [[nodiscard]] bool IsBoardFull() const;

    void UpdateCorners(const Position &pos);
    void CheckIsLocked();

public:
    explicit Board(int maxBoardSize);
    [[nodiscard]] bool     InsertCard(const Card &card, const Position &pos, PlayerTurn playerTurn);
    [[nodiscard]] bool     IsBoardLocked() const;
    [[nodiscard]] Position ShowTableWithInput() const;
    [[nodiscard]] int      GetMaxBoardSize() const;
    [[nodiscard]] std::array<Position, 4> GetCorners() const;
    [[nodiscard]] GameBoard               GetGameBoard() const;
    [[nodiscard]] bool IsPositionValid(const Position &pos, const Card &card) const;
    bool               InsertIllusion(Card &card, const Position &pos);

    bool CoverIllusion(const Card &cardOpponent, const Position &pos);
    void CleanUpBoard();
};

#endif // BOARD_H
