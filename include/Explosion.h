#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdexcept>
#include <vector>

class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

    explicit Explosion(const std::vector<std::vector<Effect>> &effectMap);
    Explosion Generate(int size);

private:
    std::vector<std::vector<Effect>> m_Effects;

public:
    Explosion(const Explosion &other)                = default;
    ~Explosion()                                     = default;
    Explosion(Explosion &&other) noexcept            = default;
    Explosion &operator=(const Explosion &other)     = default;
    Explosion &operator=(Explosion &&other) noexcept = default;

private:
    static Effect GenerateRandomEffect();

    static int RandomInt(int min, int max);
};

#endif // EXPLOSION_H
