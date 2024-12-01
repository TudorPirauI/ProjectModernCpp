#ifndef CARD_H
#define CARD_H

#include <ostream>
#include <utility>

using Position = std::pair<int, int>;
enum class PlayerTurn { Player1, Player2 };

class Card {
    int        m_Value;
    PlayerTurn m_PlacedBy : 1 {};
    bool       m_IsIllusion : 1;
    bool       m_IsEter : 1;
    bool       m_isFlipped : 1 {false};

public:
    explicit Card(int value);

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

    friend std::ostream &operator<<(std::ostream &os, const Card &card);
    bool                 operator==(const Card &other) const;
};
#endif // CARD_H
