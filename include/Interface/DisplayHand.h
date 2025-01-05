//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_DISPLAYHAND_H
#define ETER_DISPLAYHAND_H

#include <QWidget>
#include <vector>
#include "GameComponents/Card.h"

class DisplayHand : public QWidget {
Q_OBJECT

public:
    explicit DisplayHand(QWidget *parent = nullptr);
    void setCards(const std::vector<Card>& cards);
    void onDraw();
    Card getSelectedCard() const;

signals:
    void cardSelected(const Card& card);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<Card> m_Cards;
    int m_SelectedCardIndex;
};

#endif // ETER_DISPLAYHAND_H