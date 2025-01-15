//
// Created by Tudor on 1/5/2025.
//

#include "Interface/BoardWidget.h"
#include "GameComponents/Board.h"
#include "pch.h"

BoardWidget::BoardWidget(QWidget *parent, const int size) :
    QWidget(parent), m_Board(10), m_Size(size) {}

void BoardWidget::SetBoard(const Board &board) {
    m_Board = board;
    update();
}

void BoardWidget::OnDraw() { update(); }

void BoardWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter      painter(this);
    constexpr int cellSize = 50;
    constexpr int xOffset  = 10;
    constexpr int yOffset  = 10;

    // todo: this is poorly built, why wasn't the old version used?????

    for (int row = 0; row < m_Size; ++row) {
        for (int col = 0; col < m_Size; ++col) {
            const int x = xOffset + col * cellSize;
            const int y = yOffset + row * cellSize;

            // TODO: Just use buttons, why are we reinventing the wheel??
            painter.drawRect(x, y, cellSize, cellSize);

            Position pos{col, row};
            if (m_Board.GetGameBoard().contains(pos) && !m_Board.GetGameBoard().at(pos).empty()) {
                painter.drawText(x + 10, y + 20,
                                 QString::number(m_Board.GetGameBoard().at(pos).top().GetValue()));
            }
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
    constexpr int cellSize = 50;
    constexpr int xOffset  = 10;
    constexpr int yOffset  = 10;

    const int col = (event->position().x() - xOffset) / cellSize;
    const int row = (event->position().y() - yOffset) / cellSize;

    std::cout << "Clicked on " << col << ", " << row << std::endl;
    std::cout << "Board size: " << m_Size << std::endl;

    if (col < 0 || col >= m_Size || row < 0 || row >= m_Size)
        return;

    emit BoardSlotClicked(row, col);
    std::cout << "Signal emitted from BoardWidget for position: (" << row << ", " << col << ")"
              << std::endl;
}
