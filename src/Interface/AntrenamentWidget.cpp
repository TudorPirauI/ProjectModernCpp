#include <Interface/AntrenamentWidget.h>

#include <QPushButton>
#include <iostream>

AntrenamentWidget::AntrenamentWidget(const std::string &nameOne, const std::string &nameTwo,
                                     QStackedWidget *stackedWidget, QWidget *parent) :
    m_AntrenamentWidget(new QWidget(parent)), m_Layout(new QVBoxLayout(m_AntrenamentWidget)),
    m_StackedWidget(stackedWidget), m_Parent(parent), m_CurrentGame{nameOne, nameTwo} {

    m_CurrentGame = Antrenament(nameOne, nameTwo);
    DrawGame();
}

QPixmap LoadCardImage(const int cardValue, const PlayerTurn &turn) {
    int turnNumber = 1;
    if (turn != PlayerTurn::Player1) {
        turnNumber = 2;
    }
    const QString imagePath = QString("../images/%1_%2.jpg").arg(cardValue).arg(turnNumber);
    QPixmap       pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image for card" << cardValue;
        return {};
    }
    return pixmap;
}

QGridLayout *
AntrenamentWidget::GenerateBoard(const Board                         &board,
                                 const std::function<void(Position)> &cellClickedCallback) {
    const auto boardLayout = new QGridLayout();

    auto [leftX, leftY]   = board.GetLeft();
    auto [rightX, rightY] = board.GetRight();
    auto [upX, upY]       = board.GetUp();
    auto [downX, downY]   = board.GetDown();

    if (!board.IsBoardLocked()) {
        ++rightX;
        ++downY;
        --leftX;
        --upY;
    }

    const auto &boardElement = board.GetGameBoard();

    for (int j = upY; j <= downY; ++j) {
        for (int i = leftX; i <= rightX; ++i) {
            const auto button = new QPushButton();
            button->setFixedSize(150, 150);

            const auto placedCard = boardElement.find({i, j});

            if (placedCard == boardElement.end()) {
                button->setText("Empty");
            } else {
                button->setText(QString::number(placedCard->second.top().GetValue()));
            }

            connect(button, &QPushButton::clicked,
                    [i, j, cellClickedCallback] { cellClickedCallback({i, j}); });

            button->setStyleSheet("border: 3px solid black;");
            boardLayout->addWidget(button, j - upY, i - leftX);
        }
    }

    boardLayout->setRowStretch(downY - upY + 1, 1);
    boardLayout->setColumnStretch(rightX - leftX + 1, 1);

    return boardLayout;
}

QHBoxLayout *AntrenamentWidget::GenerateHand(const Hand                      &hand,
                                             const std::function<void(Card)> &cellClickedCallback) {
    const auto cardsLayout    = new QHBoxLayout();
    auto       selectedButton = std::make_shared<QPushButton *>(nullptr);

    for (const auto &card : hand) {
        const auto button = new QPushButton();
        button->setFixedSize(150, 150);

        QPixmap cardImage = LoadCardImage(card.GetValue(), card.GetPlacedBy());
        if (!cardImage.isNull()) {
            button->setIcon(QIcon(cardImage));
            button->setIconSize(button->size());
        } else {
            button->setText(QString::number(card.GetValue()));
        }

        button->setStyleSheet("border: 3px solid black;");
        connect(button, &QPushButton::clicked,
                [this, selectedButton, button, card, cellClickedCallback] {
                    if (*selectedButton) {
                        (*selectedButton)->setStyleSheet("border: 3px solid black;");
                    }
                    *selectedButton = button;

                    cellClickedCallback(card);
                });

        cardsLayout->addWidget(button);
    }

    return cardsLayout;
}

void AntrenamentWidget::DrawGame() {
    m_SelectedCard.reset();
    m_SelectedPosition.reset();

    const auto boardLayout = GenerateBoard(m_CurrentGame.GetBoard(), [&](const Position &pos) {
        if (!m_SelectedCard) {
            std::cout << "No card selected!\n";
            return;
        }

        std::cout << "[a] Selected card: " << m_SelectedCard->GetValue() << '\n';

        std::cout << "[a] Selected position: " << pos.first << ' ' << pos.second << '\n';

        m_SelectedPosition = std::make_unique<Position>(pos);

        if (PlaceCard()) {
            DrawGame();
        }
    });

    const auto hand = m_CurrentGame.GetCurrentPlayer() == PlayerTurn::Player1
                              ? m_CurrentGame.GetPlayer1().GetHand()
                              : m_CurrentGame.GetPlayer2().GetHand();

    const auto handElement = GenerateHand(hand, [&](const Card &card) {
        std::cout << "Selected card: " << card.GetValue() << '\n';

        m_SelectedCard = std::make_unique<Card>(card);
    });
    // add everything to the layout

    m_Layout->addLayout(boardLayout);
    m_Layout->addLayout(handElement);

    // draw the layout
    m_AntrenamentWidget->setLayout(m_Layout);
    m_StackedWidget->addWidget(m_AntrenamentWidget);
    m_StackedWidget->setCurrentWidget(m_AntrenamentWidget);
}

bool AntrenamentWidget::PlaceCard() {
    if (!m_SelectedCard || !m_SelectedPosition) {
        // std::cout << std::format("Selected card: {}, selected position: {}\n", m_SelectedCard,
        //                          m_SelectedPosition);
        return false;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(*m_SelectedCard, *m_SelectedPosition,
                                                             m_CurrentGame.GetCurrentPlayer());

    m_SelectedCard.reset();
    m_SelectedPosition.reset();

    if (success) {
        std::cout << "Card inserted\n";
        return true;
    }

    std::cout << "Card could not be inserted\n";
    return false;
}
