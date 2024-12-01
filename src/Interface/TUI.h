//
// Created by edi on 27.11.2024.
//

#ifndef TUI_H
#define TUI_H

#include "../Game/Game.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

/*
        sa mearga modul antrenament
            afisare player carti
            winning conditions
            gandire mod de actionare
            turn
            gandire metoda de input mai buna
            cum sa incepem jocu : programu sa te intrebe ceva */

class TUI {
public:
    TUI();

    void ShowMenu();
    void GameLoopTraining();
    void InitGame(const std::string &gameMode, const std::string &player1,
                  const std::string &player2);

private:
    std::unique_ptr<Game> m_Game;

    void StartGameMenu();
    void RestartGame();
};

#endif // TUI_H
