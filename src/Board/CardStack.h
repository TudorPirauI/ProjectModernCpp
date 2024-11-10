//
// Created by mavri on 10.11.2024.
//

#ifndef CARDSTACK_H
#define CARDSTACK_H
#include <optional>
#include <stack>

#include "../Card.h"

class CardStack {
    std::stack<Card> stack;

public:
    void               addCard(const Card &card);
    [[nodiscard]] std::optional<Card> getTopCard() const;
    [[nodiscard]] bool isCovered() const;
};


#endif // CARDSTACK_H
