//
// Created by Tudor on 10/28/2024.
//

/*
Value: between 1 and 4
Movable: "eter" cards are unmovable
Hidden: placed face down, opponent can't see the value, can try placing a card
      if oponent card is equal or lower, the opponent's card is lost and he
      wastes the turn
 */

#include <stdexcept>

#include "Card.h"

#include <iostream>
#include <ostream>

Card::Card(const CardType &cardType, uint8_t value, bool isHidden) :
    m_cardType(cardType), m_value(value), m_isHidden(isHidden) {
    if (value < 1 || value > 4) {
        throw std::invalid_argument("Card value must be between 1 and 4(including 1 and 4).");
    }


    m_canBeCovered = (m_cardType != CardType::Eter);
    m_isHidden     = isHidden;
}

uint8_t Card::getValue() const { return m_value; }

void Card::setValue(uint8_t value) {
    if (value >= 1 && value <= 4) {
        m_value = value;
    } else {
        throw std::invalid_argument("Card value must be between 1 and 4.");
    }
}

const CardType &Card::getCardType() const { return m_cardType; }

bool Card::isMovable() const { return m_canBeCovered; }

bool Card::isHidden() const { return m_isHidden; }

void Card::showCardInfo() const {
    std::cout << "Value: " << static_cast<int>(m_value) << ", CardType: ";
    switch (m_cardType) {
        case CardType::Normal:
            std::cout << "Normal";
            break;
        case CardType::Eter:
            std::cout << "Eter";
            break;
        case CardType::Flipped:
            std::cout << "Flipped";
            break;
    }
    std::cout << ", Hidden: " << (m_isHidden ? "Yes" : "No") << std::endl;
}
