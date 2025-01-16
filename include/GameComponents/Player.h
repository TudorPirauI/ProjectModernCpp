#ifndef PLAYER_H
#define PLAYER_H

#include "GameComponents/Card.h"

using Hand = std::vector<Card>;

class Player final {
    std::string m_UserName;
    int         m_Score;
    int         m_NumberOfIllusion{1};
    Hand        m_Hand;
    Hand        m_RemovedCards;

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
    void GiveEterCard(PlayerTurn playerTurn);

    int  GetIllusion();
    void SetIllusion(int value);

    void AddToRemovedCards(const Card &card);
    Hand GetRemovedCards();
    void RemoveFromRemovedCards(const Card &card);
};

#endif // PLAYER_H
