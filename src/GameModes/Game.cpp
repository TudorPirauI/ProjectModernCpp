//
// Created by edi on 28.11.2024.
//

#include "GameModes/Game.h"
#include "pch.h"

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
        case WizardPower::RemoveRow: {
            const int rowToRemove  = position.first;
            auto      newBoard     = RemadeGameBoard(m_Board);
            auto     &newGameBoard = newBoard.GetGameBoard();

            for (auto it = newGameBoard.begin(); it != newGameBoard.end(); ++it) {
                if (it->first.first == rowToRemove) {
                    it = board.erase(it);
                }
            }

            const auto &checkNewBoard = RemadeGameBoard(newBoard);

            if (checkNewBoard.GetMaxBoardSize() == 0)
                return false;

            m_Board = checkNewBoard;
            m_Board.CheckIsLocked();

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

            m_Board.UpdateDiagonals();
            return false;
        }
        case WizardPower::MoveOwnStack: {
            auto newBoard = RemadeGameBoard(m_Board);

            if (auto &newGameBoard = newBoard.GetGameBoard();
                newGameBoard[posStack].empty() and !newGameBoard[position].empty() and
                newGameBoard[position].top().GetPlacedBy() == playerTurn) {

                while (!newGameBoard[position].empty()) {
                    const auto &cardOnTop = newGameBoard[position].top();
                    const auto  result    = newBoard.InsertCard(
                            cardOnTop, position, cardOnTop.GetPlacedBy(), GetCardType(card));

                    if (result == false)
                        return false;
                }

                while (!board[position].empty()) {
                    board[position].pop();
                }

                newBoard.UpdateDiagonals();
                m_Board = newBoard;
                return true;
            }

            return false;
        }
        case WizardPower::MoveOpponentStack: {
            auto newBoard = RemadeGameBoard(m_Board);
            if (auto &newGameBoard = newBoard.GetGameBoard();
                newGameBoard[posStack].empty() and !newGameBoard[position].empty() and
                newGameBoard[position].top().GetPlacedBy() != playerTurn) {

                while (!newGameBoard[position].empty()) {
                    const auto &cardOnTop = newGameBoard[position].top();
                    const auto  result    = newBoard.InsertCard(
                            cardOnTop, position, cardOnTop.GetPlacedBy(), GetCardType(card));

                    if (result == false)
                        return false;
                }

                while (!board[position].empty()) {
                    board[position].pop();
                }

                newBoard.UpdateDiagonals();
                m_Board = newBoard;

                return true;
            }
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

            auto countOccupiedPositions = [&](int row) {
                int count = 0;
                for (auto i = leftY; i <= rightY; ++i) {
                    if (!board[{row, i}].empty()) {
                        ++count;
                    }
                }
                return count;
            };

            const auto &occupiedPositions = countOccupiedPositions(position.first);

            if (position.first == upX and occupiedPositions >= 3) {
                for (auto i = leftY; i <= rightY; ++i) {
                    if (!board[{position.first, i}].empty()) {
                        board[{downX + 1, i}] = std::move(board[{position.first, i}]);
                        board.erase({position.first, i});
                    }
                }
                m_Board.UpdateDiagonals();
                return true;
            }
            if (position.first == downX and occupiedPositions >= 3) {
                for (auto i = leftY; i <= rightY; ++i) {
                    if (!board[{position.first, i}].empty()) {
                        board[{upX - 1, i}] = std::move(board[{position.first, i}]);
                        board.erase({position.first, i});
                    }
                }
                m_Board.UpdateDiagonals();
                return true;
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
                                const PlayerTurn playerTurn, int chosenNumber) {
    auto &board = m_Board.GetGameBoard();

    switch (power) {
        case ElementIndexPower::ControlledExplosion:
            return "The board explodes!";
        case ElementIndexPower::Destruction: {
            if (playerTurn == PlayerTurn::Player1) {
                m_Player2.AddToRemovedCards(board[m_LastPositionPlayer2].top());
                board[m_LastPositionPlayer2].pop();
            } else {
                m_Player1.AddToRemovedCards(board[m_LastPositionPlayer1].top());
                board[m_LastPositionPlayer1].pop();
            }

            return true;
        }
        case ElementIndexPower::Flames: {
            if (board[firstPosition].empty())
                return false;

            return board[firstPosition].top().GetIsIllusion();
        }
        case ElementIndexPower::Lava: {
            int count = 0;

            for (const auto &stack : board | std::views::values) {
                if (!stack.empty() && stack.top().GetValue() == chosenNumber) {
                    ++count;
                }
            }

            if (count < 2) {
                return false;
            }

            for (auto &stack : board | std::views::values) {
                if (!stack.empty() && stack.top().GetValue() == chosenNumber) {
                    auto cardToReturn = stack.top();
                    stack.pop();

                    if (cardToReturn.GetPlacedBy() == PlayerTurn::Player1) {
                        m_Player1.GiveCard(cardToReturn);
                    } else {
                        m_Player2.GiveCard(cardToReturn);
                    }
                }
            }

            m_Board.UpdateDiagonals();
            return true;
        }
        case ElementIndexPower::FromAshes: {
            return true;
        }
        case ElementIndexPower::Sparks: {
            if (board[firstPosition].size() < 2)
                return false;

            const auto &cardOnTop = board[firstPosition].top();
            board[firstPosition].pop();

            const auto &cardForPower = board[firstPosition].top();

            board[firstPosition].pop();

            if (m_Board.InsertCard(cardForPower, secondPosition, playerTurn)) {
                board[firstPosition].emplace(cardOnTop);
                return true;
            }

            board[firstPosition].emplace(cardForPower);
            board[firstPosition].emplace(cardOnTop);

            return false;
        }
        case ElementIndexPower::Blizzard: {
            if (board[firstPosition].empty())
                return false;

            const auto &cardToReturn = board[firstPosition].top();
            board[firstPosition].pop();

            if (playerTurn == PlayerTurn::Player1)
                m_Player2.GiveCard(cardToReturn);
            else
                m_Player1.GiveCard(cardToReturn);

            return true;
        }
        case ElementIndexPower::Gale: {
            for (auto &val : board | std::views::values) {
                if (auto &stack = val; stack.size() > 1) {
                    std::stack<Card> tempStack;
                    while (stack.size() > 1) {
                        tempStack.push(stack.top());
                        stack.pop();
                    }
                    while (!tempStack.empty()) {
                        auto cardToGive = tempStack.top();
                        tempStack.pop();
                        if (cardToGive.GetPlacedBy() == PlayerTurn::Player1) {
                            m_Player1.GiveCard(cardToGive);
                        } else {
                            m_Player2.GiveCard(cardToGive);
                        }
                    }
                }
            }
            return true;
        }
        // todo: make a review for this function
        case ElementIndexPower::Hurricane: {
            const int row = firstPosition.first;

            int                   countCards = 0;
            std::vector<Position> stacksPosition;

            for (const auto &[position, stack] : m_Board.GetGameBoard()) {
                if (position.first == row and !stack.empty()) {
                    ++countCards;
                    stacksPosition.emplace_back(position);
                }
            }

            if (countCards != m_Board.GetMaxBoardSize() + 1) {
                return false;
            }

            auto  newBoard     = RemadeGameBoard(m_Board);
            auto &newGameBoard = newBoard.GetGameBoard();
            Hand  player1Hand{};
            Hand  player2Hand{};

            for (auto it = stacksPosition.rbegin(); it != stacksPosition.rend(); ++it) {
                if (const auto &pos = *it;
                    !newBoard.IsWithinBorderRestrictions({pos.first, pos.second + 1})) {
                    auto stack = newGameBoard[{pos.first, pos.second}];
                    while (!stack.empty()) {
                        if (const auto &cardOnTop = stack.top();
                            cardOnTop.GetPlacedBy() == PlayerTurn::Player1) {
                            player1Hand.emplace_back(cardOnTop);
                        } else {
                            player2Hand.emplace_back(cardOnTop);
                        }
                        stack.pop();
                    }
                    if (newBoard.UpdateDiagonals())
                        return false;
                } else {
                    newGameBoard[{pos.first, pos.second + 1}] = std::move(newGameBoard[pos]);
                    newGameBoard[pos]                         = {};
                }
            }

            auto giveCardsFromHand = [](Player &player, const Hand &hand) {
                for (const auto &cardHand : hand) {
                    player.GiveCard(cardHand);
                }
            };

            giveCardsFromHand(m_Player1, player1Hand);
            giveCardsFromHand(m_Player2, player2Hand);

            m_Board.UpdateDiagonals();
            return true;
        }
        case ElementIndexPower::Gust: {
            if (firstPosition == secondPosition or
                std::abs(firstPosition.first - secondPosition.first > 1) or
                std::abs(firstPosition.second - secondPosition.second) > 1) {
                return false;
            }

            auto &firstStack  = board[firstPosition];
            auto &secondStack = board[secondPosition];

            if (firstStack.empty() || secondStack.empty()) {
                return false;
            }

            const auto &firstCard = firstStack.top();

            if (const auto &secondCard = secondStack.top();
                firstCard.GetValue() <= secondCard.GetValue()) {
                return false;
            }

            CheckModifierCard(secondStack);
            secondStack.push(firstCard);
            firstStack.pop();

            m_Board.UpdateDiagonals();
            return true;
        }
        case ElementIndexPower::Mirage: {
            if (!board[firstPosition].empty() and board[firstPosition].top().GetIsIllusion() and
                board[firstPosition].top().GetPlacedBy() == playerTurn) {
                const auto &cardIllusion = board[firstPosition].top();
                board[firstPosition].pop();

                m_Player1.GiveCard(cardIllusion);
                board[firstPosition].emplace(card);

                return true;
            }

            return false;
        }
        case ElementIndexPower::Storm: {
            auto  newBoard     = RemadeGameBoard(m_Board);
            auto &newGameBoard = newBoard.GetGameBoard();

            for (auto &[position, stack] : board) {
                if (stack.size() >= 2) {
                    while (!stack.empty()) {
                        stack.pop();
                    }
                    newBoard.UpdateDiagonals();
                }
            }

            if (const auto &checkBoard = RemadeGameBoard(newBoard);
                checkBoard.GetMaxBoardSize() == 0)
                return false;

            newBoard.UpdateDiagonals();
            m_Board = newBoard;

            return true;
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
        case ElementIndexPower::Wave: {
            if (board[firstPosition].empty() or !board[secondPosition].empty())
                return false;

            if (std::abs(firstPosition.first - secondPosition.first) > 1 or
                std::abs(firstPosition.second - secondPosition.second) > 1)
                return false;

            board[secondPosition] = std::move(board[firstPosition]);

            board[firstPosition] = {};

            board[firstPosition].emplace(card);

            return true;
        }
        case ElementIndexPower::Whirlpool: {
            if (firstPosition.first != secondPosition.first) {
                return false;
            }

            auto &firstStack  = board[firstPosition];
            auto &secondStack = board[secondPosition];

            if (firstStack.empty() || secondStack.empty()) {
                return false;
            }

            Position emptySpace = {firstPosition.first,
                                   (firstPosition.second + secondPosition.second) / 2};

            if (!board[emptySpace].empty()) {
                return false;
            }

            const auto &firstCardStack  = firstStack.top();
            const auto &secondCardStack = secondStack.top();

            firstStack.pop();
            secondStack.pop();

            std::stack<Card> emptyStack;

            if (firstCardStack.GetValue() > secondCardStack.GetValue()) {
                emptyStack.emplace(firstCardStack);
                emptyStack.emplace(secondCardStack);
            } else {
                emptyStack.emplace(secondCardStack);
                emptyStack.emplace(firstCardStack);
            }

            m_Board.UpdateDiagonals();
            return true;
        }
        case ElementIndexPower::Tsunami: {
            if (playerTurn == PlayerTurn::Player1)
                m_RowPlayer2 = firstPosition.first;
            else {
                m_RowPlayer1 = firstPosition.first;
            }

            return true;
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
        case ElementIndexPower::Support: {
            if (board[firstPosition].empty())
                return false;

            if (auto &cardOnTop = board[firstPosition].top();
                cardOnTop.GetPlacedBy() != playerTurn and 1 <= cardOnTop.GetValue() and
                cardOnTop.GetValue() <= 3) {
                cardOnTop.SetModifier(1);
                return true;
            }

            return false;
        }
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
        case ElementIndexPower::Shattering: {
            if (board[firstPosition].empty())
                return false;

            if (auto &cardOnTop = board[firstPosition].top();
                cardOnTop.GetPlacedBy() != playerTurn and 2 <= cardOnTop.GetValue() and
                cardOnTop.GetValue() <= 4) {
                cardOnTop.SetModifier(-1);
                return true;
            }

            return false;
        }
        case ElementIndexPower::Granite: {
            auto newBoard = RemadeGameBoard(m_Board);

            Card graniteCard{};

            graniteCard.SetIsGranite(true);

            if (!newBoard.InsertCard(card, firstPosition, PlayerTurn::Granite, CardType::Granite))
                return false;

            if (!newBoard.UpdateDiagonals())
                return false;

            m_Board = newBoard;

            return true;
        }
        case ElementIndexPower::Avalanche: {
            if (std::abs(firstPosition.first - secondPosition.first) > 1 ||
                std::abs(firstPosition.second - secondPosition.second) > 1) {
                return false;
            }

            auto shiftStack = [&](const Position &from, const Position &to) {
                while (!board[from].empty()) {
                    board[to].push(board[from].top());
                    board[from].pop();
                }
            };

            if (firstPosition.first == secondPosition.first) {
                Position left  = firstPosition.second < secondPosition.second ? firstPosition
                                                                              : secondPosition;
                Position right = firstPosition.second < secondPosition.second ? secondPosition
                                                                              : firstPosition;

                if (board[{left.first, left.second - 1}].empty()) {
                    shiftStack(left, {left.first, left.second - 1});
                    shiftStack(right, {right.first, right.second - 1});
                } else if (board[{right.first, right.second + 1}].empty()) {
                    shiftStack(right, {right.first, right.second + 1});
                    shiftStack(left, {left.first, left.second + 1});
                } else {
                    return false;
                }
            } else {
                Position top =
                        firstPosition.first < secondPosition.first ? firstPosition : secondPosition;
                Position bottom =
                        firstPosition.first < secondPosition.first ? secondPosition : firstPosition;

                if (board[{top.first - 1, top.second}].empty()) {
                    shiftStack(top, {top.first - 1, top.second});
                    shiftStack(bottom, {bottom.first - 1, bottom.second});
                } else if (board[{bottom.first + 1, bottom.second}].empty()) {
                    shiftStack(bottom, {bottom.first + 1, bottom.second});
                    shiftStack(top, {top.first + 1, top.second});
                } else {
                    return false;
                }
            }

            m_Board.UpdateDiagonals();
            return true;
        }
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

    return false;
}

void Game::SetLastCardPlayer1(const Position &position) { m_LastPositionPlayer1 = position; }

void Game::SetLastCardPlayer2(const Position &position) { m_LastPositionPlayer2 = position; }

void Game::CheckModifierCard(std::stack<Card> &stack) {
    if (stack.top().GetModifier() != 0) {
        auto &card = stack.top();
        stack.pop();
        card.SetModifier(0);
        stack.emplace(card);
    }
}

Position Game::GetLastCardPlayer1() { return m_LastPositionPlayer1; }
Position Game::GetLastCardPlayer2() { return m_LastPositionPlayer2; }

int Game::GetRowPlayer1() { return m_RowPlayer1; }
int Game::GetRowPlayer2() { return m_RowPlayer2; }

bool Game::CheckPlayerIllusion(Player &player) {
    if (player.GetIllusion() > 0) {
        player.SetIllusion(player.GetIllusion() - 1);
        return true;
    }

    return false;
}

CardType Game::GetCardType(const Card &card) {
    if (card.GetIsEter())
        return CardType::Eter;
    if (card.GetIsIllusion())
        return CardType::Illusion;

    return CardType::Normal;
}

Board Game::RemadeGameBoard(Board board) {
    Board modifiedBoard{board.GetMaxBoardSize()};

    for (auto [position, stack] : board.GetGameBoard()) {
        while (!stack.empty()) {
            const auto &card = stack.top();
            stack.pop();

            if (!modifiedBoard.InsertCard(card, position, card.GetPlacedBy(), GetCardType(card))) {
                return Board{0};
            }
        }
    }

    return board;
}
