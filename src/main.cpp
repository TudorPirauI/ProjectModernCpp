#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

#include <Interface/MainWindow.h>

#include "GameModes/Antrenament.h"
#include "Interface/DisplayBoard.h"
#include "Interface/DisplayHand.h"

int main(int argc, char *argv[]) {
    //    QApplication app(argc, argv);
    //
    //    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.utils=false");
    //
    //    MainWindow mainWindow;
    //    mainWindow.show();
    //    return QApplication::exec();

    // Test afisare mana
    //     QApplication app(argc, argv);
    //
    //     QMainWindow mainWindow;
    //     DisplayHand *displayHand = new DisplayHand(&mainWindow);
    //
    //     std::vector<Card> cards = {Card(1), Card(2), Card(3), Card(4)};
    //     displayHand->setCards(cards);
    //
    //     mainWindow.setCentralWidget(displayHand);
    //     mainWindow.resize(600, 400);
    //     mainWindow.show();
    //
    //     return app.exec();

    // Test afisare tabla
    //     QApplication app(argc, argv);
    //
    //     QMainWindow mainWindow;
    //     DisplayBoard *displayBoard = new DisplayBoard(&mainWindow);
    //
    //     Board board(10);
    //     board.InsertCard(Card(1), {0, 0}, PlayerTurn::Player1);
    //     board.InsertCard(Card(2), {1, 0}, PlayerTurn::Player2);
    //     board.InsertCard(Card(3), {0, 1}, PlayerTurn::Player1);
    //     board.InsertCard(Card(4), {1, 1}, PlayerTurn::Player2);
    //
    //     displayBoard->setBoard(board);
    //
    //     mainWindow.setCentralWidget(displayBoard);
    //     mainWindow.resize(600, 600);
    //     mainWindow.show();
    //
    //     return app.exec();

    QApplication app(argc, argv);

    QMainWindow mainWindow;
    QWidget     centralWidget;
    QVBoxLayout layout(&centralWidget);

    constexpr int lines   = 4;
    constexpr int columns = 4;

    const auto displayBoard = new DisplayBoard(&mainWindow, lines, columns);
    const auto displayHand  = new DisplayHand(&mainWindow);

    Board board(10);
    board.InsertCard(Card(1), {0, 0}, PlayerTurn::Player1);

    displayBoard->SetBoard(board);

    const std::vector cards = {Card(1), Card(2), Card(3), Card(4)};
    displayHand->SetCards(cards);

    layout.addWidget(displayBoard);
    layout.addWidget(displayHand);

    QObject::connect(displayHand, &DisplayHand::CardSelected, displayBoard,
                     &DisplayBoard::PlaceCard);

    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.resize(800, 800);
    mainWindow.show();

    return QApplication::exec();
}
