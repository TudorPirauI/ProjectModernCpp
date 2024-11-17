#ifndef CARD_H
#define CARD_H

#include <utility>

using Position = std::pair<int, int>;

class Card {
    int  m_Value;
    bool m_PlacedBy : 1 {};
    bool m_IsIllusion : 1;
    bool m_IsEter : 1;
    bool m_isFlipped : 1 {false};

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
};


#endif // CARD_H
