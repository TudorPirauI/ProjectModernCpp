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
    explicit DisplayBoard(QWidget *parent = nullptr, int lines = 10, int columns = 10);
    void setBoard(const Board& board);
    void onDraw();
    void insertCard(const Card& card, const std::pair<int, int>& position, PlayerTurn turn);

public slots:
    void placeCard(const Card& card);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Board m_Board;
    int m_Lines;
    int m_Columns;
    Card m_SelectedCard;
    bool m_CardSelected;
};

#endif // ETER_DISPLAYBOARD_H