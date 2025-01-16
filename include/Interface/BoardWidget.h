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

signals:
    void BoardSlotClicked(int x, int y);

private:
    Board m_Board;
    int   m_Size;
};

#endif // ETER_BOARDWIDGET_H
