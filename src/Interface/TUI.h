//
// Created by edi on 27.11.2024.
//

#ifndef TUI_H
#define TUI_H


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
    void ShowMenu();
    // show meniu public
    // game loop
    // draw game
    // draw intermediary
    //
private:
    std::string         m_PlayerName1;
    std::string         m_PlayerName2;
    int                 m_GameChoice;
    std::vector<bool *> m_GameModesSelected;

    void StartGameMenu();
};


#endif // TUI_H
