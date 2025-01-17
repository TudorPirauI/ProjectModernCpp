#include "GameComponents/Board.h"
#include "GameComponents/Player.h"
#include "GameModes/Game.h"

bool Board::IsPositionValid(const Position &pos, const Card &card) const {
    if (const auto cardOnPosition = m_Board.find(pos);
        card.GetIsGranite() or cardOnPosition != m_Board.end()) {

        const auto cardOnTop = cardOnPosition->second.top();

        if (cardOnTop.GetValue() >= card.GetValue() or cardOnTop.GetIsGranite()) {
            return false;
        }

        if (cardOnTop.GetIsEter()) {
            return false;
        }

        if (cardOnTop.GetIsFlipped()) {
            if (cardOnTop.GetValue() >= card.GetValue()) {
                return false;
            }
        }
    }

    const auto [posX, posY]      = pos;
    const auto &[leftX, leftY]   = GetLeft();
    const auto &[rightX, rightY] = GetRight();
    const auto &[downX, downY]   = GetDown();
    const auto &[upX, upY]       = GetUp();

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
    const auto &[leftX, leftY]   = GetLeft();
    const auto &[rightX, rightY] = GetRight();
    const auto &[downX, downY]   = GetDown();
    const auto &[upX, upY]       = GetUp();

    if (std::abs(leftX - rightX) == m_MaxBoardSize - 1 &&
        std::abs(upY - downY) == m_MaxBoardSize - 1) {
        m_IsLocked = true;
        std::cout << "Board is locked\n";
    } else {
        m_IsLocked = false;
    }
}

bool Board::CheckPlacedCard(const Position &pos, const PlayerTurn &playerTurn) {
    return m_Board[pos].empty() || m_Board[pos].top().GetPlacedBy() == playerTurn;
}

bool Board::CheckTwoLinesFull() {
    std::unordered_map<int, int> numberOfCardsPerLines;

    for (const auto &[position, stacks] : m_Board) {
        if (!stacks.empty())
            ++numberOfCardsPerLines[position.first];
    }

    int counter = 0;

    for (const auto &[row, numberOfCards] : numberOfCardsPerLines) {
        if (numberOfCards == m_MaxBoardSize)
            ++counter;
    }

    if (counter >= 2)
        return true;

    return false;
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

bool Board::IsWithinBorderRestrictions(const Position &position) {
    if (!m_IsLocked) {
        return false;
    }

    const auto &[leftX, leftY]   = GetLeft();
    const auto &[rightX, rightY] = GetRight();
    const auto &[downX, downY]   = GetDown();
    const auto &[upX, upY]       = GetUp();

    return position.first >= leftX && position.first <= rightX && position.second >= upY &&
           position.second <= downY;
}

bool Board::UpdateDiagonals() {
    m_PrincipalDiagonal.clear();
    m_SecondaryDiagonal.clear();

    const auto &[leftX, leftY] = GetLeft();
    // const auto &[rightX, rightY] = GetRight(); // this is unused, again?
    const auto &[downX, downY] = GetDown();
    const auto &[upX, upY]     = GetUp();

    auto updateDiagonal = [&](auto &diagonal, auto condition) {
        for (const auto &position : m_Board | std::views::keys) {
            if (!condition(position))
                continue;
            std::cout << '\n';
            std::cout << position.first << ' ' << position.second << ' ';
            std::cout << (m_Board[position].top().GetPlacedBy() == PlayerTurn::Player1 ? 1 : -1);
            std::cout << '\n';
            if (m_Board[position].top().GetPlacedBy() == PlayerTurn::Player1) {
                diagonal[position.first] = 1;
            } else if (m_Board[position].top().GetPlacedBy() == PlayerTurn::Player2) {
                diagonal[position.first] = -1;
            }
        }
    };

    const auto isOnPrincipalDiagonal = [&](const Position &pos) {
        return pos.first - pos.second == leftX - upY;
    };

    const auto isOnSecondaryDiagonal = [&](const Position &pos) {
        return pos.first + pos.second == leftX + downY;
    };

    std::cout << "Diagonala Principala:\n";
    updateDiagonal(m_PrincipalDiagonal, isOnPrincipalDiagonal);
    std::cout << "Diagonala Secundara:\n";
    updateDiagonal(m_SecondaryDiagonal, isOnSecondaryDiagonal);

    return true; // TODO: This was missing a return, see if this was the right value lol
}

int Board::GetMaxBoardSize() const { return m_MaxBoardSize; }

GameBoard &Board::GetGameBoard() { return m_Board; }

Board::Board(const int maxBoardSize) :
    m_MaxBoardSize(maxBoardSize), m_Lines({}), m_Columns({}), m_PrincipalDiagonal({}),
    m_SecondaryDiagonal({}) {}

bool Board::IsBoardLocked() const { return m_IsLocked; }

InsertOutputs Board::InsertCard(Card card, const Position &pos, const PlayerTurn &playerTurn,
                                const CardType &cardType, Game &currentGame) {
    if (cardType == CardType::Granite and !m_Board[pos].empty())
        return InsertOutputs::GraniteOccupied;

    if (m_Board.empty()) {
        // ensure that we start in the middle regardless of where the first card is placed
        m_Board[START_POSITION].emplace(card);
        return InsertOutputs::Success;
    }

    if (!IsPositionValid(pos, card)) {
        std::cout << "Invalid position\n";
        return InsertOutputs::PositionInvalid;
    }

    if (cardType == CardType::Granite)
        return InsertOutputs::Success;

    if (cardType == CardType::Illusion) {
        if (!InsertIllusion(card, pos))
            return InsertOutputs::IllusionOccupied;

        if (playerTurn == PlayerTurn::Player1)
            currentGame.GetPlayer1().SetHasIllusionInGame(true);
        else
            currentGame.GetPlayer2().SetHasIllusionInGame(true);
    }

    if (cardType == CardType::Eter) {
        if (!InsertEter(card, pos))
            return InsertOutputs::EterOccupied;
    }

    int compensateForPlacingOnTop = 1;
    int playerVariation           = -1;

    if (playerTurn == PlayerTurn::Player1) {
        playerVariation = 1;
    }

    if (!CheckPlacedCard(pos, playerTurn)) {
        compensateForPlacingOnTop = 2;
    }

    m_Lines[pos.first] += compensateForPlacingOnTop * playerVariation;
    m_Columns[pos.second] += compensateForPlacingOnTop * playerVariation;

    if (!m_Board[pos].empty() and m_Board[pos].top().GetModifier() != 0) {
        auto cardOnTop = m_Board[pos].top();
        m_Board[pos].pop();
        cardOnTop.SetModifier(0);
        m_Board[pos].emplace(cardOnTop);
    }

    if (!m_Board[pos].empty()) {
        if (m_Board[pos].top().GetIsIllusion()) {
            if (CoverIllusion(card, pos)) {
                if (playerTurn == PlayerTurn::Player1)
                    currentGame.GetPlayer2().SetHasIllusionInGame(false);
                else
                    currentGame.GetPlayer1().SetHasIllusionInGame(false);
            }
        }
    }

    m_Board[pos].push(card);

    UpdateCorners(pos);
    CheckIsLocked();
    UpdateDiagonals();

    return InsertOutputs::Success;
}

bool Board::InsertIllusion(Card &card, const Position &pos) {
    if (!m_Board[pos].empty()) {
        std::cout << "Can't place an illusion on an occupied place\n";
        return false;
    }

    card.SetIllusion(true);

    return true;
}

bool Board::InsertEter(Card &card, const Position &pos) {
    if (!m_Board[pos].empty()) {
        std::cout << "Can't place an eter card on an occupied place\n";
        return false;
    }

    card.SetValue(1);
    card.SetEter(true);

    return true;
}

std::unordered_map<int, int> &Board::GetLineAdvantage() { return m_Lines; }

std::unordered_map<int, int> &Board::GetColumnAdvantage() { return m_Columns; }

std::unordered_map<int, int> &Board::GetPrincipalDiagonalAdvantage() { return m_PrincipalDiagonal; }

std::unordered_map<int, int> &Board::GetSecondaryDiagonalAdvantage() { return m_SecondaryDiagonal; }

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
