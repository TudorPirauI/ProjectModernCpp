#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "Game.h"
#include "GameComponents/Arena.h"

enum class Mode { A, B, C, BPlusC };

class Turneu {
public:
    Turneu(int boardSize, int scoreToWin, const std::string &nameOne, const std::string &nameTwo,
           const std::array<bool, 3> &options, Mode mode);

    void       Start();
    void       PlayGame();
    bool       CheckMatchWin() const;
    PlayerTurn GetMatchWinner() const;

private:
    std::unique_ptr<Game> m_Game;
    Arena                 m_Arena;
    Mode                  m_Mode;
    int                   m_GamesPlayed;
    int                   m_Player1Wins;
    int                   m_Player2Wins;

    void PlacePieceInArena(const Position &pos, PlayerTurn player);
};

#endif // TOURNAMENT_H
