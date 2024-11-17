#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "../Card/Card.h"

using Hand = std::vector<Card>;

class Player final {
    std::string m_UserName;
    int         m_Score;
    Hand        m_Hand;

public:
             Player(std::string name, Hand hand);
    virtual ~Player();

    [[nodiscard]] const Hand        &getHand() const;
    [[nodiscard]] const std::string &getUserName() const;
    [[nodiscard]] int                getScore() const;

    // void playCard(const Card &card);
    void giveCard(const Card &card);

    // the functions for the AI
    // virtual void calculateMove(); // todo: this should return a set of moves
};


#endif // PLAYER_H
