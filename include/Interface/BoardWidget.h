//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_BOARDWIDGET_H
#define ETER_BOARDWIDGET_H

#include "GameComponents/Board.h"
#include "pch.h"

class BoardWidget final : public QWidget {
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = nullptr, int lines = 10, int columns = 10);
    void SetBoard(const Board &board);
    void OnDraw();
    void InsertCard(const Card &card, const std::pair<int, int> &position, const PlayerTurn &turn);

public slots:
    void PlaceCard(const Card &card);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Board m_Board;
    int   m_Lines;
    int   m_Columns;
    Card  m_SelectedCard;
    bool  m_CardSelected;
};

#endif // ETER_BOARDWIDGET_H
