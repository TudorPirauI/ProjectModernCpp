#include "Powers/Explosion.h"
#include "../include/GameComponents/Card.h"

Explosion::Explosion(const std::vector<std::pair<Position, Effect>> &effectMap) :
    m_Effects(effectMap) {}

Explosion Explosion::Generate(const int size, const Position &left, const Position &up,
                              const Position &down, const Position &right) {
    if (size < 3 or size > 4) {
        throw std::invalid_argument("Size must be 3 or 4");
    }

    const int minEffects = size == 3 ? 2 : 3;
    const int maxEffects = size == 3 ? 4 : 6;

    const int numEffects = RandomInt(minEffects, maxEffects);

    std::vector<std::pair<Position, Effect>> effects;

    for (int i = 0; i < numEffects; ++i) {
        Effect effect = GenerateRandomEffect();
        int    x      = RandomInt(up.first, down.first - 1);
        int    y      = RandomInt(left.second, right.second - 1);

        Position pos = {x, y};
        while (std::ranges::find_if(effects, [&pos](const auto &pair) {
                   return pair.first == pos;
               }) != effects.end()) {
            x   = RandomInt(0, size - 1);
            y   = RandomInt(0, size - 1);
            pos = {x, y};
        }

        effects.emplace_back(pos, effect);
    }

    return effects;
}

// todo: check if 2  rows are full of cards

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
