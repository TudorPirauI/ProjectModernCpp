#include "Explosion.h"
#include <ctime>
#include <random>

Explosion::Explosion(const std::vector<std::vector<Effect>> &effectMap) : m_Effects(effectMap) {}

Explosion Explosion::Generate(const int size) {
    if (size != 3 && size != 4) {
        throw std::invalid_argument("Dimensiunea trebuie să fie 3 sau 4.");
    }

    const int minEffects = (size == 3) ? 2 : 3;
    const int maxEffects = (size == 3) ? 4 : 6;

    const int numEffects = RandomInt(minEffects, maxEffects);

    std::vector effectMap(size, std::vector(size, Effect::Nothing));

    for (int i = 0; i < numEffects; ++i) {
        const Effect effect = GenerateRandomEffect();
        int          x      = RandomInt(0, size - 1);
        int          y      = RandomInt(0, size - 1);

        while (effectMap[x][y] != Effect::Nothing) {
            x = RandomInt(0, size - 1);
            y = RandomInt(0, size - 1);
        }

        effectMap[x][y] = effect;
    }

    return Explosion(effectMap);
}

Explosion::Effect Explosion::GenerateRandomEffect() {
    const int roll = RandomInt(1, 100);
    if (roll <= 45)
        return Effect::Eliminate;
    if (roll <= 90)
        return Effect::Return;
    return Effect::Hole;
}

int Explosion::RandomInt(const int min, const int max) {
    static std::mt19937           rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution dist(min, max);
    return dist(rng);
}
