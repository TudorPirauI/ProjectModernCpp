//
// Created by Tudor on 11/28/2024.
//

#ifndef DUELULVRAJITORILOR_H
#define DUELULVRAJITORILOR_H

#include "GameModes/Game.h"
#include "Powers/Wizard.h"

class DuelulVrajitorilor final : public Game {
    int    m_ScoreWinning{5};
    Wizard m_AbilityPlayer1;
    Wizard m_AbilityPlayer2;

public:
    void SetNewCards() override;

    Wizard GetPlayerAbility1() const;
    Wizard GetPlayerAbility2() const;
    DuelulVrajitorilor(const DuelulVrajitorilor &other)                = default;
    DuelulVrajitorilor(DuelulVrajitorilor &&other) noexcept            = default;
    DuelulVrajitorilor &operator=(const DuelulVrajitorilor &other)     = default;
    DuelulVrajitorilor &operator=(DuelulVrajitorilor &&other) noexcept = default;

    DuelulVrajitorilor(const std::string &nameOne, const std::string &nameTwo,
                       const std::array<bool, 3> &options);
    void SetPlayerAbility1(const Wizard &ability);
    void SetPlayerAbility2(const Wizard &ability);
    // void to_json(nlohmann::json &j, DuelulVrajitorilor &duelulVrajitorilor);
    // void from_json(const nlohmann::json &j, DuelulVrajitorilor &duelulVrajitorilor);
    ~DuelulVrajitorilor() override = default;
};

#endif // DUELULVRAJITORILOR_H
