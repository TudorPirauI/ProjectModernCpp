#include "Antrenament.h"

Antrenament::Antrenament(Player player1, Player player2) {
    this->m_player1 = player1;
    this->m_player2 = player2;
    this->m_mode    = GameMode::Antrenament;
}

void Antrenament::StartGame() {
    SetCards();
    m_turn = false;
    NewTurn();
}

std::optional<Player> Antrenament::EndGame() {
    if (m_pointsPlayer1 == 3) {
        return m_player1;
    } else if (m_pointsPlayer2 == 3) {
        return m_player2;
    } else {
        return std::nullopt;
    }
}

void Antrenament::SetCards() {
    Card card1(CardType::Normal, 1, false);
    Card card2(CardType::Normal, 2, false);
    Card card3(CardType::Normal, 3, false);
    Card card4(CardType::Normal, 4, false);

    for (int i = 0; i < 2; ++i) {
        m_player1.addCard(card1);
        m_player1.addCard(card2);
        m_player1.addCard(card3);

        m_player2.addCard(card1);
        m_player2.addCard(card2);
        m_player2.addCard(card3);
    }

    m_player1.addCard(card4);
    m_player2.addCard(card4);
}

void Antrenament::DecideWinner() {}

void Antrenament::NewTurn() {}
