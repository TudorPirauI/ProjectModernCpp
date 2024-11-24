//
// Created by Tudor on 11/24/2024.
//

#ifndef WIZARD_H
#define WIZARD_H

#include <cstdlib>
#include <ctime>
#include "MagicPower.h"

class Wizard : public MagicPower {
public:
    enum class WizardPower {
        Teleportatie,
        Invocatie,
        Metamorfoza,
        Clarviziune,
        Blocaj,
        Manipulare,
        Iluzionism,
        Deflagratie,
        Amnezie,
        Furt
    };

private:
    WizardPower m_type;
    int         RandomPower();

public:
     Wizard(std::string name, int id);
     Wizard(std::string name, int id, WizardPower m_type);
    ~Wizard();

    std::string getWizardPowerDescription(WizardPower power);
};


#endif // WIZARD_H
