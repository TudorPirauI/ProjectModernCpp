#include "Card.h"

#include <stdexcept>

Card::Card(const int value) : m_IsIllusion{false}, m_IsEter{false} {
    if (value > 4 || value < 1) {
        throw std::invalid_argument("Invalid card value");
    }

    m_Value = value;
}

bool Card::GetIsFlipped() const { return m_isFlipped; }
void Card::SetIsFlipped(const bool isFlipped) { m_isFlipped = isFlipped; }

void Card::SetValue(const int value) { m_Value = value; }
int  Card::GetValue() const { return m_Value; }

void Card::SetIllusion(const bool isIllusion) { m_IsIllusion = isIllusion; }
bool Card::GetIsIllusion() const { return m_IsIllusion; }

void Card::SetEter(const bool isEter) { m_IsEter = isEter; }
bool Card::GetIsEter() const { return m_IsEter; }
