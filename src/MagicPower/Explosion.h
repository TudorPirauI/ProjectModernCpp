#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdexcept>
#include <vector>

class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

private:
    std::vector<std::vector<Effect>> m_Effects;

public:
    explicit Explosion(const std::vector<std::vector<Effect>> &effectMap);

    static Explosion Generate(int size);

private:
    static Effect GenerateRandomEffect();

    static int RandomInt(int min, int max);
};

#endif // EXPLOSION_H
