#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <array>
#include <map>
#include <stack>

#include "../Card/Card.h"
#include "../Player/Player.h"

using GameBoard                  = std::map<Position, std::stack<Card>>;
constexpr Position StartPosition = Position{0, 0};

class Board {
    GameBoard                    m_Board;
    int                          m_MaxBoardSize;
    std::unordered_map<int, int> m_Lines;
    std::unordered_map<int, int> m_Columns;
    std::unordered_map<int, int> m_PrincipalDiagonal;
    std::unordered_map<int, int> m_SecondaryDiagonal;
    bool                         m_IsLocked{false};

    std::array<Position, 4> m_Corners{StartPosition, StartPosition, StartPosition, StartPosition};

    [[nodiscard]] bool CheckProximity(const Position &pos) const;
    [[nodiscard]] bool IsBoardFull() const;

    bool UpdateCorners(const Position &pos);
    void CheckIsLocked();
    bool CheckPlacedCard(const Position &pos, PlayerTurn playerTurn);

public:
    explicit Board(int maxBoardSize);
    [[nodiscard]] bool InsertCard(Card &card, const Position &pos, PlayerTurn playerTurn);
    [[nodiscard]] bool IsBoardLocked() const;
    [[nodiscard]] int  GetMaxBoardSize() const;
    [[nodiscard]] std::array<Position, 4> GetCorners() const;
    [[nodiscard]] GameBoard               GetGameBoard() const;
    [[nodiscard]] bool IsPositionValid(const Position &pos, const Card &card) const;
    bool               InsertIllusion(Card &card, const Position &pos);
    [[nodiscard]] std::unordered_map<int, int> &GetLineAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetColumnAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetPrincipalDiagonalAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetSecondaryDiagonalAdvantage();

    bool CoverIllusion(const Card &cardOpponent, const Position &pos);
    void CleanUpBoard();
    void UpdateDiagonals(PlayerTurn playerTurn);
};

#endif // BOARD_H
