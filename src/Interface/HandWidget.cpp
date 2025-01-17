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

#include <QDebug>
#include <QEvent>
#include <QGraphicsEffect>
#include <QGraphicsProxyWidget>
#include <QGraphicsRotation>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPushButton>
#include <QtMath>

bool HandWidget::eventFilter(QObject *watched, QEvent *event) {
    if (auto *button = qobject_cast<QPushButton *>(watched)) {
        static QGraphicsRotation *rotationX = new QGraphicsRotation;
        static QGraphicsRotation *rotationY = new QGraphicsRotation;
        static QGraphicsScale    *scale     = new QGraphicsScale;
        static QGraphicsEffect   *effect    = new QGraphicsEffect;

        if (event->type() == QEvent::Enter) {
            button->setGraphicsEffect(effect);
            scale->setXScale(1.1);
            scale->setYScale(1.1);
        } else if (event->type() == QEvent::Leave) {
            button->setGraphicsEffect(nullptr); // Eliminarea efectului
        } else if (event->type() == QEvent::MouseMove) {
            auto   *mouseEvent = static_cast<QMouseEvent *>(event);
            QPointF pos        = mouseEvent->pos();
            QSize   size       = button->size();

            // Calcularea unghiurilor de rotație pe baza poziției cursorului
            qreal angleX = ((pos.y() / size.height()) - 0.5) * 30; // Între -15 și +15 grade
            qreal angleY = ((pos.x() / size.width()) - 0.5) * -30;

            rotationX->setAxis(Qt::XAxis);
            rotationX->setAngle(angleX);

            rotationY->setAxis(Qt::YAxis);
            rotationY->setAngle(angleY);

            // Aplicarea transformărilor
            button->setGraphicsEffect(effect);
            button->graphicsEffect().setTransformations({rotationX, rotationY, scale});
        }
    }
    return QWidget::eventFilter(watched, event);
}

void HandWidget::SetCards(const std::vector<Card> &cards) {
    m_Cards = cards;

    m_SelectedCardIndex = -1;

    if (layout() == nullptr) {
        auto layout = new QHBoxLayout(this);
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
        QString   imagePath =
                QString(":/images/player%1/%2.png").arg(playerId).arg(m_Cards[i].GetValue());
        QPixmap pixmap(imagePath);

        QIcon buttonIcon(pixmap);
        button->setIcon(buttonIcon);
        button->setIconSize(button->size());

        button->installEventFilter(this);

        connect(button, &QPushButton::clicked, [this, i, button] {
            if (m_SelectedCardIndex == i) {
                return; // Do nothing if the same card is clicked again
            }

            if (m_SelectedCardIndex != -1) {
                // Reset the position of the previously selected card
                QPropertyAnimation *resetAnimation =
                        new QPropertyAnimation(m_Buttons[m_SelectedCardIndex], "pos");
                resetAnimation->setDuration(200);
                resetAnimation->setStartValue(m_Buttons[m_SelectedCardIndex]->pos());
                resetAnimation->setEndValue(m_Buttons[m_SelectedCardIndex]->pos() +
                                            QPoint(0, 20)); // Lower the card back
                resetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }

            m_SelectedCardIndex = i;
            emit CardSelected(m_Cards[i].GetValue());

            for (int j = 0; j < m_Buttons.size(); ++j) {
                m_Buttons[j]->setChecked(j == m_SelectedCardIndex);
            }

            // Raise the newly selected card slightly
            QPropertyAnimation *animation = new QPropertyAnimation(button, "pos");
            animation->setDuration(200);
            animation->setStartValue(button->pos());
            animation->setEndValue(button->pos() - QPoint(0, 20)); // Raise the card slightly
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        });

        m_Buttons.push_back(button);

        layout()->addWidget(button);
    }

    update();
}
