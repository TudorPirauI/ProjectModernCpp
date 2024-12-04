//
// Created by Miruna on 11/24/2024.
//

#ifndef ELEMENTPOWER_H
#define ELEMENTPOWER_H

#include <string>

class ElementPower {
public:
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

private:
    ElementIndexPower RandomPower();
    std::string       GetElementPowerDescription(ElementIndexPower power);
};

#endif // ELEMENTPOWER_H
