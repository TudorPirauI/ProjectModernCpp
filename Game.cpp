//
// Created by edi on 30.10.2024.
//

#include "Game.h"
#include "Player.h"

Game::Game(Player player1, Player player2, GameMode mode) {
    this->m_player1  = player1;
    this->m_player2  = player2;
    this->m_mode     = mode;
    this->m_turn     = false;
    this->m_gameOver = false;
}

Player Game::getPlayer1() { return this->m_player1; }

Player Game::getPlayer2() { return this->m_player2; }

void Game::DecideTurn() {
    if (m_turn == false) {
        // write message like: Player's 2 turn!
        m_turn = true;
    } else {
        // Player's 1 turn!
        m_turn = false;
    }
}

void Game::SetPlayer1(Player player) { this->m_player1 = player; }

void Game::SetPlayer2(Player player) { this->m_player2 = player; }
