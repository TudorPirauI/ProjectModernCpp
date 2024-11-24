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
    MagicPower(std::move(name), id), m_type(static_cast<WIZARD_TYPE>(RandomPower())) {}

Wizard::Wizard(std::string name, int id, WIZARD_TYPE type) : MagicPower(std::move(name), id), m_type(type) {}

Wizard::~Wizard() {}
