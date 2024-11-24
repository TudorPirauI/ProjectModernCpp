//
// Created by mavri on 24.11.2024.
//
#include <catch2/catch_all.hpp>
#include "../src/Card/Card.h"

TEST_CASE("Card should be created with value 1", "[Card]") {
    const Card card(1);
    REQUIRE(card.GetValue() == 1);
}

TEST_CASE("Card should not allow values less than 1", "[Card]") { REQUIRE_THROWS_AS(Card(0), std::invalid_argument); }
TEST_CASE("Card should not allow values greater than 4"
          "[Card]") {
    REQUIRE_THROWS_AS(Card(5), std::invalid_argument);
}
