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
        case EPower::EliminaCarteAcoperita:
            std::cout << "Elimină o carte acoperită a oponentului.\n";
            break;
        case EPower::EliminaRandCarti:
            std::cout << "Elimină un rând de cărți.\n";
            break;
        case EPower::AcoperaCarteOponent:
            std::cout << "Acoperă o carte a oponentului.\n";
            break;
        case EPower::CreeazaGroapa:
            std::cout << "Creează o groapă pe tablă.\n";
            break;
        case EPower::MutaTeancPropriu:
            std::cout << "Mută teancul propriu.\n";
            break;
        case EPower::ExtraCarteEter:
            std::cout << "Primește o carte Eter.\n";
            break;
        case EPower::MutaTeancOponent:
            std::cout << "Mută teancul oponentului.\n";
            break;
        case EPower::MutaRandMargine:
            std::cout << "Mută un rând de la margine.\n";
            break;
        default:
            throw std::invalid_argument("Unknown power");
    }
}

std::string Wizard::ToString(EPower power) {
    switch (power) {
        case EPower::EliminaCarteAcoperita:
            return "Elimină o carte acoperită";
        case EPower::EliminaRandCarti:
            return "Elimină un rând de cărți";
        case EPower::AcoperaCarteOponent:
            return "Acoperă o carte a oponentului";
        case EPower::CreeazaGroapa:
            return "Creează o groapă";
        case EPower::MutaTeancPropriu:
            return "Mută teancul propriu";
        case EPower::ExtraCarteEter:
            return "Extra carte Eter";
        case EPower::MutaTeancOponent:
            return "Mută teancul oponentului";
        case EPower::MutaRandMargine:
            return "Mută un rând de la margine";
        default:
            return "Putere necunoscută";
    }
}
