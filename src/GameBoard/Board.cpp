#include "Board.h"

#include <fstream>
#include <iostream>
#include <ranges>

// TODO: inca nu se foloseste [posX, posY] peste tot, sunt niste locuri unde nu cred ca isi au
// rostul.

bool Board::IsPositionValid(const Position &pos, const Card &card) const {
    if (const auto cardOnPosition = m_Board.find(pos); cardOnPosition != m_Board.end()) {
        const auto cardOnTop = cardOnPosition->second.top();
        if (cardOnTop.GetValue() >= card.GetValue()) {
            return false;
        }

        if (cardOnTop.GetIsEter() == true) {
            return false;
        }

        if (cardOnTop.GetIsFlipped()) {
            if (cardOnTop.GetValue() >= card.GetValue()) {
                return false;
            }
        }
    }

    const auto [posX, posY] = pos;

    const auto [leftX, leftY]   = GetLeft();
    const auto [rightX, rightY] = GetRight();
    const auto [downX, downY]   = GetDown();
    const auto [upX, upY]       = GetUp();

    if (m_IsLocked) {
        if (posX < leftX || posY < upY || posY > downY || posX > rightX) {
            return false;
        }

        return true;
    }

    if (!CheckProximity(pos)) {
        return false;
    }

    if (std::abs(leftX - posX) >= m_MaxBoardSize || std::abs(rightX - posX) >= m_MaxBoardSize ||
        std::abs(upY - posY) >= m_MaxBoardSize || std::abs(downY - posY) >= m_MaxBoardSize) {
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

        return xDiff <= 1 && yDiff <= 1;
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

    const auto &[leftX, leftY]   = GetLeft();
    const auto &[rightX, rightY] = GetRight();
    const auto &[downX, downY]   = GetDown();
    const auto &[upX, upY]       = GetUp();

    if (std::abs(leftX - rightX) == m_MaxBoardSize - 1 &&
        std::abs(upY - downY) == m_MaxBoardSize - 1) {
        m_IsLocked = true;
        std::cout << "Board is locked\n";
    }
}

bool Board::CheckPlacedCard(const Position &pos, const PlayerTurn &playerTurn) {
    return m_Board[pos].empty() || m_Board[pos].top().GetPlacedBy() == playerTurn;
}

void Board::CleanUpBoard() {
    m_Board.clear();
    m_Corners[0] = m_Corners[1] = m_Corners[2] = m_Corners[3] = std::make_pair(0, 0);
    m_IsLocked                                                = false;
    m_Lines.clear();
    m_Columns.clear();
}

const Position &Board::GetLeft() const { return m_Corners[0]; }
const Position &Board::GetRight() const { return m_Corners[1]; }
const Position &Board::GetUp() const { return m_Corners[2]; }
const Position &Board::GetDown() const { return m_Corners[3]; }

void Board::SetLeft(const Position &position) { m_Corners[0] = position; }
void Board::SetRight(const Position &position) { m_Corners[1] = position; }
void Board::SetUp(const Position &position) { m_Corners[2] = position; }
void Board::SetDown(const Position &position) { m_Corners[3] = position; }

void Board::UpdateDiagonals(const PlayerTurn &playerTurn) {
    m_PrincipalDiagonal.clear();
    m_SecondaryDiagonal.clear();

    const auto &[leftX, leftY] = GetLeft();
    // const auto &right          = GetRight(); // this is unused, again?
    const auto &[downX, downY] = GetDown();
    const auto &[upX, upY]     = GetUp();

    // there used to be a static here, that's what caused the weird winning conditions.
    const auto currentPlayer = (playerTurn == PlayerTurn::Player1) ? 1 : -1;

    auto updateDiagonal = [&](auto &diagonal, auto condition) {
        for (const auto &position : m_Board | std::views::keys) {
            if (!condition(position))
                continue;

            if (CheckPlacedCard(position, playerTurn) == false) {
                diagonal[position.first] += (2 * currentPlayer);
            } else {
                diagonal[position.first] += (1 * currentPlayer);
            }
        }
    };

    const auto isOnPrincipalDiagonal = [&](const Position &pos) {
        return pos.first - pos.second == leftY - upX;
    };

    const auto isOnSecondaryDiagonal = [&](const Position &pos) {
        return pos.first + pos.second == leftY + downX;
    };

    updateDiagonal(m_PrincipalDiagonal, isOnPrincipalDiagonal);
    updateDiagonal(m_SecondaryDiagonal, isOnSecondaryDiagonal);
}

bool Board::VerifyWizardPower(const WizardPower &power, const Position &position,
                              const PlayerTurn &playerTurn) {
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
            const auto &[leftX, leftY]   = GetLeft();
            const auto &[rightX, rightY] = GetRight();
            const auto &[upX, upY]       = GetUp();
            const auto &[downX, downY]   = GetDown();

            if (position.first == upX) {
                bool ok = true;
                for (auto i = leftY; i <= rightY; ++i) {
                    if (m_Board[{upX, i}].empty()) {
                        ok = false;
                        break;
                    }
                }

                return ok;
            }

            if (position.first == downX) {
                bool ok = true;
                for (auto i = leftY; i <= rightY; ++i) {
                    if (m_Board[{upX, i}].empty()) {
                        ok = false;
                        break;
                    }
                }

                return ok;
            }
            return false;
        }
        default: {
            return false;
        }
    }
}

int Board::GetMaxBoardSize() const { return m_MaxBoardSize; }

GameBoard Board::GetGameBoard() const { return m_Board; }

Board::Board(const int maxBoardSize) :
    m_MaxBoardSize(maxBoardSize), m_Lines({}), m_Columns({}), m_PrincipalDiagonal({}),
    m_SecondaryDiagonal({}) {}

bool Board::IsBoardLocked() const { return m_IsLocked; }

bool Board::InsertCard(const Card &card, const Position &pos, const PlayerTurn &playerTurn) {
    if (!IsPositionValid(pos, card)) {
        std::cout << "Invalid position\n";
        return false;
    }

    int compensateForPlacingOnTop = 1;
    int playerVariation           = -1;

    if (playerTurn == PlayerTurn::Player1) {
        playerVariation = 1;
    }

    if (CheckPlacedCard(pos, playerTurn) == false) {
        compensateForPlacingOnTop = 2;
    }

    m_Lines[pos.first] += compensateForPlacingOnTop * playerVariation;
    m_Columns[pos.second] += compensateForPlacingOnTop * playerVariation;

    m_Board[pos].push(card);

    if (UpdateCorners(pos)) {
        UpdateDiagonals(playerTurn);
    } else {
        const auto &left  = GetLeft();
        const auto &right = GetRight(); // for some reason this is unused?
        const auto &down  = GetDown();
        const auto &up    = GetUp();

        if (pos.first - pos.second == left.first - up.second) {
            m_PrincipalDiagonal[pos.first] += compensateForPlacingOnTop * playerVariation;
        }
        if (pos.first + pos.second == left.first + down.second) {
            m_SecondaryDiagonal[pos.first] += compensateForPlacingOnTop * playerVariation;
        }
    }

    CheckIsLocked();

    return true;
}

bool Board::InsertIllusion(Card &card, const Position &pos) {
    if (!m_Board[pos].empty()) {
        std::cout << "Can't place an illusion on an occupied place\n";
        return false;
    }

    card.SetIllusion(true);

    // todo: this function is useless. just merge it with InsertCard (also with PlaceEter if it
    // exists)
    return true;
}

std::unordered_map<int, int> &Board::GetLineAdvantage() { return m_Lines; }

std::unordered_map<int, int> &Board::GetColumnAdvantage() { return m_Columns; }

std::unordered_map<int, int> &Board::GetPrincipalDiagonalAdvantage() { return m_PrincipalDiagonal; }

std::unordered_map<int, int> &Board::GetSecondaryDiagonalAdvantage() { return m_SecondaryDiagonal; }

bool Board::CoverIllusion(const Card &cardOpponent, const Position &pos) {
    // todo: this should also be merged with InsertCard imo
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
