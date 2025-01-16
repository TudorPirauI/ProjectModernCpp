//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_HANDWIDGET_H
#define ETER_HANDWIDGET_H

#include "GameComponents/Card.h"

class HandWidget final : public QWidget {
    Q_OBJECT

public:
    explicit HandWidget(QWidget *parent = nullptr);

    void SetCards(const std::vector<Card> &cards);

signals:
    void CardSelected(int cardValue);

private:
    std::vector<Card>          m_Cards;
    std::vector<QPushButton *> m_Buttons;
    int                        m_SelectedCardIndex;
};

#endif // ETER_HANDWIDGET_H
