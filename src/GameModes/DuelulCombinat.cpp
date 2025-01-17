//
// Created by Miruna on 1/16/2025.
//

#include "GameModes/DuelulCombinat.h"
#include <array>
#include "GameComponents/Card.h"
#include "GameComponents/Player.h"

const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    Hand cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

DuelulCombinat &DuelulCombinat::operator=(DuelulCombinat other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void DuelulCombinat::SetNewCards() {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
    m_Board.CleanUpBoard();
}

Wizard DuelulCombinat::GetWizardAbility() { return m_WizardAbility1; }

ElementPower DuelulCombinat::GetElementalAbility() { return m_ElementalAbility1; }

DuelulCombinat::DuelulCombinat(const std::string &nameOne, const std::string &nameTwo,
                               const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options), m_WizardAbility1(), m_ElementalAbility1() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_WizardAbility1.RandomPower();
    m_ElementalAbility1.RandomPower();

    m_WizardAbility2.RandomPower();
    m_ElementalAbility2.RandomPower();

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
}
