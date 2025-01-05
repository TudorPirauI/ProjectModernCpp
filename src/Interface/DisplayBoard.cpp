//
// Created by Tudor on 1/5/2025.
//

#include "Interface/DisplayBoard.h"
#include <QPainter>

DisplayBoard::DisplayBoard(QWidget *parent) : QWidget(parent), m_Board(10) {}

void DisplayBoard::setBoard(const Board& board) {
    m_Board = board;
    update();
}

void DisplayBoard::onDraw() {
    update();
}

void DisplayBoard::insertCard(const Card& card, const std::pair<int, int>& position, PlayerTurn turn) {
    m_Board.InsertCard(card, position, turn);
    update();
}

void DisplayBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    int cellSize = 50;
    int xOffset = 10;
    int yOffset = 10;

    for (const auto& [pos, stack] : m_Board.GetGameBoard()) {
        int x = xOffset + pos.first * cellSize;
        int y = yOffset + pos.second * cellSize;

        painter.drawRect(x, y, cellSize, cellSize);
        if (!stack.empty()) {
            painter.drawText(x + 10, y + 20, QString::number(stack.top().GetValue()));
        }
    }
}