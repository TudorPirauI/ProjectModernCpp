//
// Created by edi on 21.11.2024.
//

#include "Antrenament.h"

void Antrenament::CheckWinningConditions() {
    if (WinningOnDiagonal() or WinningOnVertical() or WinningOnLine()) {
        m_GameState = GameState::NormalWin;
    }

    if (NoCardLeft()) {
        m_GameState = GameState::FinishedCards;
    }
    m_GameState = GameState::NotFinished;
}

bool Antrenament::NoCardLeft() { return m_Player1.GetHand().empty() or m_Player2.GetHand().empty(); }

bool Antrenament::WinningOnDiagonal() { return false; }

bool Antrenament::WinningOnLine() { return false; }

bool Antrenament::WinningOnVertical() { return false; }

void Antrenament::GameLoop() {}
