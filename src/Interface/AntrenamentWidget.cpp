#include <Interface/AntrenamentWidget.h>

#include <QPushButton>
#include <iostream>
AntrenamentWidget::AntrenamentWidget(const std::string &nameOne, const std::string &nameTwo,
                                     QStackedWidget *stackedWidget, QWidget *parent) :
    m_AntrenamentWidget(new QWidget(parent)), m_Layout(new QVBoxLayout(m_AntrenamentWidget)),
    m_StackedWidget(stackedWidget), m_Parent(parent),
    m_CurrentGame(std::make_unique<Antrenament>(nameOne, nameTwo)) {
    m_ScoreLabel = new QLabel(m_AntrenamentWidget);
    DrawGame();
}

QPixmap LoadCardImageTwoLmao(const int cardValue, const PlayerTurn &turn) {
    int turnNumber = 1;
    if (turn != PlayerTurn::Player1) {
        turnNumber = 2;
    }
    const QString imagePath = QString("../images/%1_%2.jpg").arg(cardValue).arg(turnNumber);
    const QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image for card" << cardValue;
    }
    return pixmap;
}

QGridLayout *
AntrenamentWidget::GenerateBoard(const Board                         &board,
                                 const std::function<void(Position)> &cellClickedCallback) {
    const auto boardLayout = new QGridLayout();
    // boardLayout->setSpacing(0);
    // boardLayout->setContentsMargins(0, 0, 0, 0);

    auto left  = board.GetLeft();
    auto right = board.GetRight();
    auto up    = board.GetUp();
    auto down  = board.GetDown();

    if (!board.IsBoardLocked()) {
        ++right.first;
        ++down.second;
        --left.first;
        --up.second;
    }

    const auto boardElement = board.GetGameBoard();

    for (int j = up.second; j <= down.second; ++j) {
        for (int i = left.first; i <= right.first; ++i) {
            const auto it = boardElement.find({i, j});
            QString    cellContent;
            QString    cellStyle = "border: 1px solid black;";

            QPixmap cardImage;

            if (it == boardElement.end()) {
                if (board.IsPositionValid({i, j}, Card(2, PlayerTurn::Player1))) {
                    // cellContent = " V ";
                    cellStyle += "background-color: green;";
                } else {
                    // cellContent = " X ";
                    cellStyle += "background-color: red;";
                }
            } else {
                const auto card = it->second.top();
                cardImage       = LoadCardImageTwoLmao(card.GetValue(), card.GetPlacedBy());
                if (card.GetIsFlipped()) {
                    cellContent = " H ";
                    cellStyle += "background-color: #FFFF99;";
                } else if (card.GetIsEter()) {
                    cellContent = QString(" %1 ").arg(card.GetValue());
                    cellStyle += "background-color: #00FFFF;";
                } else {
                    cellContent = QString(" %1 ").arg(card.GetValue());
                    // if (card.GetPlacedBy() == PlayerTurn::Player1) {
                    //     // cellStyle += QString("background-color:
                    //     %1;").arg(m_Player1Color.name());
                    // } else {
                    //     // cellStyle += QString("background-color:
                    //     %1;").arg(m_Player2Color.name());
                    // }
                }

                // if (card.GetIsEter()) {
                //     if (card.GetPlacedBy() == PlayerTurn::Player1) {
                //         // cellStyle += QString("border: 2px solid
                //         %1;").arg(m_Player1Color.name());
                //     } else {
                //         // cellStyle += QString("border: 2px solid
                //         %1;").arg(m_Player2Color.name());
                //     }
                // }
            }

            const auto cellButton = new QPushButton(this);
            cellButton->setFixedSize(150, 150);

            if (!cardImage.isNull()) {
                cellButton->setIcon(QIcon(cardImage));
                cellButton->setIconSize(cellButton->size());
            } else {
                cellButton->setStyleSheet(cellStyle);
                cellButton->setText(cellContent);
            }

            connect(cellButton, &QPushButton::clicked,
                    [cellClickedCallback, i, j] { cellClickedCallback({i, j}); });
            boardLayout->addWidget(cellButton, j - up.second, i - left.first);
        }
    }

    boardLayout->setRowStretch(down.second - up.second + 1, 1);
    boardLayout->setColumnStretch(right.first - left.first + 1, 1);

    return boardLayout;
}

QHBoxLayout *AntrenamentWidget::GenerateHand(const Hand                      &hand,
                                             const std::function<void(Card)> &cellClickedCallback) {
    const auto cardsLayout    = new QHBoxLayout();
    auto       selectedButton = std::make_shared<QPushButton *>(nullptr);

    for (const auto &card : hand) {
        const auto button = new QPushButton();
        button->setFixedSize(150, 150);

        QPixmap cardImage = LoadCardImageTwoLmao(card.GetValue(), card.GetPlacedBy());
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
    m_AntrenamentWidget = new QWidget(this);
    m_Layout            = new QVBoxLayout(m_AntrenamentWidget);

    std::cout << "here\n";

    const auto  currentTurn   = m_CurrentGame->GetCurrentPlayer();
    const auto  currentPlayer = currentTurn == PlayerTurn::Player1 ? m_CurrentGame->GetPlayer1()
                                                                   : m_CurrentGame->GetPlayer2();
    const auto &currentHand   = currentPlayer.GetHand();

    const auto turnLabel = new QLabel(this);
    turnLabel->setText(
            QString::fromStdString(std::format("Current turn: {}", currentPlayer.GetUserName())));
    turnLabel->setAlignment(Qt::AlignCenter);
    QFont turnLabelFont = turnLabel->font();
    turnLabelFont.setPointSize(12);
    turnLabel->setFont(turnLabelFont);
    turnLabel->setFixedHeight(turnLabel->sizeHint().height());
    m_Layout->addWidget(turnLabel);

    // Add the score label to the layout
    m_Layout->addWidget(m_ScoreLabel);

    auto selectedCard = std::make_shared<std::optional<Card>>(std::nullopt);

    const auto boardLayout =
            GenerateBoard(m_CurrentGame->GetBoard(), [this, selectedCard, currentTurn,
                                                      currentPlayer](const auto &pos) {
                if (!selectedCard->has_value()) {
                    std::cout << "No card selected!\n";
                    return;
                }

                std::cout << selectedCard->value().GetValue();

                std::cout << "here1";

                std::cout << "here2";
                auto properCard = selectedCard->value();
                std::cout << "here3";
                if (m_CurrentGame->GetBoard().InsertCard(properCard, pos, currentTurn)) {
                    std::cout << "here4";
                    if (m_CurrentGame->CheckWinningConditions()) {
                        std::cout << "here5";
                        ShowWinningMessage(QString::fromStdString(currentPlayer.GetUserName()));
                        return;
                    }

                    std::cout << "here6";

                    m_CurrentGame->SetNextPlayerTurn(currentTurn == PlayerTurn::Player1
                                                             ? PlayerTurn::Player2
                                                             : PlayerTurn::Player1);

                    std::cout << "here7";
                    DrawGame();
                    // m_StackedWidget->repaint();
                } else {
                    std::cout << std::format("Card {} could not be inserted at ({}, {})\n",
                                             properCard.GetValue(), pos.first, pos.second);
                }
            });

    const auto centeredBoardLayout = new QHBoxLayout();
    centeredBoardLayout->addStretch();
    centeredBoardLayout->addLayout(boardLayout);
    centeredBoardLayout->addStretch();

    m_Layout->addLayout(centeredBoardLayout);

    const auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    m_Layout->addWidget(separator);

    const auto handLayout = GenerateHand(currentHand, [this, selectedCard](const Card &card) {
        *selectedCard = card;
        std::cout << "Selected card: " << selectedCard->value().GetValue() << '\n';
    });

    m_Layout->addLayout(handLayout);

    m_StackedWidget->addWidget(m_AntrenamentWidget);
    m_StackedWidget->setCurrentWidget(m_AntrenamentWidget);
}

void AntrenamentWidget::ShowWinningMessage(const QString &winnerName) {
    const auto winningWidget = new QWidget(this);
    const auto layout        = new QVBoxLayout(winningWidget);

    const auto label     = new QLabel(QString("%1 has won the game!").arg(winnerName), this);
    QFont      labelFont = label->font();
    labelFont.setPointSize(24);
    labelFont.setBold(true);
    label->setFont(labelFont);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    const auto okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, [this, winningWidget, winnerName] {
        m_StackedWidget->removeWidget(winningWidget);
        if (winnerName == QString::fromStdString(m_CurrentGame->GetPlayer1().GetUserName())) {
            ++m_Player1Score;
        } else {
            ++m_Player2Score;
        }
        UpdateScoreLabel(m_CurrentGame->GetPlayer1().GetUserName(),
                         m_CurrentGame->GetPlayer2().GetUserName());
        if (++m_GamesPlayed < 3) {
            m_CurrentGame = nullptr;
            m_CurrentGame =
                    std::make_unique<Antrenament>(m_CurrentGame->GetPlayer1().GetUserName(),
                                                  m_CurrentGame->GetPlayer2().GetUserName());
            DrawGame();
        } else {
            const auto finalScoreWidget = new QWidget(this);
            const auto finalLayout      = new QVBoxLayout(finalScoreWidget);

            const auto finalScoreLabel = new QLabel(
                    QString("Final Score:\n%1: %2\n%3: %4")
                            .arg(QString::fromStdString(m_CurrentGame->GetPlayer1().GetUserName()))
                            .arg(m_Player1Score)
                            .arg(QString::fromStdString(m_CurrentGame->GetPlayer2().GetUserName()))
                            .arg(m_Player2Score),
                    this);
            QFont finalScoreFont = finalScoreLabel->font();
            finalScoreFont.setPointSize(24);
            finalScoreFont.setBold(true);
            finalScoreLabel->setFont(finalScoreFont);
            finalScoreLabel->setAlignment(Qt::AlignCenter);
            finalLayout->addWidget(finalScoreLabel);

            const auto finalOkButton = new QPushButton("OK", this);
            finalLayout->addWidget(finalOkButton);
            connect(finalOkButton, &QPushButton::clicked, this, [this, finalScoreWidget] {
                m_StackedWidget->removeWidget(finalScoreWidget);
                // Reset the game state
                m_GamesPlayed  = 0;
                m_Player1Score = 0;
                m_Player2Score = 0;
                m_CurrentGame  = nullptr;
                DrawGame();
            });

            finalLayout->setAlignment(Qt::AlignCenter);

            m_StackedWidget->addWidget(finalScoreWidget);
            m_StackedWidget->setCurrentWidget(finalScoreWidget);
        }
    });

    layout->setAlignment(Qt::AlignCenter);

    m_StackedWidget->addWidget(winningWidget);
    m_StackedWidget->setCurrentWidget(winningWidget);
}

void AntrenamentWidget::UpdateScoreLabel(const std::string &nameOne,
                                         const std::string &nameTwo) const {
    m_ScoreLabel->setText(QString("%1: %2 - %3: %4")
                                  .arg(QString::fromStdString(nameOne))
                                  .arg(m_Player1Score)
                                  .arg(QString::fromStdString(nameTwo))
                                  .arg(m_Player2Score));
    m_ScoreLabel->show();
}
