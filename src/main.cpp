#include <iostream>

#include "./Game/Game.h"
#include "Card.h"
#include "Player.h"

int main() {
    std::cout << "Dupa 2 ani a compilat si varu\n";

    return 0;
}

/*
Eter Game

We start with a 1x1 matrix where the first player places their card of choice
The second player then places their card of choice anywhere adjacent to the
first card

This continues until a 3x3 matrix is formed and we lock the matrix in place

The first player to get 3 in a row wins
If there's a tie, the game is won by who placed the highest sum of cards


-- The matrix is locked once we found 2 cards that are theoretically diagonally
opposite
*/

// This'll be a purely virtual class, it'll later be inherited by the other
// special cards
// hole card for explosion card
// class Card {
// uint8_t value;
/*
Value: between 1 and 4
Movable: "eter" cards are unmovable
Hidden: placed face down, opponent can't see the value, can try placing a card
      if oponent card is equal or lower, the opponent's card is lost and he
      wastes the turn
 */
// };

// class Player {

/*
current game mode an uint4_t
cardsOnHand std::vector of Card (will start with X default cards or W/E)

specialCards same stuff as above, but with the Hidden and Eter card (+
other I haven't botherd reading about)

score: uint8_t -> sum of all placed cards

we may move the stuff below to a different class / game class

isAi: bool -> true if AI, false if human
difficulty: enum -> easy, medium, hard, impossible
safetySwitch: bool -> if true, if the game is lost by the human player
their PC will BSOD / Kernel Panic
*/
//};

// class gameBoard {};

/*
        Easy -> gready algorithm
        Medium -> dynamic programming
        Hard -> AI / machine learning
        Impossible -> know what cards you have on hand +
                        AI / machine learning
*/

// this class will be pure virtual, it'll be inherited by the other special
// gamemode classes
// class gameBoard -> things about adding cards, checking if the matrix is full,
// etc...
// class Game {
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
// };
