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
    explicit BoardWidget(QWidget *parent = nullptr, int size = 3);

    void SetBoard(const Board &board);
    void OnDraw();

signals:
    void BoardSlotClicked(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Board m_Board;
    int   m_Size;
};

#endif // ETER_BOARDWIDGET_H
