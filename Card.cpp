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

Card::Card(const std::string &cardType, uint8_t value) : m_cardType(cardType), m_value(value) {
    if (value < 1 || value > 4) {
        throw std::invalid_argument("Card value must be between 1 and 4(including 1 and 4).");
    }

    if (m_cardType == "eter") {
        m_isMovable = false;
    }
}

uint8_t Card::getValue() const { return m_value; }

void Card::setValue(uint8_t value) {
    if (value >= 1 && value <= 4) {
        m_value = value;
    } else {
        throw std::invalid_argument("Card value must be between 1 and 4.");
    }
}

const std::string &Card::getCardType() const { return m_cardType; }

bool Card::isMovable() const { return m_isMovable; }

bool Card::isHidden() const { return m_isHidden; }
