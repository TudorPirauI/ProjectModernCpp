//
// Created by edi on 21.11.2024.
//

#include "Antrenament.h"

// Modul antrenament: tabla este de 3x3; fiecare jucător are 7 cărți: 1, 1, 2, 2, 3, 3, 4; Format
// meci: 2/3;

const std::vector CARDS{Card(1), Card(1), Card(2), Card(2), Card(3), Card(3), Card(4)};

// todo: no more magic numbers
Antrenament::Antrenament(const std::string &nameOne, const std::string &nameTwo) :
    Game(3, 2, nameOne, nameTwo) {
    m_Player1.SetHand(CARDS);
    m_Player2.SetHand(CARDS);
}

void Antrenament::SetNewCards() {
    m_Player1.SetHand(CARDS);
    m_Player2.SetHand(CARDS);
    m_Lines.clear();
    m_Columns.clear();
    m_Board.CleanUpBoard();
}

// void Antrenament::CheckWinningConditions() {}

// void Antrenament::GameLoop() {}
