
#include "Powers/Wizard.h"

#include "Powers/ElementPower.h"

int Wizard::RandomPower() {
    static std::random_device            rd;
    static std::mt19937                  gen(rd());
    static std::uniform_int_distribution dis(0, 7);

    return dis(gen);
}

Wizard::Wizard() : m_Type(static_cast<WizardPower>(RandomPower())) {}

std::vector<ElementPowerInfo> Wizard::GetWizardRequiredInfo() const {
    switch (m_Type) {
        case WizardPower::EliminateOpponentCard:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::RemoveRow:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::CoverOpponentCard:
            return {ElementPowerInfo::PositionOne, ElementPowerInfo::Card};
        case WizardPower::CreatePit:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::MoveOwnStack:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::GainEterCard:
            return {};
        case WizardPower::MoveOpponentStack:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::ShiftRowToEdge:
            return {ElementPowerInfo::PositionOne};
        case WizardPower::NoPower:
            return {};
        default:
            return {};
    }
}

bool Wizard::HasUsedPowerInMatch() const { return m_HasUsedPowerInMatch; }

std::string Wizard::GetWizardPowerDescription() const {
    switch (m_Type) {
        case WizardPower::EliminateOpponentCard:
            return "Remove an opponent's card that covers your own card.";
        case WizardPower::RemoveRow:
            return "Eliminate an entire row with at least 3 positions, containing at least one of "
                   "your visible cards.";
        case WizardPower::CoverOpponentCard:
            return "Cover an opponent's card with one of your lower-value cards from your hand.";
        case WizardPower::CreatePit:
            return "Transform an empty space on the board into a pit.";
        case WizardPower::MoveOwnStack:
            return "Move a stack of cards with your card on top to another empty position on the "
                   "board.";
        case WizardPower::GainEterCard:
            return "Gain an extra Eter card and place it immediately on the board.";
        case WizardPower::MoveOpponentStack:
            return "Move a stack of cards with the opponent's card on top to another empty "
                   "position on the board.";
        case WizardPower::ShiftRowToEdge:
            return "Shift a row with at least 3 positions from one edge of the board to another "
                   "edge.";
        default:
            return "Unknown power.";
    }
}

WizardPower Wizard::ActivatePower() {
    if (m_HasUsedPowerInMatch) {
        std::cout << "The magical power has already been used in this match!" << std::endl;
        return WizardPower::NoPower;
    }

    m_HasUsedPowerInMatch = true;

    std::cout << "The power " << GetWizardPowerDescription() << " has been activated!" << std::endl;
    return m_Type;
}

void Wizard::ResetPowerForNewMatch() { m_HasUsedPowerInMatch = false; }

// void to_json(nlohmann::json &j, const Wizard &wizard) {
//     j = nlohmann::json{{"type", static_cast<int>(wizard.m_Type)},
//                        {"hasUsedPowerInMatch", wizard.m_HasUsedPowerInMatch}};
// }
//
// void from_json(const nlohmann::json &j, Wizard &wizard) {
//     int type;
//     j.at("type").get_to(type);
//     wizard.m_Type = static_cast<WizardPower>(type);
//     j.at("hasUsedPowerInMatch").get_to(wizard.m_HasUsedPowerInMatch);
// }
