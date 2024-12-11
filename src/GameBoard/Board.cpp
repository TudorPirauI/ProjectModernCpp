#include "Board.h"

#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>

#include "../MagicPower/ElementPower.h"
#include "../MagicPower/Wizard.h"
#include "../Player/Player.h"

bool Board::IsPositionValid(const Position &pos, const Card &card) const {
    const auto cardOnPosition = m_Board.find(pos);
    // todo: Un jucător nu poate să își acopere propria iluzie
    if (cardOnPosition != m_Board.end()) {
        const auto cardOnTop = cardOnPosition->second.top();
        if (cardOnTop.GetValue() >= card.GetValue()) {
            std::cout << "Can't place card on top of a card with a larger value\n ";
            return false;
        }

        if (cardOnTop.GetIsEter() == true) {
            std::cout << "Can't place on top of Eter card\n";
            return false;
        }

        if (cardOnTop.GetIsFlipped()) {
            if (cardOnTop.GetValue() >= card.GetValue()) {
                std::cout << "Can't place card on top of a card with a larger value\n ";
                return false;
            }
        }
    }

    if (m_IsLocked) {
        // const auto &left  = m_Corners[0];
        // const auto &right = m_Corners[1];
        // const auto &down  = m_Corners[2];
        // const auto &up    = m_Corners[3];

        const auto left  = GetLeft();
        const auto right = GetRight();
        const auto down  = GetDown();
        const auto up    = GetUp();

        if (pos.first < left.first || pos.second < up.second || pos.second > down.second ||
            pos.first > right.first) {
            std::cout << "[Locked] Card out of bounds\n";
            return false;
        }

        return true;
    }

    if (!CheckProximity(pos)) {
        std::cout << "Card not adjacent to any other card\n";
        return false;
    }

    const auto &left  = m_Corners[0];
    const auto &right = m_Corners[1];
    const auto &down  = m_Corners[2];
    const auto &up    = m_Corners[3];

    if (std::abs(left.first - pos.first) >= m_MaxBoardSize ||
        std::abs(right.first - pos.first) >= m_MaxBoardSize ||
        std::abs(up.second - pos.second) >= m_MaxBoardSize ||
        std::abs(down.second - pos.second) >= m_MaxBoardSize) {
        // std::cout << "Card out of bounds\n";
        return false;
    }

    return true;
}

bool Board::CheckProximity(const Position &pos) const {
    if (m_Board.empty()) {
        return true;
    }

    return std::ranges::any_of(m_Board | std::views::keys, [&](const auto &positionFromTable) {
        const auto xDiff = std::abs(pos.first - positionFromTable.first);
        const auto yDiff = std::abs(positionFromTable.second - pos.second);

        return xDiff <= 1 and yDiff <= 1;
    });
}

bool Board::UpdateCorners(const Position &pos) {
    bool wasUpdated = false;

    if (pos.first < m_Corners[0].first) {
        m_Corners[0] = pos;
        wasUpdated   = true;
    }
    if (pos.first > m_Corners[1].first) {
        m_Corners[1] = pos;
        wasUpdated   = true;
    }
    if (pos.second < m_Corners[2].second) {
        m_Corners[2] = pos;
        wasUpdated   = true;
    }
    if (pos.second > m_Corners[3].second) {
        m_Corners[3] = pos;
        wasUpdated   = true;
    }

    return wasUpdated;
}

void Board::CheckIsLocked() {
    if (m_IsLocked) {
        return;
    }

    const auto &left  = GetLeft();
    const auto &right = GetRight();
    const auto &down  = GetDown();
    const auto &up    = GetUp();

    if (std::abs(left.first - right.first) == m_MaxBoardSize - 1 &&
        std::abs(up.second - down.second) == m_MaxBoardSize - 1) {
        m_IsLocked = true;
        std::cout << "Board is locked\n";
    }
}

bool Board::CheckPlacedCard(const Position &pos, const PlayerTurn playerTurn) {
    const auto &it = m_Board.find(pos);

    if (it == m_Board.end())
        return true;

    return m_Board[pos].top().GetPlacedBy() == playerTurn;
}

void Board::CleanUpBoard() {
    m_Board.clear();
    m_Corners[0] = m_Corners[1] = m_Corners[2] = m_Corners[3] = std::make_pair(0, 0);
    m_IsLocked                                                = false;
    m_Lines.clear();
    m_Columns.clear();
    m_PrincipalDiagonal = 0;
    m_SecondaryDiagonal = 0;
}

Position Board::GetLeft() const { return m_Corners[0]; }
Position Board::GetRight() const { return m_Corners[1]; }
Position Board::GetUp() const { return m_Corners[2]; }
Position Board::GetDown() const { return m_Corners[3]; }

void Board::SetLeft(const Position &position) { m_Corners[0] = position; }
void Board::SetRight(const Position &position) { m_Corners[1] = position; }
void Board::SetUp(const Position &position) { m_Corners[2] = position; }
void Board::SetDown(const Position &position) { m_Corners[3] = position; }

void Board::UpdateDiagonals() {
    m_PrincipalDiagonal = m_SecondaryDiagonal = 0;

    const auto &left  = GetLeft();
    const auto &right = GetRight();
    const auto &down  = GetDown();
    const auto &up    = GetUp();

    auto isOnPrincipalDiagonal = [&](const Position &pos) {
        return pos.first - pos.second == left.second - up.first;
    };

    auto isOnSecondaryDiagonal = [&](const Position &pos) {
        return pos.first + pos.second == left.second + down.first;
    };

    auto updateDiagonal = [&](auto &diagonal, auto condition, const PlayerTurn turn) {
        for (const auto &position : m_Board | std::views::keys) {
            if (condition(position)) {
                if (CheckPlacedCard(position, PlayerTurn::Player1) == false) {
                    diagonal += (2);
                } else {
                    diagonal += (1);
                }
                if (CheckPlacedCard(position, PlayerTurn::Player2) == false) {
                    diagonal -= (2);
                } else {
                    diagonal -= (1);
                }
            }
        }
    };

    updateDiagonal(m_PrincipalDiagonal, isOnPrincipalDiagonal, PlayerTurn::Player1);
    updateDiagonal(m_SecondaryDiagonal, isOnSecondaryDiagonal, PlayerTurn::Player2);
}

bool Board::VerifyWizardPower(const WizardPower power, const Position &position,
                              const PlayerTurn playerTurn) {
    switch (power) {
        case WizardPower::EliminateOpponentCard: {
            if (auto &stack = m_Board[position]; stack.size() >= 2) {
                stack.pop();
                return true;
            }

            return false;
        }
        case WizardPower::RemoveRow: {
        }
        // todo: give a opinion about this
        case WizardPower::CoverOpponentCard: {
            if (const auto &stack = m_Board[position]; !stack.empty()) {
                return true;
            }

            return false;
        }
        case WizardPower::CreatePit: {
            if (auto &stack = m_Board[position]; stack.empty() == true) {
                stack.emplace(true);
                return true;
            }

            return false;
        }
        // todo: frontend :)
        case WizardPower::MoveOwnStack: {
            return false;
        }
        case WizardPower::GainEterCard: {
            return true;
        }
        // todo: frontend :)
        case WizardPower::MoveOpponentStack: {
            return false;
        }
        case WizardPower::ShiftRowToEdge: {
            const auto &left  = GetLeft();
            const auto &right = GetRight();
            const auto &up    = GetUp();
            const auto &down  = GetDown();

            if (position.first == up.first) {
                bool ok = true;
                for (auto i = left.second; i <= right.second and ok; ++i) {
                    if (m_Board[{up.first, i}].empty())
                        ok = false;
                }

                if (ok == false)
                    return false;

                return true;
            }
            if (position.first == down.first) {
                bool ok = true;
                for (auto i = left.second; i <= right.second and ok; ++i) {
                    if (m_Board[{up.first, i}].empty())
                        ok = false;
                }

                if (ok == false)
                    return false;

                return true;
            }
            return false;
        }
        default:;
    }

    return false;
}
bool Board::VerifyElementalPower(const ElementIndexPower &power, const Position &position,
                                 const PlayerTurn &playerTurn) {
    switch (power) {
        case ElementIndexPower::ControlledExplosion: {
            // To do
        }
        case ElementIndexPower::Destruction: {
            // to do
        }
        case ElementIndexPower::Flames: {
            // to do
        }
        case ElementIndexPower::Lava: {
            // to do
        }
        case ElementIndexPower::FromAshes: {
            // to do
        }
        case ElementIndexPower::Sparks: {
            // to do
        }
        case ElementIndexPower::Blizzard: {
            // to do
        }
        case ElementIndexPower::Gale: {
            // to do
        }
        case ElementIndexPower::Hurricane: {
            // to do
        }
        case ElementIndexPower::Gust: {
            // to do
        }
        case ElementIndexPower::Mirage: {
            // to do
        }
        case ElementIndexPower::Storm: {
            // to do
        }
        case ElementIndexPower::Tide: {
            // to do
        }
        case ElementIndexPower::Fog: {
            // to do
        }
        case ElementIndexPower::Wave: {
            // to do
        }
        case ElementIndexPower::Whirlpool: {
            // to do
        }
        case ElementIndexPower::Tsunami: {
            // to do
        }
        case ElementIndexPower::Waterfall: {
            // to do
        }
        case ElementIndexPower::Support: {
            // to do
        }
        case ElementIndexPower::Earthquake: {
            // to do
        }
        case ElementIndexPower::Shattering: {
            // to do
        }
        case ElementIndexPower::Granite: {
            // to do
        }
        case ElementIndexPower::Avalanche: {
            // to do
        }
        case ElementIndexPower::Boulder: {
            // to do
        }
        default:;
    }
    return false;
}

int Board::GetMaxBoardSize() const { return m_MaxBoardSize; }

GameBoard Board::GetGameBoard() const { return m_Board; }

Board::Board(const int maxBoardSize) :
    m_MaxBoardSize(maxBoardSize), m_Lines({}), m_Columns({}), m_PrincipalDiagonal({}),
    m_SecondaryDiagonal({}) {}

bool Board::IsBoardLocked() const { return m_IsLocked; }

bool Board::IsBoardFull() const {
    if (m_Board.size() != m_MaxBoardSize * m_MaxBoardSize) {
        std::cout << "There are still positions open on the board\n";
        return false;
    }

    std::cout << "Board is full\n";

    return true; // todo: check if cards can be placed on top of other cards.
}

bool Board::InsertCard(const Card &card, const Position &pos, const PlayerTurn playerTurn) {
    if (!IsPositionValid(pos, card)) {
        std::cout << "Invalid position\n";
        return false;
    }

    int playerVariation;

    if (playerTurn != PlayerTurn::Player1) {
        playerVariation = -1;
    } else {
        playerVariation = 1;
    }

    if (CheckPlacedCard(pos, playerTurn) == false) {
        m_Lines[pos.first] += (2 * playerVariation);
        m_Columns[pos.second] += (2 * playerVariation);
    } else {
        m_Lines[pos.first] += (1 * playerVariation);
        m_Columns[pos.second] += (1 * playerVariation);
    }

    m_Board[pos].push(card);

    if (UpdateCorners(pos) == false) {
        UpdateDiagonals();

        // const auto &left  = GetLeft();
        // const auto &right = GetRight();
        // const auto &down  = GetDown();
        // const auto &up    = GetUp();

        // if (pos.first - pos.second == left.first - up.second) {
        //     if (CheckPlacedCard(pos, playerTurn) == false) {
        //         m_PrincipalDiagonal += (2 * playerVariation);
        //
        //     } else {
        //         m_PrincipalDiagonal += (1 * playerVariation);
        //     }
        // }
        // if (pos.first + pos.second == left.first + down.second) {
        //     if (CheckPlacedCard(pos, playerTurn) == false) {
        //         m_SecondaryDiagonal += (2 * playerVariation);
        //
        //     } else {
        //         m_SecondaryDiagonal += (1 * playerVariation);
        //     }
        // }
    }

    CheckIsLocked();

    return true;
}

bool Board::InsertIllusion(Card &card, const Position &pos) {
    if (m_Board[pos].empty() == false) {
        std::cout << "Can't place an illusion on an occupied place\n";
        return false;
    }
    card.SetIllusion(true);
    return true;
}

std::unordered_map<int, int> &Board::GetLineAdvantage() { return m_Lines; }

std::unordered_map<int, int> &Board::GetColumnAdvantage() { return m_Columns; }

int &Board::GetPrincipalDiagonalAdvantage() { return m_PrincipalDiagonal; }

int &Board::GetSecondaryDiagonalAdvantage() { return m_SecondaryDiagonal; }

bool Board::CoverIllusion(const Card &cardOpponent, const Position &pos) {
    m_Board[pos].top().SetIsFlipped(true);
    if (cardOpponent.GetValue() <= m_Board[pos].top().GetValue()) {
        std::cout << "The card of the player who has the illusion has a "
                     "greater value\n";
        return false;
    }
    std::cout << "The card of the player who tried to cover the illusion has a "
                 "greater value\n";
    m_Board[pos].pop();
    m_Board[pos].emplace(cardOpponent);
    return true;
}
