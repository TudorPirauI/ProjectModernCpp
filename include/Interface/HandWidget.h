//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_HANDWIDGET_H
#define ETER_HANDWIDGET_H

#include "GameComponents/Card.h"
#include "pch.h"

class HandWidget final : public QWidget {
    Q_OBJECT

public:
    explicit HandWidget(QWidget *parent = nullptr);
    void               SetCards(const std::vector<Card> &cards);
    void               OnDraw();
    [[nodiscard]] Card GetSelectedCard() const;

signals:
    void CardSelected(int cardValue);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<Card> m_Cards;
    int               m_SelectedCardIndex;
};

#endif // ETER_HANDWIDGET_H
