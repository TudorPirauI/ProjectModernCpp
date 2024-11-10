#include "Wizard.h"

#include <iostream>
#include <utility>


Wizard::Wizard(std::string name, EPower power) : m_name(std::move(name)), m_power(power), m_wasUsed(false) {}

void Wizard::setName(std::string name) { m_name = std::move(name); }

std::string Wizard::getName() const { return m_name; }

void Wizard::setPower(EPower power) { m_power = power; }

EPower Wizard::getPower() const { return m_power; }

bool Wizard::IsUsed() const { return m_WasUsed; }


bool Wizard::CanUsePower(const std::pair<int, int> &locatie) const {
    if (m_WasUsed) {
        std::cout << "The power was already used.\n";
        return false;
    }
    return true;
}

void Wizard::UsePower(const std::pair<int, int> &locatie) {
    if (!CanUsePower(locatie)) {
        throw std::runtime_error("This power can't be used here");
    }

    m_WasUsed = true;
    switch (m_power) {
        // TO DO: case-urile
        default:;
    }
}

std::string Wizard::ToString(EPower power) {
    switch (power) {

        default:;
    }
}
