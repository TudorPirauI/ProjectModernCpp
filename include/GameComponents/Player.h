#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include <GameComponents/Card.h>

using Hand = std::vector<Card>;

class Player final {
    std::string m_UserName;
    int         m_Score;
    Hand        m_Hand;

public:
    Player(std::string name, Hand hand);
    Player(const Player &other)                = default;
    Player(Player &&other) noexcept            = default;
    Player &operator=(const Player &other)     = default;
    Player &operator=(Player &&other) noexcept = default;
    ~Player();

    [[nodiscard]] const Hand        &GetHand() const;
    [[nodiscard]] const std::string &GetUserName() const;
    [[nodiscard]] int                GetScore() const;
    void                             ShowCards();

    void SetHand(const Hand &hand);
    void GiveCard(const Card &card);
    void RemoveCard(const Card &card);
};

#endif // PLAYER_H
