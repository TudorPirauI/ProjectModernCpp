//
// Created by Miruna on 11/24/2024.
//

#ifndef ELEMENTPOWER_H
#define ELEMENTPOWER_H

#include <string>

enum class ElementPower {
    I,
    II,
    III,
    IV,
    V,
    VI,
    VII,
    VIII,
    IX,
    X,
    XI,
    XII,
    XIII,
    XIV,
    XV,
    XVI,
    XVII,
    XVIII,
    XIX,
    XX,
    XXI,
    XXII,
    XXIII,
    XXIV
};

std::string getElementPowerDescription(ElementPower power);

ElementPower RandomPower();

#endif // ELEMENTPOWER_H
