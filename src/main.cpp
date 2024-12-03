#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <iostream>
#include <ranges>
#include <thread>

#include "Card/Card.h"
#include "GameBoard/Board.h"
#include "Interface/TUI.h"

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

    for (const auto &line : std::views::split(ascii, '\n')) {
        std::cout << std::string(line.begin(), line.end()) << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    std::system("clear");
}

int main() {
    TUI tui;

    tui.ShowMenu();
    return 0;
}

// todo: backend -> problema la indici la castig (conditie castig sau player)
// todo: rule of 0 (5)

/*
    backend
    1. sa terminam antrenamentul
    2. duelul vrajitorilor
    3. POATE duelul elementelor (doamne ajuta)
    4. teste ! pentru joc;)
    5. restructurare game si antrenament :)
 */
