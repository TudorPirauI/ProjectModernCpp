#ifndef CARD_H
#define CARD_H

#include <utility>

using Position = std::pair<int, int>;

class Card {
    int m_Value;
    // std::optional<Position> m_Position;
    bool m_PlacedBy : 1 {};
    bool m_IsIllusion : 1;
    bool m_IsEter : 1;
    bool m_isFlipped : 1;

public:
    [[nodiscard]] bool GetIsFlipped() const;
    void  SetIsFlipped(bool isFlipped);

    explicit Card(int value);

    void              SetValue(int value);
    [[nodiscard]] int GetValue() const;

    void               SetIllusion(bool isIllusion);
    [[nodiscard]] bool GetIsIllusion() const;

    void               SetEter(bool isEter);
    [[nodiscard]] bool GetIsEter() const;

    // [[nodiscard]] std::optional<Position> GetPosition() const;
    // void                                  SetPosition(Position position);
};


#endif // CARD_H
