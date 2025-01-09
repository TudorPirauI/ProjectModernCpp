//
// Created by Tudor on 1/5/2025.
//

#include "Interface/DisplayHand.h"
#include <QMouseEvent>
#include <QPainter>
#include "GameComponents/Card.h"

DisplayHand::DisplayHand(QWidget *parent) : QWidget(parent), m_SelectedCardIndex(-1) {}

void DisplayHand::SetCards(const std::vector<Card> &cards) {
    m_Cards = cards;
    update();
}

void DisplayHand::OnDraw() { update(); }

Card DisplayHand::GetSelectedCard() const {
    if (m_SelectedCardIndex >= 0 && m_SelectedCardIndex < m_Cards.size()) {
        return m_Cards[m_SelectedCardIndex];
    }
    return Card(-1);
}

void DisplayHand::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    int      x          = 10;
    int      y          = 10;
    int      cardWidth  = 100;
    int      cardHeight = 100;
    int      spacing    = 10;

    for (int i = 0; i < m_Cards.size(); ++i) {
        if (i == m_SelectedCardIndex) {
            painter.setBrush(Qt::green);
        } else {
            painter.setBrush(Qt::NoBrush);
        }
        painter.drawRect(x, y, cardWidth, cardHeight);
        painter.drawText(x + 10, y + 20, QString::number(m_Cards[i].GetValue()));
        x += cardWidth + spacing;
    }
}

void DisplayHand::mousePressEvent(QMouseEvent *event) {
    int x          = 10;
    int y          = 10;
    int cardWidth  = 100;
    int cardHeight = 150;
    int spacing    = 10;

    for (int i = 0; i < m_Cards.size(); ++i) {
        QRect cardRect(x, y, cardWidth, cardHeight);
        if (cardRect.contains(event->pos())) {
            m_SelectedCardIndex = i;
            emit CardSelected(m_Cards[m_SelectedCardIndex]);
            update();
            break;
        }
        x += cardWidth + spacing;
    }
}
