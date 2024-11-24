//
// Created by Miruna on 11/24/2024.
//

#ifndef ELEMENTPOWER_H
#define ELEMENTPOWER_H

#include <string>

class ElementPower {
public:
    enum class ElementIndexPower {
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

private:
    ElementIndexPower RandomPower();
    std::string       getElementPowerDescription(ElementIndexPower power);
};

#endif // ELEMENTPOWER_H
