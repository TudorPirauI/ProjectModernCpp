#include "Player.h"

#include <algorithm>
#include <iostream>
#include <utility>

Player::Player(std::string name, Hand hand) :
    m_UserName{std::move(name)}, m_Score{0}, m_Hand{std::move(hand)} {}

Player::~Player() = default;

const Hand &Player::GetHand() const { return m_Hand; }

const std::string &Player::GetUserName() const { return m_UserName; }

int Player::GetScore() const { return m_Score; }

void Player::ShowCards() {
    for (const auto &card : m_Hand) {
        std::cout << card;
    }
    std::cout << '\n';
}

void Player::SetHand(const Hand &hand) { m_Hand = hand; }

void Player::GiveCard(const Card &card) { m_Hand.emplace_back(card); }

void Player::RemoveCard(const Card &card) {
    auto it = std::find(m_Hand.begin(), m_Hand.end(), card);
    if (it != m_Hand.end()) {
        m_Hand.erase(it);
    }
}
