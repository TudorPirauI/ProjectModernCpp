//
// Created by Tudor on 1/5/2025.
//

#include "Interface/HandWidget.h"
#include "GameComponents/Card.h"

HandWidget::HandWidget(QWidget *parent) : QWidget(parent), m_SelectedCardIndex(-1) {
    setAttribute(Qt::WA_Hover);
}

int HandWidget::GetIdealWidth() const {
    const auto handSize = m_Cards.size();
    return handSize * m_CardWidth + handSize * m_CardSpacing;
}

void HandWidget::SetCards(const std::vector<Card> &cards) {
    m_Cards = cards;

    m_SelectedCardIndex = -1;

    if (layout() == nullptr) {
        const auto layout = new QHBoxLayout(this);
        setLayout(layout);
    } else {
        QLayoutItem *item;
        while ((item = layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    m_Buttons.clear();

    for (int i = 0; i < m_Cards.size(); ++i) {
        auto button = new QPushButton(this);
        button->setCheckable(true);
        button->setFixedSize(100, 100);

        const int playerId = static_cast<int>(m_Cards[i].GetPlacedBy()) + 1;

        QString imagePath = QString(":/simple-%1-%2").arg(playerId).arg(m_Cards[i].GetValue());
        if (m_Cards[i].GetIsEter()) {
            imagePath = QString(":/eter-%1").arg(playerId);
        }

        QPixmap pixmap(imagePath);

        QIcon buttonIcon(pixmap);
        button->setIcon(buttonIcon);
        button->setIconSize(button->size());

        // Set border color
        button->setStyleSheet("border: 2px solid red;");

        connect(button, &QPushButton::clicked, [this, i, button] {
            if (m_SelectedCardIndex == i) {
                return;
            }

            if (m_SelectedCardIndex != -1) {
                const auto resetAnimation =
                        new QPropertyAnimation(m_Buttons[m_SelectedCardIndex], "pos");
                resetAnimation->setDuration(200);
                resetAnimation->setStartValue(m_Buttons[m_SelectedCardIndex]->pos());
                resetAnimation->setEndValue(m_Buttons[m_SelectedCardIndex]->pos() + QPoint(0, 20));
                resetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }

            m_SelectedCardIndex = i;
            emit CardSelected(i);

            for (int j = 0; j < m_Buttons.size(); ++j) {
                m_Buttons[j]->setChecked(j == m_SelectedCardIndex);
            }

            const auto animation = new QPropertyAnimation(button, "pos");
            animation->setDuration(100);
            animation->setStartValue(button->pos());
            animation->setEndValue(button->pos() - QPoint(0, 20));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        });

        m_Buttons.push_back(button);

        layout()->addWidget(button);
    }

    update();
}
