//
// Created by Tudor on 1/5/2025.
//

#include "Interface/DisplayHand.h"
#include "GameComponents/Card.h"
#include <QPainter>

DisplayHand::DisplayHand(QWidget *parent) : QWidget(parent) {}

void DisplayHand::setCards(const std::vector<Card>& cards) {
    m_Cards = cards;
    update(); // Trigger a repaint
}

void DisplayHand::onDraw() {
    // This function can be used to trigger a repaint if needed
    update();
}

void DisplayHand::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    int x = 10;
    int y = 10;
    int cardWidth = 100;
    int cardHeight = 150;
    int spacing = 10;

    for (const auto& card : m_Cards) {
        painter.drawRect(x, y, cardWidth, cardHeight);
        painter.drawText(x + 10, y + 20, QString::number(card.GetValue()));
        x += cardWidth + spacing;
    }
}