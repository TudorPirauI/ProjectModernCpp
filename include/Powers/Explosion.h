#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "../include/GameComponents/Card.h"
#include "pch.h"

// todo: make the explosion work well
class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

    static Explosion Generate(int size, const Position &left, const Position &up,
                              const Position &down, const Position &right);

    explicit Explosion(const std::vector<std::pair<Position, Effect>> &effectMap);

    std::vector<std::pair<Position, Effect>> GetEffects() const;

    Explosion(const Explosion &other)                = default;
    ~Explosion()                                     = default;
    Explosion(Explosion &&other) noexcept            = default;
    Explosion &operator=(const Explosion &other)     = default;
    Explosion &operator=(Explosion &&other) noexcept = default;

private:
    std::vector<std::pair<Position, Effect>> m_Effects;

    static Effect GenerateRandomEffect();

    static int RandomInt(int min, int max);
};

#endif // EXPLOSION_H
