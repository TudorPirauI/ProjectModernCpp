#include "Card.h"

bool  Card::GetIsFlipped() const { return m_isFlipped; }
void  Card::SetIsFlipped(bool isFlipped) { m_isFlipped = isFlipped; }
Card::Card(const int value) : m_Value{value}, m_IsIllusion{false}, m_IsEter{false} {}

void Card::SetValue(const int value) { m_Value = value; }
int  Card::GetValue() const { return m_Value; }

void Card::SetIllusion(const bool isIllusion) { m_IsIllusion = isIllusion; }
bool Card::GetIsIllusion() const { return m_IsIllusion; }

void Card::SetEter(const bool isEter) { m_IsEter = isEter; }
bool Card::GetIsEter() const { return m_IsEter; }

// std::optional<Position> Card::GetPosition() const { return m_Position; }
// void                    Card::SetPosition(Position position) { m_Position = position; }
