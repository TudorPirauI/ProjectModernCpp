//
// Created by edi on 17.01.2025.
//

#ifndef TURNEU_H
#define TURNEU_H
#include "Game.h"

class Turneu {
    std::unique_ptr<Game> m_Game;

public:
    void                                SetGameMode(std::unique_ptr<Game> game);
    [[nodiscard]] std::unique_ptr<Game> GetGameMode();

    Turneu();
    Turneu(const Turneu &other)     = default;
    Turneu(Turneu &&other) noexcept = default;
    Turneu &operator=(Turneu other);
    ~Turneu() = default;
};

#endif // TURNEU_H
