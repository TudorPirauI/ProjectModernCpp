//
// Created by Miruna on 1/16/2025.
//

#ifndef DUELULELEMENTELOR_H
#define DUELULELEMENTELOR_H

#include "Game.h"
#include "Powers/ElementPower.h"

class DuelulElementelor final : public Game {
public:
    DuelulElementelor(const DuelulElementelor &other)     = default;
    DuelulElementelor(DuelulElementelor &&other) noexcept = default;
    DuelulElementelor &operator=(DuelulElementelor other);

private:
    int          m_ScoreWinning{5};
    ElementPower m_AbilityPlayer1;
    ElementPower m_AbilityPlayer2;

public:
    void         SetNewCards() override;
    ElementPower GetPlayerAbility1() const;
    ElementPower GetPlayerAbility2() const;

    explicit DuelulElementelor(const std::string &nameOne, const std::string &nameTwo,
                               const std::array<bool, 3> &options);
    void SetPlayerAbility1(const ElementPower &ability);
    void SetPlayerAbility2(const ElementPower &ability);
    ~DuelulElementelor() override = default;
};

#endif // DUELULELEMENTELOR_H
