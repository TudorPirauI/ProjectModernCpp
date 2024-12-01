
//
// Created by edi on 27.11.2024.
//

#include "TUI.h"

#include <fstream>
#include <ranges>

#include "../Game/Antrenament.h"
#include "ftxui-grid-container/grid-container.hpp"

TUI::TUI() = default;

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

/*
* Position Board::ShowTableWithInput() const {
    using namespace ftxui;
    auto [left, up, down, right] = m_Corners;

    if (!IsBoardLocked()) {
        ++right.first;
        ++down.second;
        --left.first;
        --up.second;
    }

    Position pos;

    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::vector<Component>> grid;
    for (int j = up.second; j <= down.second; ++j) {
        std::vector<Component> row;
        for (int i = left.first; i <= right.first; ++i) {
            const auto  it = m_Board.find({i, j});
            std::string cellContent;
            Decorator   cellDecorator = nothing;

            if (it == m_Board.end()) {
                if (IsPositionValid({i, j}, Card(2))) {
                    cellContent   = " V ";
                    cellDecorator = color(Color::Green);
                } else {
                    cellContent   = " X ";
                    cellDecorator = color(Color::Red);
                }
            } else {
                const auto &card = it->second.top();
                if (card.GetIsFlipped()) {
                    cellContent   = " H ";
                    cellDecorator = color(Color::Yellow);
                } else {
                    cellContent   = " " + std::to_string(card.GetValue()) + " ";
                    cellDecorator = color(Color::Blue);
                }
            }

            auto cell = Button(cellContent,
                               [&pos, i, j, &screen] {
                                   pos = {i, j};
                                   screen.Exit();

                                   return true;
                               }) |
                        cellDecorator;
            row.push_back(cell);
        }
        grid.push_back(row);
    }

    const auto gridContainer = GridContainer(grid);

    screen.Loop(gridContainer | center);

    return pos;
}
 */

Element ToElement(const Component &component) { return component->Render(); }

Component WithRenderer(const Component &component, const std::function<Element()> &renderer) {
    return Renderer(component, renderer);
}

Components ToLineRenderer(const std::vector<Component> &components) {
    Components result;
    for (const auto &component : components) {
        result.push_back(component | xflex);
    }
    return result;
}

constexpr std::string INVALID_SPOT = " X ";
constexpr std::string VALID_SPOT   = " V ";

std::vector<std::vector<Component>> BuildGrid(const Board                         &board,
                                              const std::function<bool(int, int)> &callback) {
    auto [left, up, down, right] = board.GetCorners();

    if (!board.IsBoardLocked()) {
        ++right.first;
        ++down.second;
        --left.first;
        --up.second;
    }

    const auto                          boardElement = board.GetGameBoard();
    std::vector<std::vector<Component>> grid;

    for (int j = up.second; j <= down.second; ++j) {
        std::vector<Component> row;
        for (int i = left.first; i <= right.first; ++i) {
            const auto  it = boardElement.find({i, j});
            std::string cellContent;
            Decorator   cellDecorator = nothing;

            if (it == boardElement.end()) {
                if (board.IsPositionValid({i, j}, Card(2))) {
                    cellContent   = VALID_SPOT;
                    cellDecorator = color(Color::Green);
                } else {
                    cellContent   = INVALID_SPOT;
                    cellDecorator = color(Color::Red);
                }
            } else {
                const auto &card = it->second.top();
                if (card.GetIsFlipped()) {
                    cellContent   = " H ";
                    cellDecorator = color(Color::Yellow);
                } else {
                    cellContent = " " + std::to_string(card.GetValue()) + " ";
                    if (card.GetPlacedBy() == PlayerTurn::Player1) {
                        cellDecorator = bgcolor(Color::Red);
                    } else {
                        cellDecorator = bgcolor(Color::Blue);
                    }
                }
            }

            auto cell = Button(cellContent, [callback, i, j] { return callback(i, j); }) |
                        cellDecorator;
            row.push_back(cell);
        }
        grid.push_back(row);
    }

    return grid;
}

std::vector<std::vector<Component>>
BuildCardHand(const std::vector<Card>                 &playerHand,
              const std::function<bool(const Card &)> &callback) {
    std::vector<std::vector<Component>> handComponents(1);
    for (const auto &card : playerHand) {
        handComponents.at(0).push_back(Button(" " + std::to_string(card.GetValue()) + " ",
                                              [callback, card] { return callback(card); }));
    }
    return handComponents;
}

void TUI::GameLoopTraining() {
    auto game = dynamic_cast<Antrenament *>(m_Game.get());

    const auto turn     = game->GetCurrentPlayer();
    auto &currentPlayer = turn == PlayerTurn::Player1 ? game->GetPlayer1() : game->GetPlayer2();

    auto      &playerHand = currentPlayer.GetHand();
    const auto playScore  = currentPlayer.GetScore();
    const auto playerScoreTotal =
            turn == PlayerTurn::Player1 ? game->GetPlayer1Score() : game->GetPlayer2Score();
    const auto &playerName = currentPlayer.GetUserName();
    auto       &board      = game->GetBoard();

    auto screen = ScreenInteractive::Fullscreen();

    std::optional<Position> selectedPosition;
    std::optional<Card>     selectedCard;

    auto grid = BuildGrid(board, [&](int i, int j) {
        if (!selectedCard)
            return false;

        if (board.IsPositionValid({i, j}, selectedCard.value())) {
            selectedPosition = {i, j};
            screen.Clear();
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    auto handComponents = BuildCardHand(playerHand, [&](const Card &card) {
        selectedCard = card;
        return true;
    });

    const auto gridContainer = GridContainer(grid);

    const auto playerNameColor =
            (turn == PlayerTurn::Player1 ? bgcolor(Color::Red) : bgcolor(Color::Blue));
    const auto headerText = hbox({text(playerName) | bold | playerNameColor,
                                  text("'s Turn -- " + std::to_string(playerScoreTotal) + " / " +
                                       std::to_string(m_Game->GetScoreToWin())) |
                                          bold | center}) |
                            center;

    const auto grindHand   = GridContainer(handComponents);
    const auto playingArea = Container::Vertical({
            WithRenderer(gridContainer, [&] { return gridContainer->Render() | center; }),
            WithRenderer(grindHand, [&] { return grindHand->Render() | center; }),
    });

    const auto finalRenderer = Renderer(playingArea, [&] {
        return vbox({
                headerText,
                separator(),
                gridContainer->Render() | center,
                separator(),
                grindHand->Render() | center,
        });
    });

    screen.Loop(finalRenderer);

    // todo: checkbox for placing a card with Eter / Hidden

    if (selectedCard && selectedPosition) {
        bool insertCardResult = false;
        selectedCard->SetPlacedBy(turn);
        do {
            insertCardResult =
                    board.InsertCard(selectedCard.value(), selectedPosition.value(), turn);
        } while (insertCardResult != false);

        currentPlayer.RemoveCard(selectedCard.value());

        auto &lines   = game->GetLineAdvantage();
        auto &columns = game->GetColumnAdvantage();

        if (turn == PlayerTurn::Player1) {
            ++lines[selectedPosition.value().first];
            ++columns[selectedPosition.value().second];
        } else {
            --lines[selectedPosition.value().first];
            --columns[selectedPosition.value().second];
        }

        if (game->CheckWinningConditions() == false) {
            game->SetNextPlayerTurn(turn == PlayerTurn::Player1 ? PlayerTurn::Player2
                                                                : PlayerTurn::Player1);
            GameLoopTraining();
        } else {
            game->IncreasePlayerScore(turn);

            if (m_Game->GetPlayer1Score() < m_Game->GetScoreToWin() and
                m_Game->GetPlayer2Score() < m_Game->GetScoreToWin()) {
                RestartGame();
                screen.Exit();
                GameLoopTraining();
            } else {
                std::ofstream outFile("game_status.txt");
                if (outFile.is_open()) {
                    outFile << "The game is finished.\n";
                    outFile.close();
                } else {
                    std::cerr << "Unable to open file";
                }
                screen.Exit();
                RestartGame();
                StartGameMenu();
            }
        }
    }
}

void TUI::InitGame(const std::string &gameMode, const std::string &player1,
                   const std::string &player2) {
    // todo: replace this with the proper call to the virtual Game class

    m_Game = std::make_unique<Antrenament>(player1, player2);

    GameLoopTraining();
}

void TUI::StartGameMenu() {
    using namespace ftxui;

    int         gameChoice;
    std::string playerName1;
    std::string playerName2;

    auto screen = ScreenInteractive::Fullscreen();

    const auto inputPlayer1 = Input(&playerName1, "Player 1 Name");
    const auto inputPlayer2 = Input(&playerName2, "Player 2 Name");

    const std::vector<std::string> gameModes        = {"Antrenament", "Duelul Vrajitorilor",
                                                       "Duelul Elementelor", "Turneu", "Rapid"};
    int                            selectedGameMode = 0;

    const auto inputContainer = Container::Vertical({
            Wrap("Player One: ", inputPlayer1),
            Wrap("Player Two: ", inputPlayer2),
    });

    const auto radioboxContainer = Radiobox(&gameModes, &selectedGameMode);

    const auto finishButton = Button("Start", [&] {
        screen.ExitLoopClosure()();
        InitGame(gameModes[selectedGameMode], playerName1, playerName2);
    });

    const auto mainContainer = Container::Vertical({
            Renderer(inputContainer,
                     [&] {
                         return hbox({
                                        inputContainer->Render() | xflex,
                                }) |
                                xflex | border;
                     }),
            Renderer(radioboxContainer,
                     [&] {
                         return vbox({text("Mod de joc") | bold,
                                      radioboxContainer->Render() | xflex}) |
                                xflex | border;
                     }),
            Renderer(finishButton,
                     [&] {
                         return vbox({finishButton->Render() | xflex |
                                      size(WIDTH, LESS_THAN, 10)}) |
                                center;
                     }),
    });

    const auto renderer = Renderer(mainContainer, [&] {
        return vbox({
                text("Setup Game") | bold | center,
                mainContainer->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20),
        });
    });

    screen.Loop(renderer);
}

void TUI::RestartGame() const {
    if (dynamic_cast<Antrenament *>(m_Game.get())) {
        m_Game->SetNewCards();
    }
}
