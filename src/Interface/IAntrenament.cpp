//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"
// Q_DECLARE_METATYPE(Position)
// Q_DECLARE_METATYPE(Card)

IAntrenament::IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                           QWidget *parent) :
    QWidget(parent), m_CurrentGame(nameOne, nameTwo), m_CurrentPlayer(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    const auto mainLayout = new QVBoxLayout(this);

    m_BoardWidget = new BoardWidget(this, 3);
    m_BoardWidget->setFixedSize(600, 600);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this, &IAntrenament::OnPositionSelected);
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    auto *boardLayout = new QHBoxLayout();
    boardLayout->addStretch();
    boardLayout->addWidget(m_BoardWidget);
    boardLayout->addStretch();

    mainLayout->addLayout(boardLayout);

    // Center the hand widget
    m_HandWidget = new HandWidget(this);
    m_HandWidget->setFixedSize(600, 200); // Set a fixed size for the hand widget
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());

    auto *handLayout = new QHBoxLayout();
    handLayout->addStretch();
    handLayout->addWidget(m_HandWidget);
    handLayout->addStretch();

    mainLayout->addLayout(handLayout);

    parent->setLayout(mainLayout);
}

void IAntrenament::OnCardSelected(const int cardValue) {
    m_SelectedCard = Card(cardValue);
    qDebug() << "[Slot] Card selected with value:" << m_SelectedCard.value().GetValue() << '\n';
}

void IAntrenament::OnPositionSelected(const int x, const int y) {
    if (!m_SelectedCard.has_value()) {
        std::cout << "Please select a card first!\n";
        return;
    }

    if (!m_CurrentGame.GetBoard().IsPositionValid({x, y}, m_SelectedCard.value())) {
        std::cout << "You cannot place a card there!";
        // TODO: make the invalid positions unclickable in BoardWidget
        return;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(m_SelectedCard.value(), {x, y},
                                                             m_CurrentPlayer, CardType::Normal);

    if (!success) {
        std::cout << "Could not place card on board\n";
        return;
    }

    // remove the player's card from their hand

    auto &currentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                   : m_CurrentGame.GetPlayer2();

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IAntrenament::SwitchTurn() {
    m_SelectedCard.reset();

    m_CurrentPlayer =
            (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();

    m_HandWidget->SetCards(nextPlayer.GetHand());
    m_HandWidget->show(); // Ensure the hand widget is visible
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    m_HandWidget->update();
    m_BoardWidget->update();

    qDebug() << "Switched to player:"
             << (m_CurrentPlayer == PlayerTurn::Player1 ? "Player1" : "Player2");
}
