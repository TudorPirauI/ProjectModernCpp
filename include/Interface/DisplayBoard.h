//
// Created by Tudor on 1/5/2025.
//

#ifndef ETER_DISPLAYBOARD_H
#define ETER_DISPLAYBOARD_H

#include <QWidget>
#include <vector>
#include "GameComponents/Board.h"

class DisplayBoard : public QWidget {
Q_OBJECT

public:
    explicit DisplayBoard(QWidget *parent = nullptr);
    void setBoard(const Board& board);
    void onDraw();
    void insertCard(const Card& card, const std::pair<int, int>& position, PlayerTurn turn);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Board m_Board;
};

#endif // ETER_DISPLAYBOARD_H
