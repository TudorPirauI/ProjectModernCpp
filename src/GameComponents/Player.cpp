#include "GameComponents/Player.h"
#include "pch.h"

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

void Player::GiveCard(const Card &card) {
    if (card.GetModifier() != 0) {
        auto modifiedCard = card;

        modifiedCard.SetModifier(0);

        m_Hand.emplace_back(modifiedCard);
    }
    m_Hand.emplace_back(card);
}

void Player::RemoveCard(const Card &card) {
    if (const auto it = std::ranges::find(m_Hand, card); it != m_Hand.end()) {
        m_Hand.erase(it);
    }
}

void Player::GiveEterCard(const PlayerTurn playerTurn) {
    Card card{1, playerTurn};
    card.SetEter(true);
    m_Hand.emplace_back(card);
}

int Player::GetIllusion() { return m_NumberOfIllusion; }

void Player::SetIllusion(const int value) { m_NumberOfIllusion = value; }

void Player::AddToRemovedCards(const Card &card) {
    if (card.GetModifier() != 0) {
        auto modifiedCard = card;

        modifiedCard.SetModifier(0);

        m_RemovedCards.emplace_back(modifiedCard);
    }
    m_RemovedCards.emplace_back(card);
}

Hand Player::GetRemovedCards() { return m_RemovedCards; }

void Player::RemoveFromRemovedCards(const Card &card) {
    if (const auto it = std::ranges::find(m_RemovedCards, card); it != m_RemovedCards.end()) {
        m_RemovedCards.erase(it);
    }
}
