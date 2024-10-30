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

export module Game;

import Player;
import Card;

export class Game {
    private:
        // switch later to object from class gameBoard :)
        std::vector<std::vector<std::stack<Card>>> m_gameBoard;
        Player m_player1;
        Player m_player2;
        bool m_gameOver;
        bool m_turn;
    public:

};

