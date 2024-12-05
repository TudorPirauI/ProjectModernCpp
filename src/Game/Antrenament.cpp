//
// Created by edi on 21.11.2024.
//

#include "Antrenament.h"

// Modul antrenament: tabla este de 3x3; fiecare jucător are 7 cărți: 1, 1, 2, 2, 3, 3, 4; Format
// meci: 2/3;

const std::vector CARDS_PLAYER1{Card(1, PlayerTurn::Player1), Card(1, PlayerTurn::Player1),
                                Card(2, PlayerTurn::Player1), Card(2, PlayerTurn::Player1),
                                Card(3, PlayerTurn::Player1), Card(3, PlayerTurn::Player1),
                                Card(4, PlayerTurn::Player1)};
const std::vector CARDS_PLAYER2{Card(1, PlayerTurn::Player2), Card(1, PlayerTurn::Player2),
                                Card(2, PlayerTurn::Player2), Card(2, PlayerTurn::Player2),
                                Card(3, PlayerTurn::Player2), Card(3, PlayerTurn::Player2),
                                Card(4, PlayerTurn::Player2)};

// todo: no more magic numbers

Antrenament::Antrenament(const std::string &nameOne, const std::string &nameTwo) :
    Game(3, 2, nameOne, nameTwo) {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);
}

void Antrenament::SetNewCards() {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_Board.CleanUpBoard();
}

// void Antrenament::CheckWinningConditions() {}

// void Antrenament::GameLoop() {}
