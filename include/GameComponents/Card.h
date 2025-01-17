#ifndef CARD_H
#define CARD_H

using Position = std::pair<int, int>;

enum class PlayerTurn { Player1, Player2, Granite };

std::ostream &operator<<(std::ostream &os, const PlayerTurn &playerTurn);

class Card {
    int        m_Value;
    int        m_OnModifier{0};
    PlayerTurn m_PlacedBy : 2;
    bool       m_IsIllusion : 1 {false};
    bool       m_IsEter : 1 {false};
    bool       m_IsFlipped : 1 {false};
    bool       m_IsGranite : 1 {false};

public:
    explicit Card(int value);
    Card();
    Card(int value, PlayerTurn playerTurn);

    Card(const Card &other)                = default;
    Card(Card &&other) noexcept            = default;
    Card &operator=(const Card &other)     = default;
    Card &operator=(Card &&other) noexcept = default;
    ~Card()                                = default;

    void              SetValue(int value);
    [[nodiscard]] int GetValue() const;

    void               SetIllusion(bool isIllusion);
    [[nodiscard]] bool GetIsIllusion() const;

    void               SetEter(bool isEter);
    [[nodiscard]] bool GetIsEter() const;

    [[nodiscard]] bool GetIsFlipped() const;
    void               SetIsFlipped(bool isFlipped);

    [[nodiscard]] PlayerTurn GetPlacedBy() const;
    void                     SetPlacedBy(PlayerTurn placedBy);

    [[nodiscard]] bool GetIsGranite() const;
    void               SetIsGranite(bool isGranite);

    friend std::ostream &operator<<(std::ostream &os, const Card &card);
    bool                 operator==(const Card &other) const;

    [[nodiscard]] int GetModifier() const;
    void              SetModifier(int value);

    // friend void to_json(nlohmann::json &j, const Card &c);
    // friend void from_json(const nlohmann::json &j, Card &c);
};
#endif // CARD_H
