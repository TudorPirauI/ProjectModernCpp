//
// Created by Miruna on 1/16/2025.
//

#include "Game.h" // Calea către Game.h
#include "Powers/ElementPower.h" // Calea către ElementPower.h

#ifndef DUELULELEMENTELOR_H
#define DUELULELEMENTELOR_H

class DuelulElementelor : public Game {
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
    ElementPower GetPlayerAbility1();
    ElementPower GetPlayerAbility2();

    explicit DuelulElementelor(const std::string &nameOne, const std::string &nameTwo, const std::array<bool, 3> &options);
    ~DuelulElementelor() override = default;
};

#endif // DUELULELEMENTELOR_H
