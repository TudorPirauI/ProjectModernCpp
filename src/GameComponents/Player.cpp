#include "GameComponents/Player.h"

Player::Player(std::string name, Hand hand) :
    m_UserName{std::move(name)}, m_Score{0}, m_Hand{std::move(hand)} {}

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

int Player::GetIllusion() const { return m_NumberOfIllusion; }

void Player::SetIllusion(const int value) { m_NumberOfIllusion = value; }

bool Player::GetHasIllusionInGame() const { return m_HasIllusionInGame; }
void Player::SetHasIllusionInGame(const bool value) { m_HasIllusionInGame = value; }

bool Player::GetHasExplosion() const { return m_HasExplosion; }
void Player::SetHasExplosion(const bool value) { m_HasExplosion = value; }

void Player::AddToRemovedCards(const Card &card) {
    if (card.GetModifier() != 0) {
        auto modifiedCard = card;

        modifiedCard.SetModifier(0);

        m_RemovedCards.emplace_back(modifiedCard);
    }
    m_RemovedCards.emplace_back(card);
}

Hand &Player::GetRemovedCards() { return m_RemovedCards; }

void Player::RemoveFromRemovedCards(const Card &card) {
    if (const auto it = std::ranges::find(m_RemovedCards, card); it != m_RemovedCards.end()) {
        m_RemovedCards.erase(it);
    }
}

void Player::SetUserName(const std::string &userName) { m_UserName = userName; }

void Player::SetScore(int score) { m_Score = score; }

void Player::SetRemovedCards(const Hand &removedCards) { m_RemovedCards = removedCards; }

// void to_json(nlohmann::json &j, const Card &card) {
//     j = nlohmann::json{{"value", card.GetValue()},
//                        {"isEter", card.GetIsEter()},
//                        {"isIllusion", card.GetIsIllusion()},
//                        {"placedBy", card.GetPlacedBy()},
//                        {"modifier", card.GetModifier()}};
// }
//
// void from_json(const nlohmann::json &j, Card &card) {
//     int        value;
//     bool       isEter, isIllusion;
//     PlayerTurn placedBy;
//     int        modifier;
//
//     j.at("value").get_to(value);
//     j.at("isEter").get_to(isEter);
//     j.at("isIllusion").get_to(isIllusion);
//     j.at("placedBy").get_to(placedBy);
//     j.at("modifier").get_to(modifier);
//
//     card.SetValue(value);
//     card.SetEter(isEter);
//     card.SetIllusion(isIllusion);
//     card.SetPlacedBy(placedBy);
//     card.SetModifier(modifier);
// }
//
// // Define to_json for Player
// void to_json(nlohmann::json &j, const Player &p) {
//     j = nlohmann::json{{"name", p.GetUserName()},
//                        {"score", p.GetScore()},
//                        {"hand", p.GetHand()},
//                        {"removedCards", p.GetRemovedCards()},
//                        {"illusion", p.GetIllusion()},
//                        {"hasIllusion", p.GetHasIllusionInGame()},
//                        {"hasExplosion", p.GetHasExplosion()}};
// }
//
// // Define from_json for Player
// void Player::from_json(const nlohmann::json &j, Player &p) {
//     j.at("name").get_to(p.m_UserName);
//     j.at("score").get_to(p.m_Score);
//     j.at("hand").get_to(p.m_Hand);
//     j.at("removedCards").get_to(p.m_RemovedCards);
//     j.at("illusion").get_to(p.m_NumberOfIllusion);
//     j.at("hasIllusion").get_to(p.m_HasIllusionInGame);
//     j.at("hasExplosion").get_to(p.m_HasExplosion);
// }
//
// // Define to_json for Hand
// void to_json(nlohmann::json &j, const Hand &hand) {
//     j = nlohmann::json::array();
//     for (const auto &card : hand) {
//         j.push_back(card);
//     }
// }
//
// // Define from_json for Hand
// void from_json(const nlohmann::json &j, Hand &hand) {
//     hand.clear();
//     for (const auto &item : j) {
//         hand.push_back(item.get<Card>());
//     }
// }
