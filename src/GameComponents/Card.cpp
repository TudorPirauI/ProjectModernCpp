#include "GameComponents/Card.h"

Card::Card(const int value) {
    if (value > 4 || value < 1) {
        throw std::invalid_argument("Invalid card value");
    }

    m_Value = value;
}

Card::Card() = default;

Card::Card(const int value, const PlayerTurn playerTurn) : m_Value(value), m_PlacedBy(playerTurn) {}

bool Card::GetIsFlipped() const { return m_IsFlipped; }
void Card::SetIsFlipped(const bool isFlipped) { m_IsFlipped = isFlipped; }

bool Card::GetIsHole() const { return m_IsHole; }
void Card::SetIsHole(const bool isHole) { m_IsHole = isHole; }

PlayerTurn Card::GetPlacedBy() const { return m_PlacedBy; }
void       Card::SetPlacedBy(const PlayerTurn placedBy) { m_PlacedBy = placedBy; }

bool Card::GetIsGranite() const { return m_IsGranite; }
void Card::SetIsGranite(const bool isGranite) { m_IsGranite = isGranite; }

bool Card::operator==(const Card &other) const {
    return this->m_Value == other.m_Value && m_IsEter == other.m_IsEter &&
           m_IsIllusion == other.m_IsIllusion && m_IsGranite == other.m_IsGranite &&
           m_IsFlipped == other.m_IsFlipped && m_OnModifier == other.m_OnModifier &&
           m_PlacedBy == other.m_PlacedBy;
}

int Card::GetModifier() const { return m_OnModifier; }

void Card::SetModifier(const int value) { m_OnModifier = value; }

void Card::SetValue(const int value) { m_Value = value; }
int  Card::GetValue() const { return m_Value - m_OnModifier; }

void Card::SetIllusion(const bool isIllusion) { m_IsIllusion = isIllusion; }

bool Card::GetIsIllusion() const { return m_IsIllusion; }

void Card::SetEter(const bool isEter) { m_IsEter = isEter; }
bool Card::GetIsEter() const { return m_IsEter; }

std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << card.m_Value << ' ';

    if (card.m_IsEter) {
        os << "Is Eter\n";
    } else {
        os << '\n';
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const PlayerTurn &playerTurn) {
    switch (playerTurn) {
        case PlayerTurn::Player1:
            os << "Player1";
            break;
        case PlayerTurn::Player2:
            os << "Player2";
            break;
        case PlayerTurn::Granite:
            os << "Granite";
            break;
        default:
            os << "Unknown";
    }
    return os;
}

// void Card::to_json(nlohmann::json &j, const Card &c) {
//     j = nlohmann::json{{"value", c.m_Value},          {"placedBy", c.m_PlacedBy},
//                        {"isFlipped", c.m_IsFlipped},  {"isGranite", c.m_IsGranite},
//                        {"isEter", c.m_IsEter},        {"isIllusion", c.m_IsIllusion},
//                        {"onModifier", c.m_OnModifier}};
// }
//
// void Card::from_json(const nlohmann::json &j, Card &c) {
//     j.at("value").get_to(c.m_Value);
//
//     PlayerTurn placedBy;
//     j.at("placedBy").get_to(placedBy);
//     c.m_PlacedBy = placedBy;
//
//     bool isFlipped;
//     j.at("isFlipped").get_to(isFlipped);
//     c.m_IsFlipped = isFlipped;
//
//     bool isGranite;
//     j.at("isGranite").get_to(isGranite);
//     c.m_IsGranite = isGranite;
//
//     bool isEter;
//     j.at("isEter").get_to(isEter);
//     c.m_IsEter = isEter;
//
//     bool isIllusion;
//     j.at("isIllusion").get_to(isIllusion);
//     c.m_IsIllusion = isIllusion;
//
//     j.at("onModifier").get_to(c.m_OnModifier);
// }
