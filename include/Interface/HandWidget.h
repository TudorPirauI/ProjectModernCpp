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

    [[nodiscard]] int GetIdealWidth() const;

signals:
    void CardSelected(int cardValue);

private:
    std::vector<Card>          m_Cards;
    std::vector<QPushButton *> m_Buttons;
    int                        m_SelectedCardIndex;

    static constexpr int m_YStart      = 10;
    static constexpr int m_CardWidth   = 100;
    static constexpr int m_CardHeight  = 150;
    static constexpr int m_CardSpacing = 10;
};

#endif // ETER_HANDWIDGET_H
