//
// Created by Tudor on 1/9/2025.
//

#include "Interface/IAntrenament.h"

Q_DECLARE_METATYPE(Position)
Q_DECLARE_METATYPE(Card)

IAntrenament::IAntrenament(const std::string &nameOne, const std::string &nameTwo,
                           QWidget *parent) :
    QObject(parent), m_CurrentGame(nameOne, nameTwo), m_CurrentPlayer(PlayerTurn::Player1),
    m_SelectedCard(std::nullopt), m_ParentWidget(parent) {

    qRegisterMetaType<Position>("Position");
    qRegisterMetaType<Card>("Card");

    m_BoardWidget = new BoardWidget(parent, 3);
    m_HandWidget  = new HandWidget(parent);

    const auto layout = new QVBoxLayout(parent);

    // const auto titleLabel = new QLabel("Antrenament", parent);
    // QFont      font;
    // font.setPointSize(24);
    // font.setBold(true);
    // titleLabel->setFont(font);
    // titleLabel->setAlignment(Qt::AlignCenter);
    //
    // layout->addWidget(titleLabel);
    layout->addWidget(m_BoardWidget);
    layout->addWidget(m_HandWidget);

    parent->setLayout(layout);

    connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);
    connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this, &IAntrenament::OnPositionSelected);

    bool handConnection =
            connect(m_HandWidget, &HandWidget::CardSelected, this, &IAntrenament::OnCardSelected);
    bool boardConnection = connect(m_BoardWidget, &BoardWidget::BoardSlotClicked, this,
                                   &IAntrenament::OnPositionSelected);

    std::cout << "HandWidget connection status: " << handConnection << std::endl;
    std::cout << "BoardWidget connection status: " << boardConnection << std::endl;

    StartGame();
}

void IAntrenament::StartGame() {
    m_BoardWidget->SetBoard(m_CurrentGame.GetBoard());

    m_HandWidget->SetCards(m_CurrentGame.GetPlayer1().GetHand());
    m_BoardWidget->OnDraw();

    qDebug() << "Game started!";
}

void IAntrenament::OnCardSelected(const int cardValue) {
    m_SelectedCard = Card(cardValue);
    std::cout << "In slot:\n";
    std::cout << "Card selected with value:" << m_SelectedCard.value().GetValue();
}

void IAntrenament::OnPositionSelected(int x, int y) {
    const Position pos{x, y};
    std::cout << "In slot:\n";
    std::cout << std::format("Got position: ({}, {})", pos.first, pos.second) << std::endl;
    std::cout << std::format("Selected card: {}", m_SelectedCard.value().GetValue()) << std::endl;

    if (m_SelectedCard.has_value())
        std::cout << std::format("Selected card: {}", m_SelectedCard.value().GetValue())
                  << std::endl;
    else
        std::cout << "No card selected!" << std::endl;
}

void IAntrenament::SwitchTurn() {
    // TODO: probably have a smart pointer of type Player in the game itself so we can avoid this
    // tomfoolery
    m_CurrentPlayer =
            (m_CurrentPlayer == PlayerTurn::Player1) ? PlayerTurn::Player2 : PlayerTurn::Player1;

    const auto &nextPlayer = (m_CurrentPlayer == PlayerTurn::Player1) ? m_CurrentGame.GetPlayer1()
                                                                      : m_CurrentGame.GetPlayer2();
    m_HandWidget->SetCards(nextPlayer.GetHand());

    qDebug() << "Switched to player:"
             << (m_CurrentPlayer == PlayerTurn::Player1 ? "Player1" : "Player2");
}
