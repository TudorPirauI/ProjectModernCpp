#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <Interface/MainWindow.h>

#include "GameModes/Antrenament.h"
#include "Interface/DisplayHand.h"
#include "Interface/DisplayBoard.h"


int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);
//
//    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.utils=false");
//
//    MainWindow mainWindow;
//    mainWindow.show();
//    return QApplication::exec();

//Test afisare mana
//    QApplication app(argc, argv);
//
//    QMainWindow mainWindow;
//    DisplayHand *displayHand = new DisplayHand(&mainWindow);
//
//    std::vector<Card> cards = {Card(1), Card(2), Card(3), Card(4)};
//    displayHand->setCards(cards);
//
//    mainWindow.setCentralWidget(displayHand);
//    mainWindow.resize(600, 400);
//    mainWindow.show();
//
//    return app.exec();

//Test afisare tabla
//    QApplication app(argc, argv);
//
//    QMainWindow mainWindow;
//    DisplayBoard *displayBoard = new DisplayBoard(&mainWindow);
//
//    Board board(10);
//    board.InsertCard(Card(1), {0, 0}, PlayerTurn::Player1);
//    board.InsertCard(Card(2), {1, 0}, PlayerTurn::Player2);
//    board.InsertCard(Card(3), {0, 1}, PlayerTurn::Player1);
//    board.InsertCard(Card(4), {1, 1}, PlayerTurn::Player2);
//
//    displayBoard->setBoard(board);
//
//    mainWindow.setCentralWidget(displayBoard);
//    mainWindow.resize(600, 600);
//    mainWindow.show();
//
//    return app.exec();


    QApplication app(argc, argv);

    QMainWindow mainWindow;
    QWidget centralWidget;
    QVBoxLayout mainLayout(&centralWidget);

    QWidget *boardContainer = new QWidget(&centralWidget);
    QHBoxLayout *boardLayout = new QHBoxLayout(boardContainer);
    boardLayout->addStretch();
    DisplayBoard *displayBoard = new DisplayBoard(boardContainer);
    boardLayout->addWidget(displayBoard);
    boardLayout->addStretch();

    DisplayHand *displayHand = new DisplayHand(&centralWidget);

    Board board(4); // Create an empty board with size 4
    displayBoard->setBoard(board);

    std::vector<Card> cards = {Card(1), Card(2), Card(3), Card(4)};
    displayHand->setCards(cards);

    mainLayout.addWidget(boardContainer);
    mainLayout.addWidget(displayHand);

    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.resize(1200, 1200);
    mainWindow.show();

    // Example of adding cards to the board after it is displayed
    displayBoard->insertCard(Card(1), {0, 0}, PlayerTurn::Player1);
    displayBoard->insertCard(Card(2), {1, 0}, PlayerTurn::Player2);

    return app.exec();
}
