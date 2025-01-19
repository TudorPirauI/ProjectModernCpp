//
// Created by edi on 28.11.2024.
//

#include "GameModes/Game.h"

Game::Game(const int boardSize, const int scoreToWin, const std::string &nameOne,
           const std::string &nameTwo, const std::array<bool, 3> &options) :
    m_Board(boardSize), m_Player1(nameOne, {}), m_Player2(nameTwo, {}), m_ScoreToWin(scoreToWin) {
    m_EterEnabled      = options[0];
    m_IllusionEnabled  = options[1];
    m_ExplosionEnabled = options[2];

    if (m_EterEnabled) {
        Card cardEter{1};

        cardEter.SetEter(true);
        cardEter.SetPlacedBy(PlayerTurn::Player1);

        m_Player1.GiveCard(cardEter);
        cardEter.SetPlacedBy(PlayerTurn::Player2);
        m_Player2.GiveCard(cardEter);
    }

    int numberOfIllusions = 0;

    if (m_IllusionEnabled)
        numberOfIllusions = 1;

    m_Player1.SetIllusion(numberOfIllusions);
    m_Player2.SetIllusion(numberOfIllusions);

    // if (m_ExplosionEnabled) {
    //     m_Player1.SetHasExplosion(true);
    //     m_Player2.SetHasExplosion(true);
    // }
}

PlayerTurn Game::GetCurrentPlayerTurn() const { return m_PlayerTurn; }

Player &Game::GetCurrentPlayer() {
    if (m_PlayerTurn == PlayerTurn::Player1)
        return m_Player1;

    return m_Player2;
}

void Game::SwapTurn() {
    m_PlayerTurn = m_PlayerTurn == PlayerTurn::Player1 ? PlayerTurn::Player2 : PlayerTurn::Player1;
}

Game::Game() : m_Board(Board(0)), m_Player1(Player("", {})), m_Player2(Player("", {})) {}

Game::WinningCondition Game::CheckWinningConditions() {
    const auto &lines             = m_Board.GetLineAdvantage();
    const auto &columns           = m_Board.GetColumnAdvantage();
    const auto &principalDiagonal = m_Board.GetPrincipalDiagonalAdvantage();
    const auto &secondaryDiagonal = m_Board.GetSecondaryDiagonalAdvantage();

    const auto targetValue = m_Board.GetMaxBoardSize();

    const auto hasWinning = [&](const auto &data) {
        return std::ranges::any_of(data | std::views::values,
                                   [&](const auto &value) { return abs(value) == targetValue; });
    };

    if (hasWinning(lines)) {
        std::cout << "Lines\n";
        return WinningCondition::LineWin;
    }

    if (hasWinning(columns)) {
        std::cout << "Columns\n";
        return WinningCondition::ColumnWin;
    }

    if (!m_Board.IsBoardLocked()) {
        std::cout << "Board not locked\n";
        return WinningCondition::NoWin;
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

    if (principalSum == abs(size))
        return WinningCondition::DiagonalPrincipalWin;

    if (secondarySum == abs(size))
        return WinningCondition::DiagonalSecondaryWin;

    return WinningCondition::NoWin;
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

void Game::SetNewCards() {}

void Game::SetPlayerTurn(const PlayerTurn &playerTurn) { m_PlayerTurn = playerTurn; }

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
                            cardOnTop, position, cardOnTop.GetPlacedBy(), GetCardType(card), *this);

                    if (result != InsertOutputs::Success)
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
                            cardOnTop, position, cardOnTop.GetPlacedBy(), GetCardType(card), *this);

                    if (result != InsertOutputs::Success)
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
            if (!m_EterEnabled)
                return false;

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
        case ElementIndexPower::ControlledExplosion: {
            if (m_ExplosionEnabled) {
                bool result = CheckExplosion();
                std::cout << "ControlledExplosion: " << result << '\n';
                return result;
            }
            std::cout << "ControlledExplosion: false\n";
            return false;
        }
        case ElementIndexPower::Destruction: {
            if (playerTurn == PlayerTurn::Player1) {
                m_Player2.AddToRemovedCards(board[m_LastPositionPlayer2].top());
                board[m_LastPositionPlayer2].pop();
                if (board[m_LastPositionPlayer2].empty()) {
                    board.erase(m_LastPositionPlayer2);
                }
            } else {
                m_Player1.AddToRemovedCards(board[m_LastPositionPlayer1].top());
                board[m_LastPositionPlayer1].pop();

                if (board[m_LastPositionPlayer1].empty()) {
                    board.erase(m_LastPositionPlayer1);
                }
            }
            std::cout << "Destruction: true\n";
            return true;
        }
        case ElementIndexPower::Flames: {
            if (board[firstPosition].empty()) {
                std::cout << "Flames: false (empty stack)\n";
                return false;
            }
            bool result = board[firstPosition].top().GetIsIllusion();
            std::cout << "Flames: " << result << '\n';
            return result;
        }
        case ElementIndexPower::Lava: {
            int count = 0;
            for (const auto &stack : board | std::views::values) {
                if (!stack.empty() && stack.top().GetValue() == chosenNumber) {
                    ++count;
                }
            }
            if (count < 2) {
                std::cout << "Lava: false (count < 2)\n";
                return false;
            }
            for (auto &[pos, stack] : board) {
                if (!stack.empty() && stack.top().GetValue() == chosenNumber) {
                    auto cardToReturn = stack.top();
                    stack.pop();
                    if (cardToReturn.GetPlacedBy() == PlayerTurn::Player1) {
                        m_Player1.GiveCard(cardToReturn);
                    } else {
                        m_Player2.GiveCard(cardToReturn);
                    }

                    if (stack.empty()) {
                        board.erase(pos);
                    }
                }
            }
            m_Board.UpdateDiagonals();
            std::cout << "Lava: true\n";
            return true;
        }
        case ElementIndexPower::FromAshes: {
            std::cout << "FromAshes: true\n";
            return true;
        }
        case ElementIndexPower::Sparks: {
            if (board[firstPosition].size() < 2) {
                std::cout << "Sparks: false (stack size < 2)\n";
                return false;
            }
            const auto &cardOnTop = board[firstPosition].top();
            board[firstPosition].pop();
            const auto &cardForPower = board[firstPosition].top();
            board[firstPosition].pop();
            if (m_Board.InsertCard(cardForPower, secondPosition, playerTurn, GetCardType(card),
                                   *this) == InsertOutputs::Success) {
                board[firstPosition].emplace(cardOnTop);
                std::cout << "Sparks: true\n";
                return true;
            }
            board[firstPosition].emplace(cardForPower);
            board[firstPosition].emplace(cardOnTop);
            m_Board.UpdateDiagonals();
            m_Board.CheckIsLocked();
            std::cout << "Sparks: false (insert failed)\n";
            return false;
        }
        case ElementIndexPower::Blizzard: {
            if (board[firstPosition].empty()) {
                std::cout << "Blizzard: false (empty stack)\n";
                return false;
            }
            const auto &cardToReturn = board[firstPosition].top();
            board[firstPosition].pop();
            if (playerTurn == PlayerTurn::Player1) {
                m_Player2.GiveCard(cardToReturn);
            } else {
                m_Player1.GiveCard(cardToReturn);
            }
            m_Board.UpdateDiagonals();
            m_Board.CheckIsLocked();
            std::cout << "Blizzard: true\n";
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
            std::cout << "Gale: true\n";
            return true;
        }
        case ElementIndexPower::Hurricane: {
            const int             row        = firstPosition.first;
            int                   countCards = 0;
            std::vector<Position> stacksPosition;
            for (const auto &[position, stack] : m_Board.GetGameBoard()) {
                if (position.first == row && !stack.empty()) {
                    ++countCards;
                    stacksPosition.emplace_back(position);
                }
            }
            if (countCards != m_Board.GetMaxBoardSize()) {
                std::cout << countCards << " Board Size: " << m_Board.GetMaxBoardSize() << '\n';
                std::cout << "Hurricane: false (countCards != max board size)\n";
                return false;
            }
            auto  newBoard     = RemadeGameBoard(m_Board);
            auto &newGameBoard = newBoard.GetGameBoard();
            Hand  player1Hand{};
            Hand  player2Hand{};
            for (auto &pos : std::ranges::reverse_view(stacksPosition)) {
                if (!newBoard.IsWithinBorderRestrictions({pos.first, pos.second + 1})) {
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
            m_Board.CheckIsLocked();
            std::cout << "Hurricane: true\n";
            return true;
        }
        case ElementIndexPower::Gust: {
            if (firstPosition == secondPosition ||
                std::abs(firstPosition.first - secondPosition.first) > 1 ||
                std::abs(firstPosition.second - secondPosition.second) > 1) {
                std::cout << "Gust: false (invalid positions)\n";
                return false;
            }
            auto &firstStack  = board[firstPosition];
            auto &secondStack = board[secondPosition];
            if (firstStack.empty() || secondStack.empty()) {
                std::cout << "Gust: false (empty stack)\n";
                return false;
            }
            const auto &firstCard = firstStack.top();
            if (const auto &secondCard = secondStack.top();
                firstCard.GetValue() > secondCard.GetValue()) {
                std::cout << "Gust: false (first card value > second card value)\n";
                return false;
            }
            CheckModifierCard(secondStack);
            secondStack.push(firstCard);
            firstStack.pop();
            m_Board.UpdateDiagonals();
            std::cout << "Gust: true\n";
            return true;
        }
        case ElementIndexPower::Mirage: {
            if (!m_IllusionEnabled) {
                std::cout << "Mirage: false (illusion not enabled)\n";
                return false;
            }
            if (!board[firstPosition].empty() && board[firstPosition].top().GetIsIllusion() &&
                board[firstPosition].top().GetPlacedBy() == playerTurn) {
                const auto &cardIllusion = board[firstPosition].top();
                board[firstPosition].pop();
                m_Player1.GiveCard(cardIllusion);
                board[firstPosition].emplace(card);
                std::cout << "Mirage: true\n";
                return true;
            }
            std::cout << "Mirage: false (conditions not met)\n";
            return false;
        }
        case ElementIndexPower::Storm: {
            auto newBoard = RemadeGameBoard(m_Board);
            for (auto &stack : board | std::views::values) {
                if (stack.size() >= 2) {
                    std::cout << "Removing stack\n";
                    while (!stack.empty()) {
                        std::cout << "Removing " << stack.top().GetValue() << '\n';
                        stack.pop();
                    }
                    newBoard.UpdateDiagonals();
                }
            }
            if (const auto &checkBoard = RemadeGameBoard(newBoard);
                checkBoard.GetMaxBoardSize() == 0) {
                std::cout << "Storm: false (invalid board size)\n";
                return false;
            }
            newBoard.UpdateDiagonals();
            newBoard.CheckIsLocked();
            m_Board = newBoard;
            std::cout << "Storm: true\n";
            return true;
        }
        case ElementIndexPower::Tide: {
            if (!board[firstPosition].empty() && !board[secondPosition].empty()) {
                std::swap(board[firstPosition], board[secondPosition]);
                std::cout << "Tide: true\n";
                return true;
            }
            std::cout << "Tide: false (empty stack)\n";
            return false;
        }
        case ElementIndexPower::Fog: {
            if (!m_IllusionEnabled) {
                std::cout << "Fog: false (illusion not enabled)\n";
                return false;
            }
            if (playerTurn == PlayerTurn::Player1) {
                m_Player1.SetIllusion(m_Player1.GetIllusion() + 1);
            } else {
                m_Player2.SetIllusion(m_Player2.GetIllusion() + 1);
            }
            std::cout << "Fog: true\n";
            return true;
        }
        case ElementIndexPower::Wave: {
            if (board[firstPosition].empty() || !board[secondPosition].empty()) {
                std::cout << "Wave: false (invalid stack state)\n";
                return false;
            }
            if (std::abs(firstPosition.first - secondPosition.first) > 1 ||
                std::abs(firstPosition.second - secondPosition.second) > 1) {
                std::cout << "Wave: false (invalid positions)\n";
                return false;
            }
            auto  newBoard               = RemadeGameBoard(m_Board);
            auto &newGameBoard           = newBoard.GetGameBoard();
            newGameBoard[secondPosition] = std::move(newGameBoard[firstPosition]);
            newGameBoard[firstPosition]  = {};
            newGameBoard[firstPosition].emplace(card);
            if (const auto &remadeNewBoard = RemadeGameBoard(newBoard);
                remadeNewBoard.GetMaxBoardSize() != 0) {
                m_Board = remadeNewBoard;
                m_Board.UpdateDiagonals();
                m_Board.CheckIsLocked();
                std::cout << "Wave: true\n";
                return true;
            }
            std::cout << "Wave: false (invalid board size)\n";
            return false;
        }
        case ElementIndexPower::Whirlpool: {
            if (firstPosition.first != secondPosition.first) {
                std::cout << "Whirlpool: false (invalid row)\n";
                return false;
            }
            auto &firstStack  = board[firstPosition];
            auto &secondStack = board[secondPosition];
            if (firstStack.empty() || secondStack.empty() || firstStack.size() > 1 ||
                secondStack.size() > 1) {
                std::cout << "Whirlpool: false (invalid stack state)\n";
                return false;
            }
            Position emptySpace = {firstPosition.first,
                                   (firstPosition.second + secondPosition.second) / 2};
            if (!board[emptySpace].empty()) {
                std::cout << "Whirlpool: false (empty space not empty)\n";
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
            m_Board.CheckIsLocked();
            std::cout << "Whirlpool: true\n";
            return true;
        }
        case ElementIndexPower::Tsunami: {
            auto hasEmptySpot = [&] {
                for (const auto &stack : board | std::views::values) {
                    if (stack.empty()) {
                        return true;
                    }
                }
                return false;
            };
            if (!hasEmptySpot()) {
                std::cout << "Tsunami: false (no empty spot)\n";
                return false;
            }
            if (playerTurn == PlayerTurn::Player1) {
                m_RowPlayer2 = firstPosition.first;
            } else {
                m_RowPlayer1 = firstPosition.first;
            }
            std::cout << "Tsunami: true\n";
            return true;
        }
        case ElementIndexPower::Waterfall: {
            const auto &[leftX, leftY]   = m_Board.GetLeft();
            const auto &[rightX, rightY] = m_Board.GetRight();
            const auto row               = firstPosition.first;
            int        cardCount         = 0;
            for (auto i = leftY; i <= rightY; ++i) {
                if (!board[{row, i}].empty()) {
                    ++cardCount;
                    std::cout << "Card value: " << board[{row, i}].top().GetValue() << '\n';
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
                std::cout << "Waterfall: true\n";
                return true;
            }
            std::cout << "Count: " << cardCount << " Waterfall: false (card count < 3)\n";
            return false;
        }
        case ElementIndexPower::Support: {
            if (board[firstPosition].empty()) {
                std::cout << "Support: false (empty stack)\n";
                return false;
            }
            if (auto &cardOnTop = board[firstPosition].top();
                cardOnTop.GetPlacedBy() == playerTurn && cardOnTop.GetValue() >= 1 &&
                cardOnTop.GetValue() <= 3) {
                // todo: see if that != playerTurn was somehow right (doubt it)
                cardOnTop.SetModifier(1);
                std::cout << "Support: true\n";
                return true;
            }
            std::cout << "Support: false (conditions not met)\n";
            return false;
        }
        case ElementIndexPower::Earthquake: {
            auto  newBoard     = RemadeGameBoard(m_Board);
            auto &newGameBoard = newBoard.GetGameBoard();
            for (auto it = newGameBoard.begin(); it != newGameBoard.end();) {
                if (auto &stack = it->second; !stack.empty() && stack.top().GetValue() == 1) {
                    stack.pop();
                    if (stack.empty()) {
                        it = newGameBoard.erase(it);
                        continue;
                    }
                }
                ++it;
            }
            if (const auto &remadeNewBoard = RemadeGameBoard(newBoard);
                remadeNewBoard.GetMaxBoardSize() != 0) {
                m_Board = remadeNewBoard;
                m_Board.UpdateDiagonals();
                m_Board.CheckIsLocked();
                std::cout << "Earthquake: true\n";
                return true;
            }
            std::cout << "Earthquake: false (invalid board size)\n";
            return false;
        }
        case ElementIndexPower::Shattering: {
            if (board[firstPosition].empty()) {
                std::cout << "Shattering: false (empty stack)\n";
                return false;
            }
            if (auto &cardOnTop = board[firstPosition].top();
                cardOnTop.GetPlacedBy() != playerTurn && 2 <= cardOnTop.GetValue() &&
                cardOnTop.GetValue() <= 4) {
                cardOnTop.SetModifier(-1);
                std::cout << "Shattering: true\n";
                return true;
            }
            std::cout << "Shattering: false (conditions not met)\n";
            return false;
        }
        case ElementIndexPower::Granite: {
            auto newBoard = RemadeGameBoard(m_Board);
            Card graniteCard{};
            graniteCard.SetIsGranite(true);
            if (newBoard.InsertCard(card, firstPosition, PlayerTurn::Granite, CardType::Granite,
                                    *this) != InsertOutputs::Success) {
                std::cout << "Granite: false (insert failed)\n";
                return false;
            }
            newBoard.CheckIsLocked();
            if (newBoard.UpdateCorners(firstPosition) && newBoard.IsBoardLocked()) {
                m_Board = newBoard;
                m_Board.UpdateDiagonals();
                std::cout << "Granite: true\n";
                return true;
            }
            std::cout << "Granite: false (conditions not met)\n";
            return false;
        }
        case ElementIndexPower::Avalanche: {
            if (std::abs(firstPosition.first - secondPosition.first) > 1 ||
                std::abs(firstPosition.second - secondPosition.second) > 1) {
                std::cout << "Avalanche: false (invalid positions)\n";
                std::cout << "First position: (" << firstPosition.first << ", "
                          << firstPosition.second << ")\n";
                std::cout << "Second position: (" << secondPosition.first << ", "
                          << secondPosition.second << ")\n";
                return false;
            }

            auto shiftStack = [&](const Position &from, const Position &to) {
                while (!board[from].empty()) {
                    board[to].push(board[from].top());
                    board[from].pop();
                }
                if (board[from].empty()) {
                    board.erase(from);
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
                    std::cout << "1 Avalanche: false (no empty space)\n";
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
                    std::cout << "2 Avalanche: false (no empty space)\n";
                    return false;
                }
            }

            m_Board.UpdateDiagonals();
            m_Board.CheckIsLocked();
            return true;
        }
        case ElementIndexPower::Boulder: {
            if (!m_IllusionEnabled) {
                std::cout << "Boulder: false (illusion not enabled)\n";
                return false;
            }
            if (!board[firstPosition].empty() and board[firstPosition].top().GetIsIllusion()) {
                board[firstPosition].pop();
                board[firstPosition].emplace(card);

                std::cout << "Boulder: true\n";

                return true;
            }

            std::cout << "Boulder: false (conditions not met)\n";

            return false;
        }
        default:
            return "Unknown power.";
    }
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

int Game::GetRowPlayer1() const { return m_RowPlayer1; }
int Game::GetRowPlayer2() const { return m_RowPlayer2; }

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
    if (card.GetIsGranite())
        return CardType::Granite;
    if (card.GetIsHole())
        return CardType::Hole;

    return CardType::Normal;
}

Board Game::RemadeGameBoard(Board board) {
    Board modifiedBoard{board.GetMaxBoardSize()};

    for (auto [position, stack] : board.GetGameBoard()) {
        std::stack<Card> tempStack;

        while (!stack.empty()) {
            tempStack.push(stack.top());
            stack.pop();
        }

        while (!tempStack.empty()) {
            const auto &card = tempStack.top();
            tempStack.pop();

            std::cout << "[RMD BOARD]\nCard: " << card.GetValue()
                      << "\nIs Hole: " << card.GetIsHole() << "\nPosition: (" << position.first
                      << ", " << position.second << ")\n\n";

            const auto result = modifiedBoard.InsertCard(card, position, card.GetPlacedBy(),
                                                         GetCardType(card), *this, true);

            if (!card.GetIsHole()) {
                modifiedBoard.UpdateCorners(position);
            }
            if (result != InsertOutputs::Success) {
                std::cout << "Invalid move\n";

                return Board{0};
            }
        }
    }

    return modifiedBoard;
}

Explosion Game::GenerateExplosion() const {
    return Explosion::Generate(m_Board.GetMaxBoardSize(), m_Board.GetLeft(), m_Board.GetUp(),
                               m_Board.GetDown(), m_Board.GetRight());
}

bool Game::ApplyExplosion(const Explosion &explosion) {
    std::cout << "Applying explosion\n";
    auto  newBoard     = RemadeGameBoard(m_Board);
    auto &newGameBoard = newBoard.GetGameBoard();

    for (const auto &[position, effect] : explosion.GetEffects()) {
        switch (effect) {
            case Explosion::Effect::Eliminate: {
                newGameBoard.erase(position);
                break;
            }
            case Explosion::Effect::Return: {
                auto stack = newGameBoard[position];

                newGameBoard.erase(position);

                // todo: return the shit to the players

                break;
            }
            case Explosion::Effect::Hole: {
                auto &stack = newGameBoard[position];

                while (!stack.empty()) {
                    stack.pop();
                }

                Card hole{1};
                hole.SetIsHole(true);
                hole.SetPlacedBy(PlayerTurn::Granite);
                // ^this is gonna bite us in the ass later on 103%

                stack.emplace(hole);
                break;
            }
            default: {
                break;
            }
        }
    }

    if (const auto &remadeNewBoard = RemadeGameBoard(newBoard);
        remadeNewBoard.GetMaxBoardSize() != 0) {
        m_Board = remadeNewBoard;

        m_Board.UpdateDiagonals();
        m_Board.CheckIsLocked();

        // std::cout << "Board up size: " << m_Board.GetGameBoard().at(m_Board.GetUp()).size() <<
        // '\n';

        std::cout << "[expl] is board locked?: " << m_Board.IsBoardLocked() << '\n';

        std::cout << "Explosion applied\n";

        m_ExplosionUsed = true;

        return true;
    }

    std::cout << "Explosion not applied\n";

    return false;
}

bool Game::CheckExplosion() { return GetCurrentPlayer().GetHasExplosion() && !m_ExplosionUsed; }

bool Game::GetIllusionEnabled() const { return m_IllusionEnabled; }
bool Game::GetEterEnabled() const { return m_EterEnabled; }
bool Game::ExplosionEnabled() const { return m_ExplosionEnabled; }

void Game::SetRowPlayer1(const int row) { m_RowPlayer1 = row; }

void Game::SetRowPlayer2(const int row) { m_RowPlayer2 = row; }

std::pair<Position, Card> Game::RecommendMove() {
    Player &currentPlayer  = (m_PlayerTurn == PlayerTurn::Player1) ? m_Player1 : m_Player2;
    Player &opponentPlayer = (m_PlayerTurn == PlayerTurn::Player1) ? m_Player2 : m_Player1;

    // Check if we can win in the next move
    for (const auto &card : currentPlayer.GetHand()) {
        if (CanWinNextMove(currentPlayer, card)) {
            return {GetWinningPosition(currentPlayer, card), card};
        }
    }

    // Check if the opponent can win in the next move
    for (const auto &card : currentPlayer.GetHand()) {
        if (CanOpponentWinNextMove(opponentPlayer, card)) {
            return {GetBlockingPosition(opponentPlayer, card), card};
        }
    }

    // Prioritize placing cards that would make another card win in the next move
    for (const auto &card : currentPlayer.GetHand()) {
        if (CanSetupNextMoveWin(currentPlayer, card)) {
            return {GetSetupPosition(currentPlayer, card), card};
        }
    }

    // Prioritize having as many face cards up as possible
    for (const auto &card : currentPlayer.GetHand()) {
        if (CanPlaceOverOpponentCard(currentPlayer, card)) {
            return {GetPlacementPosition(currentPlayer, card), card};
        }
    }

    // Default move if no other conditions are met
    return {GetDefaultPosition(), currentPlayer.GetHand().front()};
}

bool Game::CanWinNextMove(const Player &player, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        if (newBoard.CheckLineWin(board, position)) {
            return true;
        }

        if (newBoard.CheckColumnWin(board, position)) {
            return true;
        }

        if (newBoard.CheckDiagonalWin(board, position)) {
            return true;
        }
    }
    return false;
}

bool Game::CanOpponentWinNextMove(const Player &opponent, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        if (newBoard.CheckLineWin(board, position)) {
            return true;
        }

        if (newBoard.CheckColumnWin(board, position)) {
            return true;
        }

        if (newBoard.CheckDiagonalWin(board, position)) {
            return true;
        }
    }
    return false;
}

bool Game::CanSetupNextMoveWin(const Player &player, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        for (const auto &nextCard : player.GetHand()) {
            if (nextCard == card) {
                continue;
            }

            stack.emplace(nextCard);

            if (newBoard.CheckLineWin(board, position) ||
                newBoard.CheckColumnWin(board, position) ||
                newBoard.CheckDiagonalWin(board, position)) {
                return true;
            }

            stack.pop();
        }

        stack.pop();
    }
    return false;
}

bool Game::CanPlaceOverOpponentCard(const Player &player, const Card &card) {
    auto &board = m_Board.GetGameBoard();
    for (const auto &stack : board | std::views::values) {
        if (!stack.empty() && stack.top().GetPlacedBy() != m_PlayerTurn &&
            card.GetValue() > stack.top().GetValue()) {
            return true;
        }
    }
    return false;
}

Position Game::GetWinningPosition(const Player &player, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        if (newBoard.CheckLineWin(board, position) || newBoard.CheckColumnWin(board, position) ||
            newBoard.CheckDiagonalWin(board, position)) {
            return position;
        }

        stack.pop();
    }
    return Position{0, 0};
}

Position Game::GetBlockingPosition(const Player &opponent, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        if (newBoard.CheckLineWin(board, position) || newBoard.CheckColumnWin(board, position) ||
            newBoard.CheckDiagonalWin(board, position)) {
            return position;
        }

        stack.pop();
    }
    return Position{0, 0};
}

Position Game::GetSetupPosition(const Player &player, const Card &card) {
    auto  newBoard = RemadeGameBoard(m_Board);
    auto &board    = newBoard.GetGameBoard();
    for (auto &[position, stack] : board) {
        if (stack.empty() || stack.top().GetPlacedBy() != m_PlayerTurn) {
            continue;
        }

        stack.emplace(card);

        for (const auto &nextCard : player.GetHand()) {
            if (nextCard == card) {
                continue;
            }

            stack.emplace(nextCard);

            if (newBoard.CheckLineWin(board, position) ||
                newBoard.CheckColumnWin(board, position) ||
                newBoard.CheckDiagonalWin(board, position)) {
                return position;
            }

            stack.pop();
        }

        stack.pop();
    }
    return Position{0, 0};
}

Position Game::GetPlacementPosition(const Player &player, const Card &card) {
    auto &board = m_Board.GetGameBoard();
    for (const auto &[position, stack] : board) {
        if (!stack.empty() && stack.top().GetPlacedBy() != m_PlayerTurn &&
            card.GetValue() > stack.top().GetValue()) {
            return position;
        }
    }
    return Position{0, 0};
}

Position Game::GetDefaultPosition() {
    auto &board = m_Board.GetGameBoard();
    for (const auto &[position, stack] : board) {
        if (stack.empty()) {
            return position;
        }
    }
    return Position{0, 0};
}

void Game::SaveGameState() {
    // nlohmann::json j;
    // to_json(j, *this);
    // std::ofstream file(JSON_FILE_NAME);
    // if (file.is_open()) {
    //     file << j.dump(4); // Indent with 4 spaces for readability
    //     file.close();
    // } else {
    //     std::cerr << "Unable to open file for saving game state\n";
    // }
}

// void Game::to_json(nlohmann::json &j, const Game &game) {
//     j = nlohmann::json{
//             {"boardSize", (static_cast<Game>(game).GetBoard().GetMaxBoardSize())},
//             {"scoreToWin", game.GetScoreToWin()},
//             {"player1", static_cast<Game>(game).GetPlayer1()},
//             {"player2", static_cast<Game>(game).GetPlayer2()},
//             {"options",
//              {game.GetEterEnabled(), game.GetIllusionEnabled(), game.ExplosionEnabled()}},
//             {"currentPlayer", game.GetCurrentPlayerTurn()},
//             {"scorePlayer1", game.GetPlayer1Score()},
//             {"scorePlayer2", game.GetPlayer2Score()},
//             {"lastPositionPlayer1", static_cast<Game>(game).GetLastCardPlayer1()},
//             {"lastPositionPlayer2", static_cast<Game>(game).GetLastCardPlayer2()},
//             {"rowPlayer1", game.GetRowPlayer1()},
//             {"rowPlayer2", game.GetRowPlayer2()}};
// }
//
// void Game::from_json(const nlohmann::json &j, Game &game) {
//     int                 boardSize, scoreToWin;
//     std::string         nameOne, nameTwo;
//     std::array<bool, 3> options;
//     PlayerTurn          currentPlayer;
//     int                 scorePlayer1, scorePlayer2;
//     Position            lastPositionPlayer1, lastPositionPlayer2;
//     int                 rowPlayer1, rowPlayer2;
//
//     j.at("boardSize").get_to(boardSize);
//     j.at("scoreToWin").get_to(scoreToWin);
//     j.at("player1").get_to(nameOne);
//     j.at("player2").get_to(nameTwo);
//     j.at("options").get_to(options);
//     j.at("currentPlayer").get_to(currentPlayer);
//     j.at("scorePlayer1").get_to(scorePlayer1);
//     j.at("scorePlayer2").get_to(scorePlayer2);
//     j.at("lastPositionPlayer1").get_to(lastPositionPlayer1);
//     j.at("lastPositionPlayer2").get_to(lastPositionPlayer2);
//     j.at("rowPlayer1").get_to(rowPlayer1);
//     j.at("rowPlayer2").get_to(rowPlayer2);
//
//     game = Game(boardSize, scoreToWin, nameOne, nameTwo, options);
//     game.SetNextPlayerTurn(currentPlayer);
//     game.IncreasePlayerScore(PlayerTurn::Player1);
//     game.IncreasePlayerScore(PlayerTurn::Player2);
//     game.SetLastCardPlayer1(lastPositionPlayer1);
//     game.SetLastCardPlayer2(lastPositionPlayer2);
//     game.SetRowPlayer1(rowPlayer1);
//     game.SetRowPlayer2(rowPlayer2);
// }
