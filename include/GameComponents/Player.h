#ifndef PLAYER_H
#define PLAYER_H

#include "GameComponents/Card.h"

using Hand = std::vector<Card>;

class Player final {
    std::string m_UserName;
    int         m_Score;
    int         m_NumberOfIllusion{1};
    bool        m_HasIllusionInGame{false};
    bool        m_HasExplosion{false};
    Hand        m_Hand;
    Hand        m_RemovedCards;

public:
    Player(std::string name, Hand hand);

    Player()                                   = default;
    Player(const Player &other)                = default;
    Player(Player &&other) noexcept            = default;
    Player &operator=(const Player &other)     = default;
    Player &operator=(Player &&other) noexcept = default;
    ~Player()                                  = default;

    [[nodiscard]] const Hand        &GetHand() const;
    [[nodiscard]] const std::string &GetUserName() const;
    [[nodiscard]] int                GetScore() const;
    void                             ShowCards();

    void SetHand(const Hand &hand);
    void GiveCard(const Card &card);
    void RemoveCard(const Card &card);
    void GiveEterCard(PlayerTurn playerTurn);

    [[nodiscard]] int GetIllusion() const;
    void              SetIllusion(int value);

    [[nodiscard]] bool GetHasIllusionInGame() const;
    void               SetHasIllusionInGame(bool value);

    [[nodiscard]] bool GetHasExplosion() const;
    void               SetHasExplosion(bool value);

    [[nodiscard]] Hand GetRemovedCards() const;
    void               AddToRemovedCards(const Card &card);
    void               RemoveFromRemovedCards(const Card &card);

    // static void to_json(nlohmann::json &j, const Player &p);
    // static void from_json(const nlohmann::json &j, Player &p);
    //
    // void to_json(nlohmann::json &j, const Hand &hand);
    // void from_json(const nlohmann::json &j, Hand &hand);
};

#endif // PLAYER_H
