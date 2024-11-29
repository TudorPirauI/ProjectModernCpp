//
// Created by edi on 27.11.2024.
//

#include "TUI.h"

#include <fstream>

TUI::TUI() : m_GameChoice{0} {};

using namespace ftxui;

Component Wrap(const std::string &name, const Component &component) {
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

    auto screen = ScreenInteractive::Fullscreen();

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

    const auto close = screen.ExitLoopClosure();

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
                screen.ExitLoopClosure()();
                return true;
            }
        }
        return false;
    });

    screen.Loop(component);
}

void TUI::StartGameMenu() {
    using namespace ftxui;

    auto m_Screen = ScreenInteractive::Fullscreen();

    const auto inputPlayer1 = Input(&m_PlayerName1, "Player 1 Name");
    const auto inputPlayer2 = Input(&m_PlayerName2, "Player 2 Name");

    const std::vector<std::string> gameModes = {"Antrenament", "Duelul Vrajitorilor",
                                                "Duelul Elementelor", "Turneu", "Rapid"};
    std::array<bool, 5>            gameModesSelected{false};

    const auto inputContainer = Container::Vertical({
            Wrap("Player One: ", inputPlayer1),
            Wrap("Player Two: ", inputPlayer2),
    });

    const auto checkboxContainer = Container::Vertical({});
    for (size_t i = 0; i < gameModes.size(); ++i) {
        checkboxContainer->Add(Checkbox(gameModes[i], &gameModesSelected[i]));
    }

    const auto finishButton = Button("Start", [&] {
        std::cout << "Player 1: " << m_PlayerName1 << "\n";
        std::cout << "Player 2: " << m_PlayerName2 << "\n";
        for (size_t i = 0; i < gameModes.size(); ++i) {
            std::cout << gameModes[i] << ": "
                      << (gameModesSelected[i] ? "Selected" : "Not Selected") << "\n";
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

    const auto mainContainer = Container::Vertical(
            {Renderer(inputContainer,
                      [&] {
                          return hbox({
                                         inputContainer->Render() | xflex,
                                 }) |
                                 xflex | border;
                      }),
             Renderer(checkboxContainer,
                      [&] {
                          return vbox({text("Mod de joc") | bold,
                                       checkboxContainer->Render() | xflex}) |
                                 xflex | border;
                      }),
             Renderer(finishButton, [&] {
                 return vbox({finishButton->Render() | xflex | size(WIDTH, LESS_THAN, 10)}) |
                        center;
             })});

    const auto renderer = Renderer(mainContainer, [&] {
        return vbox({
                text("Setup Game") | bold | center,
                mainContainer->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20),
        });
    });

    m_Screen.Loop(renderer);
}
