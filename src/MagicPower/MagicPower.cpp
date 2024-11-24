//
// Created by Tudor on 11/24/2024.
//

#include "MagicPower.h"

MagicPower::MagicPower(std::string name, int id) {
    m_name = name;
    m_id   = id;
}

void MagicPower::setName(std::string name) { m_name = name; }
void MagicPower::setId(int id) { m_id = id; }
void MagicPower::setWasUsed(bool wasUsed) { m_WasUsed = wasUsed; }

std::string MagicPower::getName() { return m_name; }
int         MagicPower::getId() { return m_id; }
bool        MagicPower::getWasUsed() { return m_WasUsed; }

MagicPower::~MagicPower() {}
