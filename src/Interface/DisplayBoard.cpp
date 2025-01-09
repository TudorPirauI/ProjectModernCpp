//
// Created by Tudor on 1/5/2025.
//

#include "Interface/DisplayBoard.h"
#include <QMouseEvent>
#include <QPainter>
#include "GameComponents/Board.h"

DisplayBoard::DisplayBoard(QWidget *parent, const int lines, const int columns) :
    QWidget(parent), m_Board(10), m_Lines(lines), m_Columns(columns), m_SelectedCard(1),
    m_CardSelected(false) {}

void DisplayBoard::SetBoard(const Board &board) {
    m_Board = board;
    update();
}

void DisplayBoard::OnDraw() { update(); }

void DisplayBoard::InsertCard(const Card &card, const std::pair<int, int> &position,
                              const PlayerTurn &turn) {
    m_Board.InsertCard(card, position, turn);
    update();
}

void DisplayBoard::PlaceCard(const Card &card) {
    m_SelectedCard = card;
    m_CardSelected = true;
}

void DisplayBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter      painter(this);
    constexpr int cellSize = 50;
    constexpr int xOffset  = 10;
    constexpr int yOffset  = 10;

    for (int row = 0; row < m_Lines; ++row) {
        for (int col = 0; col < m_Columns; ++col) {
            const int x = xOffset + col * cellSize;
            const int y = yOffset + row * cellSize;

            painter.drawRect(x, y, cellSize, cellSize);

            Position pos{col, row};
            if (m_Board.GetGameBoard().contains(pos) && !m_Board.GetGameBoard().at(pos).empty()) {
                painter.drawText(x + 10, y + 20,
                                 QString::number(m_Board.GetGameBoard().at(pos).top().GetValue()));
            }
        }
    }
}

void DisplayBoard::mousePressEvent(QMouseEvent *event) {
    if (m_CardSelected) {
        constexpr int cellSize = 50;
        constexpr int xOffset  = 10;
        constexpr int yOffset  = 10;

        const int col = (event->position().x() - xOffset) / cellSize;
        const int row = (event->position().y() - yOffset) / cellSize;

        if (col >= 0 && col < m_Columns && row >= 0 && row < m_Lines) {
            InsertCard(m_SelectedCard, {col, row},
                       PlayerTurn::Player1); // Assuming Player1 for simplicity
            m_CardSelected = false;
        }
    }
}
