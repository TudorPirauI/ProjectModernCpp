//
// Created by Tudor on 1/5/2025.
//

#include "Interface/DisplayBoard.h"
#include "GameComponents/Board.h"
#include <QPainter>
#include <QMouseEvent>

DisplayBoard::DisplayBoard(QWidget *parent, int lines, int columns)
        : QWidget(parent), m_Board(10), m_Lines(lines), m_Columns(columns), m_SelectedCard(1), m_CardSelected(false) {}

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

void DisplayBoard::placeCard(const Card& card) {
    m_SelectedCard = card;
    m_CardSelected = true;
}

void DisplayBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    int cellSize = 50;
    int xOffset = 10;
    int yOffset = 10;

    for (int row = 0; row < m_Lines; ++row) {
        for (int col = 0; col < m_Columns; ++col) {
            int x = xOffset + col * cellSize;
            int y = yOffset + row * cellSize;

            painter.drawRect(x, y, cellSize, cellSize);

            Position pos{col, row};
            if (m_Board.GetGameBoard().count(pos) && !m_Board.GetGameBoard().at(pos).empty()) {
                painter.drawText(x + 10, y + 20, QString::number(m_Board.GetGameBoard().at(pos).top().GetValue()));
            }
        }
    }
}

void DisplayBoard::mousePressEvent(QMouseEvent *event) {
    if (m_CardSelected) {
        int cellSize = 50;
        int xOffset = 10;
        int yOffset = 10;

        int col = (event->x() - xOffset) / cellSize;
        int row = (event->y() - yOffset) / cellSize;

        if (col >= 0 && col < m_Columns && row >= 0 && row < m_Lines) {
            insertCard(m_SelectedCard, {col, row}, PlayerTurn::Player1); // Assuming Player1 for simplicity
            m_CardSelected = false;
        }
    }
}