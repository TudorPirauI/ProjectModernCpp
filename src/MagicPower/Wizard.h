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
    Wizard(const Wizard &other)                = default;
    Wizard(Wizard &&other) noexcept            = default;
    Wizard &operator=(const Wizard &other)     = default;
    Wizard &operator=(Wizard &&other) noexcept = default;
    enum class WizardPower {
        Teleportation,
        Summoning,
        Metamorphosis,
        Clairvoyance,
        Blockade,
        Manipulation,
        Illusionism,
        Deflagration,
        Amnesia,
        Theft,
        NoPower
    };

private:
    WizardPower m_Type;
    int         RandomPower();
    bool        m_HasUsedPowerInMatch{false};

public:
    Wizard(std::string name, int id);
    Wizard(std::string name, int id, WizardPower type);
    Wizard();
    ~Wizard();

    static std::string GetWizardPowerDescription(WizardPower power);

    WizardPower ActivatePower();
    void        ResetPowerForNewMatch();
};

#endif // WIZARD_H
