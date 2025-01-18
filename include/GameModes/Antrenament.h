//
// Created by edi on 21.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H

#include "GameModes/Game.h"

class Antrenament final : public Game {
public:
    Antrenament(const std::string &nameOne, const std::string &nameTwo,
                const std::array<bool, 3> &options);

    void SetNewCards() override;
    // void to_json(nlohmann::json &j, Antrenament &antrenament);
    // void from_json(const nlohmann::json &j, Antrenament &antrenament);

    Antrenament(const Antrenament &other)                = default;
    Antrenament(Antrenament &&other) noexcept            = default;
    Antrenament &operator=(const Antrenament &other)     = default;
    Antrenament &operator=(Antrenament &&other) noexcept = default;
};

#endif // ANTRENAMENT_H
