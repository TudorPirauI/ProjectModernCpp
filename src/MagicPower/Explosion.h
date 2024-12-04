#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdexcept>
#include <vector>

class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

    explicit Explosion(const std::vector<std::vector<Effect>> &effectMap);
    Effect    generateRandomEffect();
    int       randomInt(int min, int max);
    Explosion generate(int size);

    static Explosion Generate(int size);

private:
    std::vector<std::vector<Effect>>              m_Effects;
    std::vector<std::vector<::Explosion::Effect>> effects;

    static Effect GenerateRandomEffect();

    static int RandomInt(int min, int max);
};

#endif // EXPLOSION_H
