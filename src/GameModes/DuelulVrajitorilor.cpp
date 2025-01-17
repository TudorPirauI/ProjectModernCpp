#include "GameModes/DuelulVrajitorilor.h"

const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    std::vector<Card> cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

void DuelulVrajitorilor::SetNewCards() {
    m_Player1.SetHand(
            CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1)); // Creează cărțile direct aici
    m_Player2.SetHand(CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2));

    Card cardEter(1, PlayerTurn::Player1); // Inițializare directă cu constructor
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);

    cardEter.SetPlacedBy(PlayerTurn::Player2);
    m_Player2.GiveCard(cardEter);

    m_Board.CleanUpBoard();
}

Wizard DuelulVrajitorilor::GetPlayerAbility1() { return m_AbilityPlayer1; }

Wizard DuelulVrajitorilor::GetPlayerAbility2() { return m_AbilityPlayer2; }

DuelulVrajitorilor::DuelulVrajitorilor(const std::string &nameOne, const std::string &nameTwo,
                                       const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options) {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_AbilityPlayer1.ResetPowerForNewMatch();
    m_AbilityPlayer2.ResetPowerForNewMatch();

    Card cardEter;

    cardEter.SetValue(1);
    cardEter.SetEter(true);
    cardEter.SetPlacedBy(PlayerTurn::Player1);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);
    m_Player2.GiveCard(cardEter);
}
