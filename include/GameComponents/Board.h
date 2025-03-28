#ifndef BOARD_H
#define BOARD_H

#include "GameComponents/Card.h"

class Game;
using GameBoard               = std::map<Position, std::stack<Card>>;
constexpr auto START_POSITION = Position{0, 0};

enum class CardType { Normal, Eter, Illusion, Hole, Granite };
enum class InsertOutputs {
    Success,
    PositionInvalid,
    IllusionOccupied,
    EterOccupied,
    GraniteOccupied
};

class Board {
    GameBoard                    m_Board;
    int                          m_MaxBoardSize;
    std::unordered_map<int, int> m_Lines;
    std::unordered_map<int, int> m_Columns;
    std::unordered_map<int, int> m_PrincipalDiagonal;
    std::unordered_map<int, int> m_SecondaryDiagonal;
    bool                         m_IsLocked{false};
    bool                         m_TwoLines{false};
    bool                         m_TwoColumns{false};
    bool                         m_Cross{false};

    std::array<Position, 4> m_Corners{START_POSITION, START_POSITION, START_POSITION,
                                      START_POSITION};

    [[nodiscard]] bool CheckProximity(const Position &pos) const;

    bool CheckPlacedCard(const Position &pos, const PlayerTurn &playerTurn);

public:
    explicit Board(int maxBoardSize);

    [[nodiscard]] InsertOutputs InsertCard(Card card, Position pos, const PlayerTurn &playerTurn,
                                           const CardType &cardType, Game &currentGame,
                                           bool forceInsert = false);
    [[nodiscard]] bool          InsertIllusion(Card &card, const Position &pos);
    [[nodiscard]] bool          InsertEter(Card &card, const Position &pos);

    [[nodiscard]] bool CoverIllusion(const Card &cardOpponent, const Position &pos);
    bool               CheckLineWin(GameBoard &board, const Position &position);
    bool               CheckColumnWin(GameBoard &board, const Position &position);
    bool               CheckDiagonalWin(GameBoard &board, const Position &position);
    [[nodiscard]] bool IsBoardLocked() const;
    [[nodiscard]] int  GetMaxBoardSize() const;
    [[nodiscard]] bool IsPositionValid(const Position &pos, const Card &card) const;

    [[nodiscard]] GameBoard &GetGameBoard();

    [[nodiscard]] std::unordered_map<int, int> &GetLineAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetColumnAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetPrincipalDiagonalAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetSecondaryDiagonalAdvantage();

    [[nodiscard]] const Position &GetLeft() const;
    [[nodiscard]] const Position &GetRight() const;
    [[nodiscard]] const Position &GetUp() const;
    [[nodiscard]] const Position &GetDown() const;

    void SetLeft(const Position &position);
    void SetRight(const Position &position);
    void SetUp(const Position &position);
    void SetDown(const Position &position);

    bool UpdateCorners(const Position &pos);

    void CheckIsLocked();

    [[nodiscard]] bool IsWithinBorderRestrictions(const Position &position) const;

    bool UpdateDiagonals();

    void CleanUpBoard();

    void CheckTwoLinesFull(const Position &position, const PlayerTurn &playerTurn);

    void CheckTwoColumns(const Position &position, const PlayerTurn &playerTurn);

    void CheckCross(const Position &position, const PlayerTurn &playerTurn);

    void SetTwoLines(bool value);
    void SetTwoColumns(bool value);
    void SetCross(bool value);

    bool GetTwoLines() const;
    bool GetTwoColumns() const;
    bool GetCross() const;
    void SetGameBoard(const GameBoard &gameBoard);
};

#endif // BOARD_H
