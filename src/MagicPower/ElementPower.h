//
// Created by Miruna on 11/24/2024.
//

#ifndef ELEMENTPOWER_H
#define ELEMENTPOWER_H

#include <string>

class ElementPower {
public:
    enum class ElementIndexPower {
        ExplozieControlata,
        Distrugere,
        Flacari,
        Lava,
        DinCenusa,
        Scantei,
        Vifor,
        Vijelie,
        Uragan,
        Rafala,
        Miraj,
        Furtuna,
        Maree,
        Ceata,
        Val,
        VartejDeApa,
        Tsunami,
        Cascada,
        Sprijin,
        Cutremur,
        Sfaramare,
        Granite,
        Avalansa,
        Bolovan
    };

private:
    ElementIndexPower RandomPower();
    std::string       GetElementPowerDescription(ElementIndexPower power);
};

#endif // ELEMENTPOWER_H
