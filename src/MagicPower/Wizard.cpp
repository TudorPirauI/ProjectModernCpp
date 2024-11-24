//
// Created by Tudor on 11/24/2024.
//

#include "Wizard.h"

#include <utility>

int Wizard::RandomPower() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    return rand() % 7 + 1;
}

Wizard::Wizard(std::string name, int id) :
    MagicPower(std::move(name), id), m_type(static_cast<WizardPower>(RandomPower())) {}

Wizard::Wizard(std::string name, int id, WizardPower type) : MagicPower(std::move(name), id), m_type(type) {}

Wizard::~Wizard() {}

std::string Wizard::getWizardPowerDescription(WizardPower power) {
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
