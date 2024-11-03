#include "Wizard.h"

#include <utility>


Wizard::Wizard(std::string name, EPower power) : m_name(std::move(name)), m_power(power), m_wasUsed(false) {}

void Wizard::setName(std::string name) { m_name = std::move(name); }

std::string Wizard::getName() const { return m_name; }

void Wizard::setPower(EPower power) { m_power = power; }

EPower Wizard::getPower() const { return m_power; }

bool Wizard::isUsed() const { return m_wasUsed; }
