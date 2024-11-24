//
// Created by mavri on 18.11.2024.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../src/Card/Card.h"
#include "../src/GameBoard/Board.h"

TEST_CASE("Card should be inserted successfully", "[Board]") {
    Board board;

    const Card cardOne(1);

    REQUIRE(board.InsertCard(cardOne, {0, 0}) == true);
}

TEST_CASE("Board should be locked", "[Board]") {
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

    for (auto i = 0; i < cards.size(); ++i) {
        const auto res = board.InsertCard(cards[i], positions[i]);

        REQUIRE(res == true);
    }

    REQUIRE(board.IsBoardLocked() == true);
}
