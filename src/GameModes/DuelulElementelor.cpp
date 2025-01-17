//
// Created by Miruna on 1/16/2025.
//

#include "GameModes/DuelulElementelor.h"
#include <array>

// Funcție pentru crearea setului de cărți inițial
const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    Hand cards; // Presupunând că Hand este definit ca std::vector<Card> sau similar
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

DuelulElementelor &DuelulElementelor::operator=(DuelulElementelor other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void DuelulElementelor::SetNewCards() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
    m_Board.CleanUpBoard();
}

ElementPower DuelulElementelor::GetPlayerAbility1() const { return m_AbilityPlayer1; }

ElementPower DuelulElementelor::GetPlayerAbility2() const { return m_AbilityPlayer2; }

DuelulElementelor::DuelulElementelor(const std::string &nameOne, const std::string &nameTwo,
                                     const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options), m_AbilityPlayer1(), m_AbilityPlayer2() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_AbilityPlayer1.RandomPower();
    m_AbilityPlayer2.RandomPower();

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
}
