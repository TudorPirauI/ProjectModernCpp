//
// Created by mavri on 10.11.2024.
//

#include "CardStack.h"

void CardStack::addCard(const Card &card) { stack.push(card); }

std::optional<Card> CardStack::getTopCard() const { return stack.top(); }

bool CardStack::isCovered() const { return stack.top().isHidden(); }
