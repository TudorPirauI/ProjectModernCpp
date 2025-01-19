//
// Created by Tudor on 11/24/2024.
//

#ifndef WIZARD_H
#define WIZARD_H
#include <iosfwd>

#include <vector>

#include "ElementPower.h"

enum class WizardPower {
    EliminateOpponentCard,
    RemoveRow,
    CoverOpponentCard,
    CreatePit,
    MoveOwnStack,
    GainEterCard,
    MoveOpponentStack,
    ShiftRowToEdge,
    NoPower
};

class Wizard {
    WizardPower m_Type;

    bool m_HasUsedPowerInMatch{false};

public:
    Wizard(const Wizard &other)                = default;
    Wizard(Wizard &&other) noexcept            = default;
    Wizard &operator=(const Wizard &other)     = default;
    Wizard &operator=(Wizard &&other) noexcept = default;
    Wizard();
    WizardPower GetType() const;
    ~Wizard() = default;

    void SetType(WizardPower toInt);

    [[nodiscard]] bool HasUsedPowerInMatch() const;

    [[nodiscard]] std::string                   GetWizardPowerDescription() const;
    [[nodiscard]] std::vector<ElementPowerInfo> GetWizardRequiredInfo() const;
    static int                                  RandomPower();

    WizardPower ActivatePower();
    void        ResetPowerForNewMatch();

    void SetUsedPowerInMatch(bool toBool);

    // friend void to_json(nlohmann::json &j, const Wizard &wizard);
    // friend void from_json(const nlohmann::json &j, Wizard &wizard);
};

#endif // WIZARD_H
