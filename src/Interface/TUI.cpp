//
// Created by edi on 27.11.2024.
//

#include "TUI.h"

#include <fstream>

TUI::TUI() : m_GameChoice{0} {};

using namespace ftxui;

Component Wrap(std::string name, Component component) {
    return Renderer(component, [name, component] {
        return hbox({
                       text(name) | size(WIDTH, EQUAL, name.size()),
                       separator(),
                       component->Render() | xflex,
               }) |
               xflex;
    });
}

void TUI::ShowMenu() {
    using namespace ftxui;

    auto m_Screen = ScreenInteractive::Fullscreen();

    const std::vector<std::string> entries  = {"Start Game", "Options", "Exit"};
    int                            selected = 0;
    auto                           menu     = Menu(&entries, &selected);

    const auto container = Container::Vertical({menu});

    const auto renderer = Renderer(container, [&] {
        return vbox({
                text("Main Menu") | bold | center,
                separator(),
                menu->Render() | center,
        });
    });

    const auto close = m_Screen.ExitLoopClosure();

    const auto component = CatchEvent(renderer, [&](const Event &event) {
        if (event == Event::Return) {
            if (selected == 0) {
                close();
                StartGameMenu();
                return true;
            }
            if (selected == 1) {
                std::cout << "Opening options...\n";
                // Add options handling here
                return true;
            }
            if (selected == 2) {
                m_Screen.ExitLoopClosure()();
                return true;
            }
        }
        return false;
    });

    m_Screen.Loop(component);
}

void TUI::StartGameMenu() {
    using namespace ftxui;

    auto m_Screen = ScreenInteractive::Fullscreen();

    const auto input_player1 = Input(&m_PlayerName1, "Player 1 Name");
    const auto input_player2 = Input(&m_PlayerName2, "Player 2 Name");

    const std::vector<std::string> gameModes = {"Antrenament", "Duelul Vrajitorilor", "Duelul Elementelor", "Turneu",
                                                "Rapid"};
    std::array<bool, 5>            gameModesSelected{false};

    const auto input_container = Container::Vertical({
            Wrap("Player One: ", input_player1),
            Wrap("Player Two: ", input_player2),
    });

    const auto checkbox_container = Container::Vertical({});
    for (size_t i = 0; i < gameModes.size(); ++i) {
        checkbox_container->Add(Checkbox(gameModes[i], &gameModesSelected[i]));
    }

    const auto finish_button = Button("Start", [&] {
        std::cout << "Player 1: " << m_PlayerName1 << "\n";
        std::cout << "Player 2: " << m_PlayerName2 << "\n";
        for (size_t i = 0; i < gameModes.size(); ++i) {
            std::cout << gameModes[i] << ": " << (gameModesSelected[i] ? "Selected" : "Not Selected") << "\n";
        }
        m_Screen.ExitLoopClosure()();
    });

    /*
    return Renderer(component, [name, component] {
        return hbox({
                       text(name) | size(WIDTH, EQUAL, name.size()),
                       separator(),
                       component->Render() | xflex,
               }) |
               xflex;
    });
     */

    const auto main_container = Container::Vertical(
            {Renderer(input_container,
                      [&] {
                          return hbox({
                                         input_container->Render() | xflex,
                                 }) |
                                 xflex | border;
                      }),
             Renderer(checkbox_container,
                      [&] {
                          return vbox({text("Mod de joc") | bold, checkbox_container->Render() | xflex}) | xflex |
                                 border;
                      }),
             Renderer(finish_button,
                      [&] { return vbox({finish_button->Render() | xflex | size(WIDTH, LESS_THAN, 10)}) | center; })});

    const auto renderer = Renderer(main_container, [&] {
        return vbox({
                text("Setup Game") | bold | center,
                main_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20),
        });
    });

    m_Screen.Loop(renderer);
}
