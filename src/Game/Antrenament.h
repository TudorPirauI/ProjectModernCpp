//
// Created by edi on 21.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H

#include <unordered_map>

#include "../GameBoard/Board.h"
#include "../Player/Player.h"
#include "Game.h"

class Antrenament : public Game {
public:
    Antrenament(const std::string &nameOne, const std::string &nameTwo);

    void SetNewCards() override;

    Antrenament(const Antrenament &other)                = default;
    Antrenament(Antrenament &&other) noexcept            = default;
    Antrenament &operator=(const Antrenament &other)     = default;
    Antrenament &operator=(Antrenament &&other) noexcept = default;
};

#endif // ANTRENAMENT_H
