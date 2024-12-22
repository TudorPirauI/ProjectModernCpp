//
// Created by Tudor on 11/24/2024.
//

#include "MagicPower.h"

#include <utility>

MagicPower::MagicPower(std::string name, const int id) : m_Name(std::move(name)), m_Id(id) {}

void MagicPower::SetName(const std::string &name) { m_Name = name; }
void MagicPower::SetId(const int id) { m_Id = id; }
void MagicPower::SetWasUsed(const bool wasUsed) { m_WasUsed = wasUsed; }

std::string MagicPower::GetName() const { return m_Name; }
int         MagicPower::GetId() const { return m_Id; }
bool        MagicPower::GetWasUsed() const { return m_WasUsed; }

MagicPower::~MagicPower() = default;
