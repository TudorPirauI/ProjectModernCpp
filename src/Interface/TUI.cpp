//
// Created by edi on 27.11.2024.
//

#include "TUI.h"

#include <fstream>

void TUI::ShowMenu() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    const std::vector<std::string> entries  = {"Start Game", "Options", "Exit"};
    int                            selected = 0;
    auto                           menu     = Menu(&entries, &selected);

    auto container = Container::Vertical({
            menu,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                text("Main Menu") | bold | center,
                separator(),
                menu->Render() | center,
        });
    });

    auto component = CatchEvent(renderer, [&](const Event &event) {
        if (event == Event::Return) {
            if (selected == 0) {
                screen.ExitLoopClosure()();
                std::system("clear");
                StartGameMenu();
            } else if (selected == 1) {
                std::cout << "Opening options...\n";
                // Add options handling here
            } else if (selected == 2) {
                screen.ExitLoopClosure()();
            }
        }
        return false;
    });

    screen.Loop(component);
}

void TUI::StartGameMenu() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    auto          input_player1 = Input(&m_PlayerName1, "Player 1 Name");
    auto          input_player2 = Input(&m_PlayerName2, "Player 2 Name");
    std::ofstream fout("log.txt");

    std::vector<std::string> gameModes = {"Antrenament", "Duelul Vrajitorilor", "Duelul Elementelor", "Turneu",
                                          "Rapid"};

    std::vector<Component> checkboxes;

    for (size_t i = 0; i < gameModes.size(); ++i) {
        checkboxes.push_back(Checkbox(gameModes[i], m_GameModesSelected[i]));
    }

    auto finish_button = Button("Finish", screen.ExitLoopClosure());

    auto container = Container::Vertical({input_player1, input_player2, Container::Vertical(checkboxes)});

    auto renderer = Renderer(container, [&] {
        Elements checkbox_elements;
        for (size_t i = 0; i < gameModes.size(); ++i) {
            checkbox_elements.push_back(checkboxes[i]->Render());
        }
        return vbox({
                text("Enter Player Names") | bold | center,
                separator(),
                hbox(text("Player 1: "), input_player1->Render()) | center,
                hbox(text("Player 2: "), input_player2->Render()) | center,
                separator(),
                vbox(checkbox_elements) | center,
                separator(),
                finish_button->Render(),
        });
    });

    auto component = CatchEvent(renderer, [&](const Event &event) {
        if (event == Event::Return) {
            std::cout << "Player 1: " << m_PlayerName1 << "\n";
            std::cout << "Player 2: " << m_PlayerName2 << "\n";
            for (size_t i = 0; i < gameModes.size(); ++i) {
                std::cout << gameModes[i] << ": " << (m_GameModesSelected[i] ? "Selected" : "Not Selected") << "\n";
            }
            screen.ExitLoopClosure()();
        }
        return false;
    });

    screen.Loop(component);
}
