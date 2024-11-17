#include <iostream>
#include <vector>

#include "Card/Card.h"
#include "GameBoard/Board.h"

int main() {
    Board board;

    const Card cardOne(1);
    const Card cardTwo(2);
    Card       cardThree(3);
    const Card cardFour(4);
    Card       cardFive(1);

    cardFive.SetEter(true);
    cardThree.SetIllusion(true);
    cardThree.SetIsFlipped(true);

    const std::vector cards = {cardOne, cardTwo, cardThree, cardFour, cardFive};

    const std::vector<Position> positions = {{0, 0}, {0, 1}, {-1, 1}, {-2, 1}, {0, 2}};

    for (size_t i = 0; i < cards.size(); ++i) {
        const auto res = board.InsertCard(cards[i], positions[i]);

        if (!res) {
            std::cout << "Failed to insert card " << i + 1 << std::endl;
        }
        board.PrintTable();
    }


    return 0;
}
