#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <iostream>
#include <ranges>
#include <thread>
#include <vector>

#include "Card/Card.h"
#include "GameBoard/Board.h"

void PrintAsciiArt() {
    const std::string ascii = R"(
.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.
(                 _____             _____                    _____                    _____                   )
 )               /\    \           /\    \                  /\    \                  /\    \                 (
(               /::\    \         /::\    \                /::\    \                /::\    \                 )
 )             /::::\    \        \:::\    \              /::::\    \              /::::\    \               (
(             /::::::\    \        \:::\    \            /::::::\    \            /::::::\    \               )
 )           /:::/\:::\    \        \:::\    \          /:::/\:::\    \          /:::/\:::\    \             (
(           /:::/__\:::\    \        \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \             )
 )         /::::\   \:::\    \       /::::\    \      /::::\   \:::\    \      /::::\   \:::\    \           (
(         /::::::\   \:::\    \     /::::::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \           )
 )       /:::/\:::\   \:::\    \   /:::/\:::\    \  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\____\         (
(       /:::/__\:::\   \:::\____\ /:::/  \:::\____\/:::/__\:::\   \:::\____\/:::/  \:::\   \:::|    |         )
 )      \:::\   \:::\   \::/    //:::/    \::/    /\:::\   \:::\   \::/    /\::/   |::::\  /:::|____|        (
(        \:::\   \:::\   \/____//:::/    / \/____/  \:::\   \:::\   \/____/  \/____|:::::\/:::/    /          )
 )        \:::\   \:::\    \   /:::/    /            \:::\   \:::\    \            |:::::::::/    /          (
(          \:::\   \:::\____\ /:::/    /              \:::\   \:::\____\           |::|\::::/    /            )
 )          \:::\   \::/    / \::/    /                \:::\   \::/    /           |::| \::/____/            (
(            \:::\   \/____/   \/____/                  \:::\   \/____/            |::|  ~|                   )
 )            \:::\    \                                 \:::\    \                |::|   |                  (
(              \:::\____\                                 \:::\____\               \::|   |                   )
 )              \::/    /                                  \::/    /                \:|   |                  (
(                \/____/                                    \/____/                  \|___|                   )
 )                                                                                                           (
(                                                                                                             )
 "+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"+.+"
    )";

    for (const auto &line: std::views::split(ascii, '\n')) {
        std::cout << std::string(line.begin(), line.end()) << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    std::system("clear");
}

// using namespace ftxui; // blah blah bad practice blah blah

int main() {
    Board board;
    /*
        sa mearga modul antrenament
            afisare player carti
            winning conditions
            gandire mod de actionare
            turn
            gandire metoda de input mai buna
            cum sa incepem jocu : programu sa te intrebe ceva */


    // auto screen = ScreenInteractive::TerminalOutput();
    //
    // const std::vector<std::string> entries = {"Start Game", "Options", "Exit"};
    //
    // int        selected = 0;
    // const auto menu     = Menu(&entries, &selected);
    //
    // const auto renderer = Renderer(menu, [&] {
    //     return vbox({
    //             text("Main Menu") | bold | center,
    //             separator(),
    //             menu->Render() | center,
    //     });
    // });
    //
    // const auto component = CatchEvent(renderer, [&](const Event &event) {
    //     if (event == Event::Return) {
    //         if (selected == 0) {
    //             std::cout << "Starting game...\n";
    //             screen.ExitLoopClosure()();
    //
    //         } else if (selected == 1) {
    //             std::cout << "Opening options...\n";
    //             // Add options handling here
    //         } else if (selected == 2) {
    //             screen.ExitLoopClosure()();
    //         }
    //     }
    //     return false;
    // });
    //
    // screen.Loop(component);


    const Card cardOne(1);
    const Card cardTwo(2);
    Card       cardThree(3);
    const Card cardFour(4);
    Card       cardFive(1);

    cardFive.SetEter(true);
    cardThree.SetIllusion(true);
    cardThree.SetIsFlipped(true);

    PrintAsciiArt();

    const std::vector cards = {cardOne, cardTwo, cardThree, cardFour, cardFive};

    const std::vector<Position> positions = {{0, 0}, {0, 1}, {-1, 1}, {-2, 1}, {0, 2}};

    for (size_t i = 0; i < cards.size(); ++i) {
        const auto res = board.InsertCard(cards[i], positions[i]);

        if (!res) {
            std::cout << "Failed to insert card " << i + 1 << std::endl;
        }
    }

    board.PrintTable();

    return 0;
}
