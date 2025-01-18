//
// Created by Tudor on 1/5/2025.
//

#include "Interface/BoardWidget.h"
#include "GameComponents/Board.h"
#include "GameComponents/Card.h"

BoardWidget::BoardWidget(QWidget *parent, const int size) : QWidget(parent), m_Board(size) {}

void BoardWidget::SetBoard(const Board &board) {
    m_Board = board;

    if (layout() != nullptr) {
        QLayoutItem *item;
        while ((item = layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout();
    }

    auto [leftX, leftY]   = board.GetLeft();
    auto [rightX, rightY] = board.GetRight();
    auto [downX, downY]   = board.GetDown();
    auto [upX, upY]       = board.GetUp();

    if (!board.IsBoardLocked()) {
        ++rightX;
        ++downY;
        --leftX;
        --upY;
    }
    auto *layout = new QGridLayout(this);

    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    const auto &gameBoard = m_Board.GetGameBoard();

    for (int j = upY; j <= downY; ++j) {
        for (int i = leftX; i <= rightX; ++i) {
            const auto button = new QPushButton();
            button->setFixedSize(150, 150);

            const auto placedCard = gameBoard.find({i, j});

            if (placedCard == gameBoard.end()) {
                if (!board.IsPositionValid({i, j}, Card(2))) {
                    button->setEnabled(false);
                }
            } else {
                const int playerId =
                        placedCard->second.top().GetPlacedBy() == PlayerTurn::Player1 ? 1 : 2;
                QString imagePath = QString(":/simple-%1-%2")
                                            .arg(playerId)
                                            .arg(placedCard->second.top().GetValue());
                QPixmap pixmap(imagePath);

                QIcon buttonIcon(pixmap);
                button->setIcon(buttonIcon);
                button->setIconSize(button->size());
            }

            connect(button, &QPushButton::clicked, [this, i, j] { emit BoardSlotClicked(i, j); });

            layout->addWidget(button, j - upY, i - leftX);
        }
    }

    setLayout(layout);

    update();
}
