#include "Player.h"

#include <algorithm>
#include <utility>

Player::Player(std::string name, Hand hand) : m_UserName{std::move(name)}, m_Score{0}, m_Hand{std::move(hand)} {}

Player::~Player() = default;

const Hand &Player::getHand() const { return m_Hand; }

const std::string &Player::getUserName() const { return m_UserName; }

int Player::getScore() const { return m_Score; }

// void Player::playCard(const Card &card) {
//     const auto it = std::ranges::find(m_Hand, card);
//     if (it != m_Hand.end()) {
//         m_Hand.erase(it);
//     }
// }

void Player::giveCard(const Card &card) { m_Hand.emplace_back(card); }
