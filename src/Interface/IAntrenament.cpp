//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"
#include <QVBoxLayout>

IAntrenament::IAntrenament(QWidget *parent)
        : QObject(parent),
          m_CurrentGame("Player1", "Player2"),
          m_CurrentPlayer(PlayerTurn::Player1),
          m_SelectedCard(1),
          m_ParentWidget(parent) {

    m_DisplayHand = new DisplayHand(parent);
    m_DisplayBoard = new DisplayBoard(parent, 3, 3);

    connect(m_DisplayHand, &DisplayHand::CardSelected, this, &IAntrenament::OnCardSelected);
    connect(m_DisplayBoard, &DisplayBoard::PlaceCard, this, &IAntrenament::OnCardPlaced);
}

void IAntrenament::StartGame() {
    m_CurrentGame.SetNewCards();
    DisplayHand();
    DisplayBoard();
}

void IAntrenament::OnCardSelected(const Card &card) {
    m_SelectedCard = card;
    m_DisplayBoard->PlaceCard(m_SelectedCard);
}

void IAntrenament::OnCardPlaced() {
    if (m_SelectedCard.GetValue() == -1) {
        return;
    }

    if (m_CurrentPlayer == PlayerTurn::Player1) {
        m_CurrentGame.GetPlayer1().RemoveCard(m_SelectedCard);
    } else {
        m_CurrentGame.GetPlayer2().RemoveCard(m_SelectedCard);
    }

    DisplayHand();
    DisplayBoard();

    if (m_CurrentGame.CheckWinningConditions()) {
        emit GameFinished(m_CurrentPlayer);
        return;
    }

    m_CurrentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    DisplayHand();
}