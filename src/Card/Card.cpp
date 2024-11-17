#include "Card.h"

Card::Card(const int value) : m_Value{value}, m_IsIllusion{false}, m_IsEter{false} {}

void Card::SetValue(const int value) { this->m_Value = value; }
int  Card::GetValue() const { return this->m_Value; }

void Card::SetIllusion(const bool isIllusion) { this->m_IsIllusion = isIllusion; }
bool Card::GetIsIllusion() const { return this->m_IsIllusion; }

void Card::SetEter(const bool isEter) { this->m_IsEter = isEter; }
bool Card::GetIsEter() const { return this->m_IsEter; }

// std::optional<Position> Card::GetPosition() const { return this->m_Position; }
// void                    Card::SetPosition(Position position) { this->m_Position = position; }
