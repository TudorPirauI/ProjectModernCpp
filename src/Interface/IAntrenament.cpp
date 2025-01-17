//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"

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

    m_HandWidget = new HandWidget(this);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_HandWidget->setFixedSize(m_HandWidget->GetIdealWidth(), 200);
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

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
        return;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(
            m_SelectedCard.value(), {x, y}, m_CurrentPlayer, CardType::Normal, m_CurrentGame);

    if (success != InsertOutputs::Success) {
        std::cout << "Could not place card on board\n";
        return;
    }

    auto &currentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                   : m_CurrentGame.GetPlayer2();

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IAntrenament::SwitchTurn() {
    // TODO: Check for game end
    // TODO: Use the AlertWidget here to handle the above
    // TODO: Handle the best out of 3 games
    // TODO: Pretty up the hand and the board
    // The board should have consistent spacings between the slots
    // The cards should be held like a bridge hand, slightly circular
    // When you select a card, it should be raised slightly
    // When you hover over the cards in your hands they should increase in size a bit and move
    // according to your cursor

    m_SelectedCard.reset();

    m_CurrentPlayer =
            (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();

    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
    m_HandWidget->SetCards(nextPlayer.GetHand());

    m_HandWidget->update();
    m_BoardWidget->update();
}
