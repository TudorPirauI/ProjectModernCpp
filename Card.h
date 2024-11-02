// Created by Tudor on 10/28/2024.
#pragma once

#include <cstdint>
#include <string>

class Card {
    uint8_t     m_value;
    std::string m_cardType;
    bool        m_isMovable = true;
    bool        m_isHidden  = false;

public:
    Card() = default;
    Card(const std::string &cardType, uint8_t value);

    uint8_t getValue() const;
    void    setValue(uint8_t value);

    const std::string &getCardType() const;
    bool               isMovable() const;
    bool               isHidden() const;

    ~Card() = default;
};
