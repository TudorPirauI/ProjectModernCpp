#include <iostream>
#include <ranges>
#include <thread>

#include "Game/Antrenament.h"
#include "Game/Game.h"

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

    // todo
    auto  game         = Antrenament("Player1", "Player2");
    auto &board        = game.GetBoard();
    auto  turn         = PlayerTurn::Player1;
    auto  cardsPlayer1 = game.GetPlayer1().GetHand();
    auto  cardsPlayer2 = game.GetPlayer2().GetHand();

    std::vector<Position> positions = {{0, 0}, {1, 1}, {2, 2}};

    for (const auto &pos : positions) {
        board.InsertCard(cardsPlayer1[0], pos, turn);
        auto left  = board.GetLeft();
        auto right = board.GetRight();
        auto down  = board.GetDown();
        auto up    = board.GetUp();

        std::cout << "Left: (" << left.first << ", " << left.second << ")\n";
        std::cout << "Right: (" << right.first << ", " << right.second << ")\n";
        std::cout << "Down: (" << down.first << ", " << down.second << ")\n";
        std::cout << "Up: (" << up.first << ", " << up.second << ")\n";

        std::cout << "Status Locked: " << board.IsBoardLocked() << '\n';
    }

    std::cout << game.CheckWinningConditions();

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
