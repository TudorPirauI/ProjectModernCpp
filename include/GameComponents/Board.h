#ifndef BOARD_H
#define BOARD_H

#include <GameComponents/Card.h>
#include <Powers/Wizard.h>
#include <array>
#include <map>
#include <stack>
#include <unordered_map>

using GameBoard               = std::map<Position, std::stack<Card>>;
constexpr auto START_POSITION = Position{0, 0};

enum class CardType { Normal, Eter, Ilusion };

class Board {
    GameBoard                    m_Board;
    int                          m_MaxBoardSize;
    std::unordered_map<int, int> m_Lines;
    std::unordered_map<int, int> m_Columns;
    std::unordered_map<int, int> m_PrincipalDiagonal;
    std::unordered_map<int, int> m_SecondaryDiagonal;
    bool                         m_IsLocked{false};

    std::array<Position, 4> m_Corners{START_POSITION, START_POSITION, START_POSITION,
                                      START_POSITION};

    [[nodiscard]] bool CheckProximity(const Position &pos) const;
    [[nodiscard]] bool IsBoardFull() const;

    bool UpdateCorners(const Position &pos);
    void CheckIsLocked();
    bool CheckPlacedCard(const Position &pos, const PlayerTurn &playerTurn);

public:
    explicit Board(int maxBoardSize);

    [[nodiscard]] bool InsertCard(Card &card, const Position &pos, const PlayerTurn &playerTurn,
                                  const CardType &cardType = CardType::Normal);
    [[nodiscard]] bool InsertIllusion(Card &card, const Position &pos);
    [[nodiscard]] bool InsertEter(Card &card, const Position &pos);

    [[nodiscard]] bool CoverIllusion(const Card &cardOpponent, const Position &pos);
    [[nodiscard]] bool IsBoardLocked() const;
    [[nodiscard]] int  GetMaxBoardSize() const;
    [[nodiscard]] bool IsPositionValid(const Position &pos, const Card &card) const;

    [[nodiscard]] GameBoard GetGameBoard() const;

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
    void UpdateDiagonals();

    void CleanUpBoard();

    bool VerifyWizardPower(const WizardPower &power, const Position &position,
                           const PlayerTurn &playerTurn);
};

#endif // BOARD_H
