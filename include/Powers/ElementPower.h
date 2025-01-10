//
// Created by Miruna on 11/24/2024.
//

#ifndef ELEMENTPOWER_H
#define ELEMENTPOWER_H

#include "pch.h"

enum class ElementIndexPower {
    ControlledExplosion,
    Destruction,
    Flames,
    Lava,
    FromAshes,
    Sparks,
    Blizzard,
    Gale,
    Hurricane,
    Gust,
    Mirage,
    Storm,
    Tide,
    Fog,
    Wave,
    Whirlpool,
    Tsunami,
    Waterfall,
    Support,
    Earthquake,
    Shattering,
    Granite,
    Avalanche,
    Boulder
};

class ElementPower {
    static ElementIndexPower RandomPower();
    static std::string       GetElementPowerDescription(ElementIndexPower power);

public:
    explicit ElementPower(ElementIndexPower power);

    ElementPower(const ElementPower &other) = default;

    ElementPower(ElementPower &&other) noexcept = default;

    ElementPower &operator=(const ElementPower &other) = default;

    ElementPower &operator=(ElementPower &&other) noexcept = default;

    ~ElementPower() = default;

    [[nodiscard]] ElementIndexPower GetPower() const;

    void SetPower(ElementIndexPower newPower);
};

#endif // ELEMENTPOWER_H
