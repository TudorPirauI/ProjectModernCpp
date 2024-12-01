//
// Created by edi on 28.11.2024.
//

#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include "../GameBoard/Board.h"
#include "../Player/Player.h"

class Game {
public:
    enum class GameState { NotFinished, NormalWin, FinishedCards };

    virtual ~Game() = default;
    Game(int boardSize, int scoreToWin, const std::string &nameOne, const std::string &nameTwo);
    Game();

    [[nodiscard]] Board                        &GetBoard();
    [[nodiscard]] Player                       &GetPlayer1();
    [[nodiscard]] Player                       &GetPlayer2();
    [[nodiscard]] GameState                     GetGameState() const;
    [[nodiscard]] std::unordered_map<int, int> &GetLineAdvantage();
    [[nodiscard]] std::unordered_map<int, int> &GetColumnAdvantage();
    [[nodiscard]] int                           GetPlayer1Score() const;
    [[nodiscard]] int                           GetPlayer2Score() const;
    [[nodiscard]] int                           GetScoreToWin() const;
    [[nodiscard]] PlayerTurn                    GetCurrentPlayer() const;
    bool                                        CheckWinningConditions();
    void                                        SetGameState(GameState gameState);
    void                                        AddWinedGames(PlayerTurn turn);
    void                                        SetNextPlayerTurn(PlayerTurn playerTurn);

protected:
    Board                        m_Board;
    Player                       m_Player1;
    Player                       m_Player2;
    PlayerTurn                   m_PlayerTurn{PlayerTurn::Player1};
    GameState                    m_GameState{GameState::NotFinished};
    std::unordered_map<int, int> m_Lines;
    std::unordered_map<int, int> m_Columns;
    int                          m_ScorePlayer1{0};
    int                          m_ScorePlayer2{0};
    int                          m_ScoreToWin{};
    // virtual void GameLoop() = 0; // to be implemented
};

#endif // GAME_H
