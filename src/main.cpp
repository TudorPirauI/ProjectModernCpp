#include <iostream>
#include "Card.h"
#include "Board/GameBoard.h"
#include "Player.h"
#include "Game/Antrenament.h"

int main() {
    std::cout << "Eter Game - Step 1: Initial Setup\n";

    uint8_t rows = 1, cols = 1;
    GameBoard gameBoard(rows, cols);

    Player playerOne;
    playerOne.setAntrenamentCards();
    Player playerTwo;
    playerTwo.setAntrenamentCards();

    std::cout << "Player One Cards:\n";
    playerOne.showCards();

    std::cout << "Player Two Cards:\n";
    playerTwo.showCards();

    Antrenament game(playerOne, playerTwo);

    game.StartGame();

    game.NewTurn();
    game.NewTurn();
    game.NewTurn();

    auto winner = game.EndGame();
    if (winner) {
        std::cout << "The winner is: " << winner->getName() << std::endl;
    } else {
        std::cout << "Game is still ongoing." << std::endl;
    }

    return 0;
}
