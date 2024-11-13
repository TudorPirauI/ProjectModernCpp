// Player.cpp

// class Player {

/*
current game mode an uint8_t
cardsOnHand std::vector of Card (will start with X default cards or W/E)

specialCards same stuff as above, but with the Hidden and Eter card (+
other I haven't botherd reading about)

score: uint8_t -> sum of all placed cards

we may move the stuff below to a different class / game class

isAi: bool -> true if AI, false if human
difficulty: enum -> easy, medium, hard, impossible
safetySwitch: bool -> if true, if the game is lost by the human player
their PC will BSOD / Kernel Panic
*/
//};

#include <iostream>

#include "./Game/Game.h"
#include "Player.h"

Player::Player(uint8_t gameMode, const std::vector<Card> &cards) : m_gameMode(gameMode), m_cards(cards) {}

uint8_t Player::getGameMode() const { return m_gameMode; }

void Player::setGameMode(uint8_t gameMode) { m_gameMode = gameMode; }

uint8_t Player::getScore() const { return m_score; }

void Player::setScore(uint8_t score) { m_score = score; }

void Player::setAntrenamentCards() {
    Card card1(CardType::Normal, 1, false);
    Card card2(CardType::Normal, 2, false);
    Card card3(CardType::Normal, 3, false);
    Card card4(CardType::Normal, 4, false);

    for (int i = 0; i < 2; ++i) {
        m_cards.push_back(card1);
        m_cards.push_back(card2);
        m_cards.push_back(card3);
    }

    m_cards.push_back(card4);
}

void Player::setDuelulVrajitorilorCards() {
    Card card1(CardType::Normal, 1, false);
    Card card2(CardType::Normal, 2, false);
    Card card3(CardType::Normal, 3, false);
    Card card4(CardType::Normal, 4, false);

    for (int i = 0; i < 2; ++i) {
        m_cards.push_back(card1);
        m_cards.push_back(card2);
        m_cards.push_back(card3);
    }
    m_cards.push_back(card2);
    m_cards.push_back(card3);
    m_cards.push_back(card4);
}

void Player::setDuelulElementelorCards() {
    Card card1(CardType::Normal, 1, false);
    Card card2(CardType::Normal, 2, false);
    Card card3(CardType::Normal, 3, false);
    Card card4(CardType::Normal, 4, false);

    m_cards.push_back(card1);
    for (int i = 0; i < 3; ++i) {
        m_cards.push_back(card2);
        m_cards.push_back(card3);
    }
    m_cards.push_back(card4);
}


std::vector<Card> Player::getCards() const { return m_cards; }

void Player::showCards() const {
    for (size_t i = 0; i < m_cards.size(); ++i) {
        std::cout << "Card no." << i << " is ";
        m_cards[i].showCardInfo();
    }
}


void Player::calculateScore() {
    m_score = 0;
    for (const auto &card: m_cards) {
        m_score += card.getValue(); // de implementat getValue in Card
    }
}

// getCards

void Player::addCard(const Card &card) {
    m_cards.push_back(card);
    calculateScore();
}

void Player::playCard(size_t index) {
    if (index < m_cards.size()) {
        m_cards.erase(m_cards.begin() + index);
        calculateScore();
    } else {
        std::cout << "Invalid index!" << std::endl;
    }
}

void Player::makeMove() {
    if (m_isAi) {
        switch (m_difficulty) {
            case Difficulty::Easy:
                break;
            case Difficulty::Medium:
                break;
            case Difficulty::Hard:
            case Difficulty::Impossible:
                break;
        }
    }
}
