// Player.ixx
#pragma once

#include <cstdint>
#include <vector>

#include "Card.h"

class Player {
public:
    enum class Difficulty { Easy, Medium, Hard, Impossible };

private:
    uint8_t           m_gameMode = 0;
    std::vector<Card> m_cards;
    uint8_t           m_score      = 0;
    bool              m_isAi       = false;
    Difficulty        m_difficulty = Difficulty::Easy;

public:
    Player() = default;
    Player(uint8_t gameMode, const std::vector<Card> &cards);

    uint8_t getGameMode() const;
    void    setGameMode(uint8_t gameMode);

    uint8_t getScore() const;
    void    setScore(uint8_t score);

    void calculateScore();
    void addCard(const Card &card);
    void playCard(size_t index);
    void makeMove();

    ~Player() = default;
};
