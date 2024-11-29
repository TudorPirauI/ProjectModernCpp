#pragma once

#include <ctime>
#include <iostream>
#include <random>
#include <vector>

class Explosion {
public:
    enum class Effect { Nothing, Eliminate, Return, Hole };

private:
    std::vector<std::vector<Effect>> m_Effects;

public:
    explicit Explosion(const std::vector<std::vector<Effect>> &effectMap) : m_Effects(effectMap) {}

    static Explosion Generate(const int size) {
        if (size != 3 && size != 4) {
            throw std::invalid_argument("Dimensiunea trebuie să fie 3 sau 4.");
        }

        const int minEffects = size == 3 ? 2 : 3;
        const int maxEffects = size == 3 ? 4 : 6;

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

private:
    static Effect GenerateRandomEffect() {
        const int roll = RandomInt(1, 100);
        if (roll <= 45)
            return Effect::Eliminate; // 45% șanse
        if (roll <= 90)
            return Effect::Return; // 45% șanse
        return Effect::Hole; // 10% șanse
    }

    static int RandomInt(const int min, const int max) {
        static std::random_device     rd;
        static std::mt19937           rng(rd());
        std::uniform_int_distribution dist(min, max);
        return dist(rng);
    }
};
