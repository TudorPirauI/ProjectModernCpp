//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"
#include <QLabel>
#include <QVBoxLayout>
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"

IAntrenament::IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                           QWidget *parent) :
    QObject(parent), m_CurrentGame(nameOne, nameTwo), m_CurrentPlayer(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    m_BoardWidget = new BoardWidget(parent, 4, 4);
    m_HandWidget  = new HandWidget(parent);

    auto layout = new QVBoxLayout(parent);

    auto  titleLabel = new QLabel("Antrenament", parent);
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(m_BoardWidget);
    layout->addWidget(m_HandWidget);

    parent->setLayout(layout);

    //    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);
    //    connect(m_BoardWidget, &BoardWidget::PlaceCard, this,
    //            [this](const Card &, const std::pair<int, int> &position) {
    //                OnCardPlaced(position.first, position.second);
    //            });

    StartGame();
}

void IAntrenament::StartGame() {
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_BoardWidget->OnDraw();

    qDebug() << "Game started!";
}

void IAntrenament::OnCardSelected(const Card &card) {
    m_SelectedCard = card;

    m_BoardWidget->PlaceCard(card);

    qDebug() << "Card selected with value:" << card.GetValue();
}

void IAntrenament::OnCardPlaced(int row, int col) {
    if (!m_SelectedCard.has_value()) {
        qDebug() << "No card selected!";
        return;
    }

    const auto cardValue = m_SelectedCard.value().GetValue();
    const Card card(cardValue);

    //    if (m_CurrentGame.GetBoard().IsPositionEmpty(row, col)) {
    //        m_CurrentGame.GetBoard().InsertCard(card, {row, col}, m_CurrentPlayer);
    //        m_BoardWidget->InsertCard(card, {row, col}, m_CurrentPlayer);
    //
    //        auto &currentPlayer = m_CurrentPlayer == PlayerTurn::Player1 ?
    //        m_CurrentGame.GetPlayer1()
    //                                                                     :
    //                                                                     m_CurrentGame.GetPlayer2();
    //        currentPlayer.RemoveCard(cardValue);
    //
    //        m_SelectedCard.reset();
    //
    //        SwitchTurn();
    //    } else {
    //        qDebug() << "Position not empty! Try another.";
    //    }
}

void IAntrenament::SwitchTurn() {
    m_CurrentPlayer =
            (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();
    m_HandWidget->SetCards(nextPlayer.GetHand());

    qDebug() << "Switched to player:"
             << (m_CurrentPlayer == PlayerTurn::Player1 ? "Player1" : "Player2");
}
