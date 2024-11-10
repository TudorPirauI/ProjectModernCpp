// Created by Tudor on 10/28/2024.
#pragma once

#include <cstdint>
#include <string>
enum class CardType { Normal, Eter, Flipped };

class Card {
    uint8_t  m_value;
    CardType m_cardType;
    bool     m_canBeCovered;
    bool     m_isHidden;

public:
    Card() = default;
    Card(const CardType &cardType, uint8_t value, bool isHidden);


    [[nodiscard]] uint8_t getValue() const;
    void                  setValue(uint8_t value);

    [[nodiscard]] const CardType &getCardType() const;
    [[nodiscard]] bool            isMovable() const;
    [[nodiscard]] bool            isHidden() const;

    void showCardInfo() const;

    ~Card() = default;
};
