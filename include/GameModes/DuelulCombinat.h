//
// Created by Miruna on 1/16/2025.
//

#ifndef DUELULCOMBINAT_H
#define DUELULCOMBINAT_H

#include "../Powers/ElementPower.h"
#include "../Powers/Wizard.h"
#include "Game.h"

class DuelulCombinat final : public Game {
    int          m_ScoreWinning{5};
    Wizard       m_WizardAbility1;
    ElementPower m_ElementalAbility1;
    Wizard       m_WizardAbility2;
    ElementPower m_ElementalAbility2;

public:
    DuelulCombinat(const DuelulCombinat &other)     = default;
    DuelulCombinat(DuelulCombinat &&other) noexcept = default;
    DuelulCombinat &operator=(DuelulCombinat other);

    void SetNewCards() override;

    Wizard       GetWizardAbility() const;
    ElementPower GetElementalAbility() const;

    DuelulCombinat(const std::string &nameOne, const std::string &nameTwo,
                   const std::array<bool, 3> &options);
    ~DuelulCombinat() override = default;
};

#endif // DUELULCOMBINAT_H
