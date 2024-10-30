//
// Created by edi on 30.10.2024.
//


//class Game {
    /*
  gameBoard: std::vector<std::vector<std::vector<Card*>>> 3x3 matrix (could also
  be 4x4 depending on gamemode) potentially making the last vector
  std::stack<Card*>

  isLocked: bool -> if we found a 3x3/4x4 matrix

  gameMode: Enum with all available gamemodes

  playerOne: Player
  playerTwo: Player

  turn: bool -> true if playerOne's turn, false if playerTwo's turn

  isFull(): bool -> returns true if the matrix is full with cards

  willBecomeIsolated(effect: Effect, int row/column) TBD, based on special cards
    */
//};

#include <stack>
#include <vector>
#include <cstdint>

import Card;
import Player;

export module Game;


export class Game {
public:
    virtual ~Game() = default;

    enum class GameMode : uint8_t {
        Antrenament,
        DuelulVrajitorilor,
        DuelulElementelor,
        Turneu,
        Viteza
    };

    // Game will be a virtual class btw
    // this function will be implemented in the future class ;)

    virtual void DecideWinner() = 0;
    Game(Player player1, Player player2, GameMode mode);
    void DecideTurn();
    void SetPlayer1(Player player);
    void SetPlayer2(Player player);
    Player getPlayer1();
    Player getPlayer2();

    private:
        // switch later to object from class gameBoard :)
        std::vector<std::vector<std::stack<Card>>> m_gameBoard;
        GameMode m_mode;
        Player m_player1;
        Player m_player2;
        bool m_gameOver;
        bool m_turn;
};

