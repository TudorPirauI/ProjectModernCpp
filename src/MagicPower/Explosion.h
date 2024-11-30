#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdexcept>
#include <vector>

class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

private:
    std::vector<std::vector<Effect>> effects;

public:
    explicit Explosion(const std::vector<std::vector<Effect>> &effectMap);

    static Explosion generate(int size);

private:
    static Effect generateRandomEffect();


    static int randomInt(int min, int max);
};

#endif // EXPLOSION_H
