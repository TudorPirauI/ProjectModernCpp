//
// Created by Miruna on 11/24/2024.
//

#include "Powers/ElementPower.h"

void ElementPower::RandomPower() {
    constexpr int totalPowers = 24;

    static std::random_device            rd;
    static std::mt19937                  gen(rd());
    static std::uniform_int_distribution dis(0, totalPowers - 1);

    m_Power = static_cast<ElementIndexPower>(dis(gen));
}

std::vector<ElementPowerInfo> ElementPower::GetRequiredInfo(const ElementIndexPower &power) {
    switch (power) {
        case ElementIndexPower::ControlledExplosion:
            return {};
        case ElementIndexPower::Destruction:
            return {};
        case ElementIndexPower::Flames:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Lava:
            return {ElementPowerInfo::Number};
        case ElementIndexPower::FromAshes:
            return {}; // todo: implement in frontend
        case ElementIndexPower::Sparks:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo,
                    ElementPowerInfo::Card};
        case ElementIndexPower::Blizzard:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Gale:
            return {};
        case ElementIndexPower::Hurricane:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Gust:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo};
        case ElementIndexPower::Mirage:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Storm:
            return {};
        case ElementIndexPower::Tide:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo};
        case ElementIndexPower::Fog:
            return {};
        case ElementIndexPower::Wave:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo,
                    ElementPowerInfo::Card};
        case ElementIndexPower::Whirlpool:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo};
        case ElementIndexPower::Tsunami:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Waterfall:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Support:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Earthquake:
            return {};
        case ElementIndexPower::Shattering:
            return {ElementPowerInfo::PositionOne};
        case ElementIndexPower::Granite:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::Card};
        case ElementIndexPower::Avalanche:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::PositionTwo};
        case ElementIndexPower::Boulder:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::Card};
    }
    throw std::runtime_error("Unknown power.");
}

std::string ElementPower::GetElementPowerDescription(const ElementIndexPower power) {
    switch (power) {
        case ElementIndexPower::ControlledExplosion:
            return "The board explodes!";
        case ElementIndexPower::Destruction:
            return "Removes the opponent's last played card from the game.";
        case ElementIndexPower::Flames:
            return "Flip the opponent's illusion face up. Then play a card on any position on the "
                   "board.";
        case ElementIndexPower::Lava:
            return "Choose a number, provided that at least 2 cards with that number are visible "
                   "on the board. All visible cards with this number return to their owners' "
                   "hands.";
        case ElementIndexPower::FromAshes:
            return "Choose one of your own cards that was removed from the game and play it "
                   "immediately.";
        case ElementIndexPower::Sparks:
            return "Choose any of your own cards covered by the opponent and play it immediately "
                   "on another position.";
        case ElementIndexPower::Blizzard:
            return "Return a visible card of the opponent to their hand.";
        case ElementIndexPower::Gale:
            return "All cards covered by other cards return to their owners' hands.";
        case ElementIndexPower::Hurricane:
            return "Shift a fully occupied row by 1 position to the right. Cards in "
                   "the stack that move out of the board boundaries return to their owners' hands.";
        case ElementIndexPower::Gust:
            return "Move any visible card on the board horizontally or vertically to an adjacent "
                   "position with a card of a lower number.";
        case ElementIndexPower::Mirage:
            return "Replace your placed illusion with another illusion.";
        case ElementIndexPower::Storm:
            return "Remove any stack of cards containing 2 or more cards from the game.";
        case ElementIndexPower::Tide:
            return "Swap the positions of 2 stacks of cards.";
        case ElementIndexPower::Fog:
            return "Play an additional illusion.";
        case ElementIndexPower::Wave:
            return "Move a stack to an adjacent empty position. Play a card on the newly empty "
                   "position.";
        case ElementIndexPower::Whirlpool:
            return "Move 2 cards from the same row, separated by an empty space, into that empty "
                   "space. The card with the higher number goes on top, and in case of a tie, the "
                   "player chooses.";
        case ElementIndexPower::Tsunami:
            return "Choose a row. During the opponent's next turn, they cannot place cards on that "
                   "row.";
        case ElementIndexPower::Waterfall:
            return "Choose a row with at least 3 occupied positions. The stacks on that row 'fall' "
                   "towards one edge, forming a new stack at that edge.";
        case ElementIndexPower::Support:
            return "The value of one of your cards (1/2/3) increases by 1. If that card is covered "
                   "or returned to the hand, it loses the bonus.";
        case ElementIndexPower::Earthquake:
            return "Remove all visible cards with the number 1 from the board.";
        case ElementIndexPower::Shattering:
            return "The value of an opponent's card (2/3/4) decreases by 1. If that card is "
                   "covered or returned to the hand, it loses the penalty.";
        case ElementIndexPower::Granite:
            return "Place a neutral card on the board such that it defines at least one boundary "
                   "of the game board.";
        case ElementIndexPower::Avalanche:
            return "Choose 2 adjacent stacks horizontally/vertically. Shift the two stacks by 1 "
                   "position in their row.";
        case ElementIndexPower::Boulder:
            return "Cover any illusion with a card (from your hand) without flipping the illusion "
                   "face up.";
        default:
            return "Unknown power.";
    }
}
ElementIndexPower ElementPower::GetPower() const { return m_Power; }
void              ElementPower::SetPower(const ElementIndexPower &newPower) { m_Power = newPower; }

bool ElementPower::GetUsedPowerInMatch() const { return m_HasUsedPowerInMatch; }

void ElementPower::SetUsedPowerInMatch(const bool value) { m_HasUsedPowerInMatch = value; }

// void to_json(nlohmann::json &j, const ElementPower &elementPower) {
//     j = nlohmann::json{{"power", static_cast<int>(elementPower.m_Power)},
//                        {"hasUsedPowerInMatch", elementPower.m_HasUsedPowerInMatch}};
// }
//
// void from_json(const nlohmann::json &j, ElementPower &elementPower) {
//     int power;
//     j.at("power").get_to(power);
//     elementPower.m_Power = static_cast<ElementIndexPower>(power);
//     j.at("hasUsedPowerInMatch").get_to(elementPower.m_HasUsedPowerInMatch);
// }
