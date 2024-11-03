// Created by Tudor on 10/28/2024.
#pragma once

#include <cstdint>
#include <string>
enum class CardType { Normal, Eter, Flipped };

class Card {
    uint8_t  m_value;
    CardType m_cardType;
    bool     m_canBeCovered = true;
    bool     m_isHidden     = false;

public:
    Card() = default;
    Card(const CardType  &cardType, uint8_t value);


    uint8_t getValue() const;
    void    setValue(uint8_t value);

    std::string getCardType() const;
    bool        isMovable() const;
    bool        isHidden() const;

    ~Card() = default;
};
