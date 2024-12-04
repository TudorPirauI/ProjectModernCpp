
#include "Wizard.h"

#include <iostream>
#include <utility>

int Wizard::RandomPower() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    return rand() % 10;
}

Wizard::Wizard(std::string name, const int id) :
    MagicPower(std::move(name), id), m_Type(static_cast<WizardPower>(RandomPower())) {}

Wizard::Wizard(std::string name, const int id, const WizardPower type) :
    MagicPower(std::move(name), id), m_Type(type) {}

Wizard::~Wizard() = default;

Wizard::Wizard() : MagicPower("", 0), m_Type(WizardPower::Amnesia) {}

std::string Wizard::GetWizardPowerDescription(const WizardPower power) {
    switch (power) {
        case WizardPower::Teleportation:
            return "Move one of your visible cards to another empty space.";
        case WizardPower::Summoning:
            return "Play a card directly from your own deck.";
        case WizardPower::Metamorphosis:
            return "Swap the positions of two visible cards on the board.";
        case WizardPower::Clairvoyance:
            return "Look at any three covered cards on the board.";
        case WizardPower::Blockade:
            return "Choose an empty space; your opponent cannot place cards there on their next "
                   "turn.";
        case WizardPower::Manipulation:
            return "Rearrange the order of three cards in your own stack.";
        case WizardPower::Illusionism:
            return "Play two illusions simultaneously in different positions.";
        case WizardPower::Deflagration:
            return "Remove all visible covered cards from the board.";
        case WizardPower::Amnesia:
            return "Your opponent loses their next turn.";
        case WizardPower::Theft:
            return "Take a card from your opponent and play it on the board.";
        default:
            return "Unknown power.";
    }
}

Wizard::WizardPower Wizard::ActivatePower() {
    if (m_HasUsedPowerInMatch) {
        std::cout << "The magical power has already been used in this match!" << std::endl;
        return WizardPower::NoPower;
    }

    m_HasUsedPowerInMatch = true;

    std::cout << "The power " << GetWizardPowerDescription(m_Type) << " has been activated!"
              << std::endl;
    return m_Type;
}

void Wizard::ResetPowerForNewMatch() { m_HasUsedPowerInMatch = false; }
