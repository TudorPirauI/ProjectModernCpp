//
// Created by mavri on 18.11.2024.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <iostream>

#include "../src/Card/Card.h"
#include "../src/GameBoard/Board.h"

TEST_CASE("Card should be inserted successfully", "[Board]") {
    Board board{3};

    const Card cardOne(1);

    REQUIRE(board.InsertCard(cardOne, {0, 0}, PlayerTurn::Player1) == true);
}

TEST_CASE("Board should be locked", "[Board]") {
    Board board{3};

    const Card cardOne(1);
    const Card cardTwo(2);
    Card       cardThree(3);
    const Card cardFour(4);
    Card       cardFive(1);

    // cardFive.SetEter(true);
    // cardThree.SetIllusion(true);
    // cardThree.SetIsFlipped(true);

    const std::vector cards = {cardOne, cardTwo, cardThree, cardFour, cardFive};

    const std::vector<Position> positions = {{0, 0}, {0, 1}, {-1, 1}, {-2, 1}, {0, 2}};

    for (auto i = 0; i < cards.size(); ++i) {
        const auto res = board.InsertCard(cards[i], positions[i], PlayerTurn::Player1);

        REQUIRE(res == true);
    }

    for (const auto [fst, snd] : board.GetGameBoard()) {
        const auto card = snd.top();
        std::cout << std::format(
                "(x: {}, y: {}) -> Card {} (Illusion: {} | Flipped: {} | Eter: {})\n", fst.first,
                fst.second, card.GetValue(), card.GetIsIllusion(), card.GetIsFlipped(),
                card.GetIsEter());
    }

    REQUIRE(board.IsBoardLocked() == true);
}
