//
// Created by edi on 06.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H

#include "Game.h"

class Antrenament : public Game {
public:
    Antrenament(Player player1, Player player2) {
        this->m_player1 = player1;
        this->m_player2 = player2;
        this->m_mode = GameMode::Antrenament;
    }

    // need methods from player and card :))))))))))))
    // player -> getCards of the player public method
    // card -> constructor with value and more (with all the values)
    void DecideWinner() override;
    void StartGame() override;
    void EndGame() override;
    void SetCards() override;
};

#endif //ANTRENAMENT_H
