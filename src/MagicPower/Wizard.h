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
        Furt,
        NoPower
    };

private:
    WizardPower m_Type;
    int         RandomPower();
    bool m_HasUsedPowerInMatch;


public:
    Wizard(std::string name, int id);
    Wizard(std::string name, int id, WizardPower type);
    Wizard();
    ~Wizard();

    static std::string GetWizardPowerDescription(WizardPower power);

    WizardPower ActivatePower();
    void ResetPowerForNewMatch();
};

#endif // WIZARD_H
