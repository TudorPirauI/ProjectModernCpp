#include "GameModes/Antrenament.h"

// Modul antrenament: tabla este de 3x3; fiecare jucător are 7 cărți: 1, 1, 2, 2, 3, 3, 4; Format
// meci: 2/3;

// todo: move this to somewhere more permanent
const auto CREATE_CARDS = [](const std::array<int, 7> &values, const PlayerTurn &player) {
    std::vector<Card> cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

Antrenament::Antrenament(const std::string &nameOne, const std::string &nameTwo) :
    Game(3, 2, nameOne, nameTwo) {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);
}

void Antrenament::SetNewCards() {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_Board.CleanUpBoard();
}
