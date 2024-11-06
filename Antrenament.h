//
// Created by edi on 06.11.2024.
//

#ifndef ANTRENAMENT_H
#define ANTRENAMENT_H

#include "Game.h"

class Antrenament : public Game {
private:
    void SetCards() override;
public:
    Antrenament(Player player1, Player player2);

    // need methods from player and card :))))))))))))
    // player -> getCards of the player public method
    // to be implemented in the future DecideWinner() methods
    void DecideWinner() override; // for turn to be more precise
    void StartGame() override;
    void NewTurn() override;
    std::optional<Player> EndGame() override;

};

#endif //ANTRENAMENT_H
