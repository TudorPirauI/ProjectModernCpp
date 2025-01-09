//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_DISPLAYHAND_H
#define ETER_DISPLAYHAND_H

#include <QWidget>
#include <vector>
#include "GameComponents/Card.h"

class DisplayHand final : public QWidget {
    Q_OBJECT

public:
    explicit DisplayHand(QWidget *parent = nullptr);
    void               SetCards(const std::vector<Card> &cards);
    void               OnDraw();
    [[nodiscard]] Card GetSelectedCard() const;

signals:
    void CardSelected(const Card &card);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<Card> m_Cards;
    int               m_SelectedCardIndex;
};

#endif // ETER_DISPLAYHAND_H
