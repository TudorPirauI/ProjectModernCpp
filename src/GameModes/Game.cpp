//
// Created by edi on 28.11.2024.
//

#include <GameModes/Game.h>

#include <iostream>
#include <numeric>
#include <ranges>

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne,
           const std::string &nameTwo) :
    m_Board(boardSize), m_Player1(nameOne, {}), m_Player2(nameTwo, {}), m_ScoreToWin(scoreToWin) {}

PlayerTurn Game::GetCurrentPlayer() const { return m_PlayerTurn; }

Game::Game() : m_Board(Board(0)), m_Player1(Player("", {})), m_Player2(Player("", {})) {}

bool Game::CheckWinningConditions() {
    const auto &lines             = m_Board.GetLineAdvantage();
    const auto &columns           = m_Board.GetColumnAdvantage();
    const auto &principalDiagonal = m_Board.GetPrincipalDiagonalAdvantage();
    const auto &secondaryDiagonal = m_Board.GetSecondaryDiagonalAdvantage();

    const auto targetValue = m_Board.GetMaxBoardSize();

    const auto hasWinning = [&](const auto &data) {
        return std::ranges::any_of(data | std::views::values,
                                   [&](const auto &value) { return abs(value) == targetValue; });
    };

    if (hasWinning(lines) || hasWinning(columns)) {
        std::cout << "Lines or columns\n";
        return true;
    }

    if (!m_Board.IsBoardLocked()) {
        std::cout << "Board not locked\n";
        return false;
    }

    auto sumValues = [&](const auto &data) {
        auto values = data | std::views::values;
        return std::accumulate(values.begin(), values.end(), 0,
                               [](int sum, const auto &value) { return sum + value; });
    };

    const auto size         = m_Board.GetMaxBoardSize();
    const auto principalSum = sumValues(principalDiagonal);
    const auto secondarySum = sumValues(secondaryDiagonal);

    std::cout << std::format("Principal sum: {}\n", principalSum);
    std::cout << std::format("Secondary sum: {}\n", secondarySum);

    return principalSum == abs(size) || secondarySum == abs(size);
}

void Game::SetGameState(const GameState gameState) { m_GameState = gameState; }

void Game::IncreasePlayerScore(const PlayerTurn turn) {
    if (turn == PlayerTurn::Player1) {
        ++m_ScorePlayer1;
    } else {
        ++m_ScorePlayer2;
    }
}

void Game::SetNextPlayerTurn(const PlayerTurn playerTurn) { m_PlayerTurn = playerTurn; }

Board &Game::GetBoard() { return m_Board; }

Player &Game::GetPlayer1() { return m_Player1; }

Player &Game::GetPlayer2() { return m_Player2; }

void Game::SwitchPlayerTurn() {
    if (m_PlayerTurn == PlayerTurn::Player1)
        m_PlayerTurn = PlayerTurn::Player2;
    else
        m_PlayerTurn = PlayerTurn::Player1;
}

int Game::GetPlayer1Score() const { return m_ScorePlayer1; }

int Game::GetPlayer2Score() const { return m_ScorePlayer2; }

int Game::GetScoreToWin() const { return m_ScoreToWin; }

bool Game::VerifyWizardPower(const WizardPower &power, const Position &position,
                             const Position &posStack, const Card &card,
                             const PlayerTurn &playerTurn) {
    auto &board = m_Board.GetGameBoard();
    switch (power) {
        case WizardPower::EliminateOpponentCard: {
            if (auto &stack = board[position]; stack.size() >= 2) {
                stack.pop();
                return true;
            }

            return false;
        }
        // todo: review this function
        case WizardPower::RemoveRow: {
            const int rowToRemove = position.first;
            for (auto it = board.begin(); it != board.end();) {
                if (it->first.first == rowToRemove) {
                    it = board.erase(it);
                } else {
                    ++it;
                }
            }
            m_Board.UpdateDiagonals();
            return true;
        }
        case WizardPower::CoverOpponentCard: {
            if (auto &stack = board[position];
                !stack.empty() and stack.top().GetValue() > card.GetValue()) {
                stack.emplace(card);
                return true;
            }

            return false;
        }
        case WizardPower::CreatePit: {
            if (auto &stack = board[position]; stack.empty()) {
                stack.emplace(true);
                return true;
            }

            return false;
        }

        // todo: fix the error
        case WizardPower::MoveOwnStack: {
            if (board[posStack].empty() and board[position].top().GetPlacedBy() == playerTurn) {
                board[posStack] = board[position];
                while (!board[position].empty()) {
                    board[position].pop();
                }
                return true;
            }
            m_Board.UpdateDiagonals();
            return false;
        }
        // todo: fix the error
        case WizardPower::MoveOpponentStack: {
            if (board[posStack].empty() and board[position].top().GetPlacedBy() != playerTurn) {
                board[posStack] = board[position];
                while (!board[position].empty()) {
                    board[position].pop();
                }
                return true;
            }
            m_Board.UpdateDiagonals();
            return false;
        }

        case WizardPower::GainEterCard: {
            if (playerTurn == PlayerTurn::Player1) {
                m_Player1.GiveEterCard(PlayerTurn::Player1);
            } else {
                m_Player2.GiveEterCard(PlayerTurn::Player2);
            }
            return true;
        }

        case WizardPower::ShiftRowToEdge: {
            const auto &[leftX, leftY]   = m_Board.GetLeft();
            const auto &[rightX, rightY] = m_Board.GetRight();
            const auto &[upX, upY]       = m_Board.GetUp();
            const auto &[downX, downY]   = m_Board.GetDown();

            if (position.first == upX || position.first == downX) {
                int cardCount = 0;
                for (auto i = leftY; i <= rightY; ++i) {
                    if (!board[{position.first, i}].empty()) {
                        ++cardCount;
                    }
                }

                // todo: make the board to shift to the right edge
                if (cardCount >= 3) {
                    // Shift row to the left edge
                    for (auto i = leftY; i <= rightY; ++i) {
                        board[{position.first, leftY + (i - leftY)}] = board[{position.first, i}];
                        if (i != leftY) {
                            board.erase({position.first, i});
                        }
                    }
                    m_Board.UpdateDiagonals();
                    return true;
                }
            }
            return false;
        }
        default: {
            return false;
        }
    }
}

bool Game::VerifyElementalPower(const ElementIndexPower &power, const Position &firstPosition,
                                const Position &secondPosition, const Card &card,
                                const PlayerTurn playerTurn) {
    auto &board = m_Board.GetGameBoard();
    switch (power) {
        case ElementIndexPower::ControlledExplosion:
            return "The board explodes!";
        case ElementIndexPower::Destruction:
            return "Removes the opponent's last played card from the game.";
        case ElementIndexPower::Flames:
            return "Flip the opponent's illusion face up. Then play a card on any position on the "
                   "board.";
        case ElementIndexPower::Lava:
            return "Choose a number, provided that at least 2 cards with that number are visible "
                   "on the board. All visible cards with this number return to their owners' "
                   "hands.";
        case ElementIndexPower::FromAshes:
            return "Choose one of your own cards that was removed from the game and play it "
                   "immediately.";
        case ElementIndexPower::Sparks:
            return "Choose any of your own cards covered by the opponent and play it immediately "
                   "on another position.";
        case ElementIndexPower::Blizzard:
            return "Return a visible card of the opponent to their hand.";
        case ElementIndexPower::Gale:
            return "All cards covered by other cards return to their owners' hands.";
        case ElementIndexPower::Hurricane:
            return "Shift a fully occupied row by 1 position in the desired direction. Cards in "
                   "the stack that move out of the board boundaries return to their owners' hands.";
        case ElementIndexPower::Gust:
            return "Move any visible card on the board horizontally or vertically to an adjacent "
                   "position with a card of a lower number.";
        case ElementIndexPower::Mirage: {
            if (!board[firstPosition].empty() and board[firstPosition].top().GetIsIllusion() and
                board[firstPosition].top().GetPlacedBy() == playerTurn) {
                const auto &cardIlussion = board[firstPosition].top();
                board[firstPosition].pop();

                m_Player1.GiveCard(cardIlussion);
                board[firstPosition].emplace(card);

                return true;
            }

            return false;
        }
        case ElementIndexPower::Storm: {
            if (board[firstPosition].size() >= 2) {
                while (!board[firstPosition].empty()) {
                    board[firstPosition].pop();
                }
                m_Board.UpdateDiagonals();
                return true;
            }

            return false;
        }
        case ElementIndexPower::Tide: {
            if (!board[firstPosition].empty() and !board[secondPosition].empty()) {
                std::swap(board[firstPosition], board[secondPosition]);
                return true;
            }

            return false;
        }
        case ElementIndexPower::Fog: {
            if (playerTurn == PlayerTurn::Player1) {
                m_Player1.SetIllusion(m_Player1.GetIllusion() + 1);
                return true;
            }

            m_Player2.SetIllusion(m_Player2.GetIllusion() + 1);
            return true;
        }

        case ElementIndexPower::Wave:
            return "Move a stack to an adjacent empty position. Play a card on the newly empty "
                   "position.";
        case ElementIndexPower::Whirlpool:
            return "Move 2 cards from the same row, separated by an empty space, into that empty "
                   "space. The card with the higher number goes on top, and in case of a tie, the "
                   "player chooses.";
        case ElementIndexPower::Tsunami: {
            "Choose a row. During the opponent's next turn, they cannot "
            "place cards on that row.";
        }

        // todo: fix this method:)
        case ElementIndexPower::Waterfall: {
            const auto &[leftX, leftY]   = m_Board.GetLeft();
            const auto &[rightX, rightY] = m_Board.GetRight();
            const auto row               = firstPosition.first;

            int cardCount = 0;

            for (auto i = leftY; i <= rightY; ++i) {
                if (!board[{row, i}].empty()) {
                    ++cardCount;
                }
            }

            if (cardCount >= 3) {
                std::stack<Card> cards;
                for (auto i = leftY; i <= rightY; ++i) {
                    while (!board[{row, i}].empty()) {
                        cards.push(board[{row, i}].top());
                        board[{row, i}].pop();
                    }

                    if (i != leftY) {
                        board.erase({row, i});
                    }
                }
                board[{row, leftY}] = std::move(cards);
                m_Board.UpdateDiagonals();
                return true;
            }
            return false;
        }
        case ElementIndexPower::Support:
            return "The value of one of your cards (1/2/3) increases by 1. If that card is covered "
                   "or returned to the hand, it loses the bonus.";
        case ElementIndexPower::Earthquake: {
            for (auto it = board.begin(); it != board.end();) {
                if (auto &stack = it->second; !stack.empty() && stack.top().GetValue() == 1) {
                    stack.pop();
                    if (stack.empty()) {
                        it = board.erase(it);
                        continue;
                    }
                }
                ++it;
            }
            m_Board.UpdateDiagonals();
            return true;
        }
        case ElementIndexPower::Shattering:
            return "The value of an opponent's card (2/3/4) decreases by 1. If that card is "
                   "covered or returned to the hand, it loses the penalty.";
        case ElementIndexPower::Granite:
            return "Place a neutral card on the board such that it defines at least one boundary "
                   "of the game board.";
        case ElementIndexPower::Avalanche:
            return "Choose 2 adjacent stacks horizontally/vertically. Shift the two stacks by 1 "
                   "position in their row.";
        case ElementIndexPower::Boulder: {
            if (!board[firstPosition].empty() and board[firstPosition].top().GetIsIllusion()) {
                board[firstPosition].pop();
                board[firstPosition].emplace(card);

                return true;
            }

            return false;
        }
        default:
            return "Unknown power.";
    }

    m_Board.UpdateDiagonals();

    return false;
}
