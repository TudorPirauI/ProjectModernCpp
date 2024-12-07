//
// Created by edi on 28.11.2024.
//

#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include "../GameBoard/Board.h"
#include "../Player/Player.h"

#include <memory>

class Game {
public:
    enum class GameState { NotFinished, NormalWin, FinishedCards };

    virtual ~Game() = default;
    Game(int boardSize, int scoreToWin, const std::string &nameOne, const std::string &nameTwo);
    Game();

    [[nodiscard]] Board     &GetBoard();
    [[nodiscard]] Player    &GetPlayer1();
    [[nodiscard]] Player    &GetPlayer2();
    void                     SwitchPlayerTurn();
    [[nodiscard]] int        GetPlayer1Score() const;
    [[nodiscard]] int        GetPlayer2Score() const;
    [[nodiscard]] int        GetScoreToWin() const;
    [[nodiscard]] PlayerTurn GetCurrentPlayer() const;
    bool                     CheckWinningConditions();
    void                     SetGameState(GameState gameState);
    void                     IncreasePlayerScore(PlayerTurn turn);
    void                     SetNextPlayerTurn(PlayerTurn playerTurn);
    virtual void             SetNewCards() = 0;

    Game(const Game &other)                = default;
    Game(Game &&other) noexcept            = default;
    Game &operator=(const Game &other)     = default;
    Game &operator=(Game &&other) noexcept = default;

protected:
    Board      m_Board;
    Player     m_Player1;
    Player     m_Player2;
    PlayerTurn m_PlayerTurn{PlayerTurn::Player1};
    GameState  m_GameState{GameState::NotFinished};
    int        m_ScorePlayer1{0};
    int        m_ScorePlayer2{0};
    int        m_ScoreToWin{};
};

#endif // GAME_H
