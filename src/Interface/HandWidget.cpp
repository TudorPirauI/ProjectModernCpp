//
// Created by Tudor on 1/5/2025.
//

#include "Interface/HandWidget.h"
#include "GameComponents/Card.h"

HandWidget::HandWidget(QWidget *parent) : QWidget(parent), m_SelectedCardIndex(-1) {}

void HandWidget::SetCards(const std::vector<Card> &cards) {
    m_Cards = cards;

    qDeleteAll(m_Buttons);
    m_Buttons.clear();

    int           x          = 10;
    constexpr int y          = 10;
    constexpr int cardWidth  = 100;
    constexpr int cardHeight = 150;
    constexpr int spacing    = 10;

    for (int i = 0; i < m_Cards.size(); ++i) {
        std::cout << m_Cards[i].GetValue() << ' ';
        auto button = new QPushButton(QString::number(m_Cards[i].GetValue()), this);

        button->setGeometry(x, y, cardWidth, cardHeight);
        button->setCheckable(true);

        connect(button, &QPushButton::clicked, [this, i] {
            m_SelectedCardIndex = i;

            emit CardSelected(m_Cards[i].GetValue());

            for (int j = 0; j < m_Buttons.size(); ++j) {
                m_Buttons[j]->setChecked(j == m_SelectedCardIndex);
            }
        });

        m_Buttons.push_back(button);
        x += cardWidth + spacing;
    }

    std::cout << '\n';

    update();
}
