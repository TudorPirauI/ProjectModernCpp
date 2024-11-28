//
// Created by Tudor on 11/24/2024.
//

#include "MagicPower.h"

#include <utility>

MagicPower::MagicPower(std::string name, const int id) : m_name(std::move(name)), m_id(id) {}

void MagicPower::setName(const std::string &name) { m_name = name; }
void MagicPower::setId(const int id) { m_id = id; }
void MagicPower::setWasUsed(const bool wasUsed) { m_WasUsed = wasUsed; }

std::string MagicPower::getName() const { return m_name; }
int         MagicPower::getId() const { return m_id; }
bool        MagicPower::getWasUsed() const { return m_WasUsed; }

MagicPower::~MagicPower() = default;
