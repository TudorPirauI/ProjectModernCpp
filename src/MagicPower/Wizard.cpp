//
// Created by Tudor on 11/24/2024.
//

#include "Wizard.h"

#include <utility>
#include <iostream>

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

Wizard::Wizard() : MagicPower("", 0), m_Type(WizardPower::Amnezie) {}

std::string Wizard::GetWizardPowerDescription(const WizardPower power) {
    switch (power) {
        case WizardPower::Teleportatie:
            return "Mută o carte proprie vizibilă într-un alt spațiu gol.";
        case WizardPower::Invocatie:
            return "Joacă o carte direct din teancul propriu de joc.";
        case WizardPower::Metamorfoza:
            return "Schimbă locul între două cărți vizibile de pe tablă.";
        case WizardPower::Clarviziune:
            return "Uită-te la oricare trei cărți acoperite de pe tablă.";
        case WizardPower::Blocaj:
            return "Alege un spațiu gol; adversarul nu poate plasa cărți acolo în următoarea tură.";
        case WizardPower::Manipulare:
            return "Schimbă ordinea a trei cărți dintr-un teanc propriu.";
        case WizardPower::Iluzionism:
            return "Joacă două iluzii simultan în poziții diferite.";
        case WizardPower::Deflagratie:
            return "Elimină toate cărțile acoperite vizibile de pe tablă.";
        case WizardPower::Amnezie:
            return "Adversarul își pierde următoarea tură.";
        case WizardPower::Furt:
            return "Ia o carte de la adversar și joac-o pe tablă.";
        default:
            return "Putere necunoscută.";
    }
}

Wizard::WizardPower Wizard::ActivatePower() {
    if (m_HasUsedPowerInMatch) {
        std::cout << "Puterea magică a fost deja folosită în acest meci!" << std::endl;
        return WizardPower::NoPower;
    }

    m_HasUsedPowerInMatch = true;

    std::cout << "Puterea " << GetWizardPowerDescription(m_Type) << " a fost activată!" << std::endl;
    return m_Type;
}


void Wizard::ResetPowerForNewMatch() {
    m_HasUsedPowerInMatch = false;
}
