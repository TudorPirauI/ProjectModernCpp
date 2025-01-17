//
// Created by Tudor on 1/17/2025.
//

#include "Interface/IDuelulElementelor.h"

IDuelulElementelor::IDuelulElementelor(const std::string &nameOne, const std::string &nameTwo,
                                       const std::array<bool, 3> &options, QWidget *parent) :
    QWidget(parent), m_CurrentGame(nameOne, nameTwo, options), m_CurrentPlayer(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    const auto mainLayout = new QVBoxLayout(this);

    m_BoardWidget = new BoardWidget(this, 3);
    m_BoardWidget->setFixedSize(600, 600);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this,
            &IDuelulElementelor::OnPositionSelected);
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    auto *boardLayout = new QHBoxLayout();
    boardLayout->addStretch();
    boardLayout->addWidget(m_BoardWidget);
    boardLayout->addStretch();

    mainLayout->addLayout(boardLayout);

    m_HandWidget = new HandWidget(this);
    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_HandWidget->setFixedSize(m_HandWidget->GetIdealWidth(), 200);
    connect(m_HandWidget, &HandWidget::CardSelected, this, &IDuelulElementelor::OnCardSelected);

    auto *handLayout = new QHBoxLayout();
    handLayout->addWidget(m_HandWidget);

    mainLayout->addLayout(handLayout);

    parent->setLayout(mainLayout);
}

void IDuelulElementelor::OnCardSelected(const int cardValue) {
    m_SelectedCard = Card(cardValue);
    qDebug() << "[Slot] Card selected with value:" << m_SelectedCard.value().GetValue() << '\n';
}

void IDuelulElementelor::OnPositionSelected(const int x, const int y) {
    if (!m_SelectedCard.has_value()) {
        std::cout << "Please select a card first!\n";
        return;
    }

    if (!m_CurrentGame.GetBoard().IsPositionValid({x, y}, m_SelectedCard.value())) {
        std::cout << "You cannot place a card there!";
        // TODO: make the invalid positions unclickable in BoardWidget
        return;
    }

    const auto success = m_CurrentGame.GetBoard().InsertCard(
            m_SelectedCard.value(), {x, y}, m_CurrentPlayer, CardType::Normal, m_CurrentGame);

    if (success != InsertOutputs::Success) {
        std::cout << "Could not place card on board\n";
        return;
    }

    // remove the player's card from their hand

    auto &currentPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                   : m_CurrentGame.GetPlayer2();

    currentPlayer.RemoveCard(m_SelectedCard.value());

    SwitchTurn();
}

void IDuelulElementelor::SwitchTurn() {
    m_SelectedCard.reset();

    m_CurrentPlayer =
            (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();

    // m_HandWidget->SetCards(nextPlayer.GetHand());
    // m_HandWidget->show(); // Ensure the hand widget is visible
    // m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
    //
    // m_HandWidget->update();
    // m_BoardWidget->update();

    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());
    m_HandWidget->SetCards(nextPlayer.GetHand());
    m_HandWidget->setVisible(true); // Ensure the hand widget is visible

    m_HandWidget->update();
    m_BoardWidget->update();

    qDebug() << "HandWidget visibility:" << m_HandWidget->isVisible();
    qDebug() << "HandWidget card count:" << nextPlayer.GetHand().size();
    qDebug() << "width:" << m_HandWidget->width();

    qDebug() << "Switched to player:"
             << (m_CurrentPlayer == PlayerTurn::Player1 ? "Player1" : "Player2");
}
