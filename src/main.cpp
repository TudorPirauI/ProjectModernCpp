#include <QLoggingCategory>

#include <Interface/MainWindow.h>

#include "GameModes/Antrenament.h"
#include "Interface/DisplayHand.h"


int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);
//
//    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.utils=false");
//
//    MainWindow mainWindow;
//    mainWindow.show();
//    return QApplication::exec();

    QApplication app(argc, argv);

    QMainWindow mainWindow;
    DisplayHand *displayHand = new DisplayHand(&mainWindow);

    std::vector<Card> cards = {Card(1), Card(2), Card(3), Card(4)};
    displayHand->setCards(cards);

    mainWindow.setCentralWidget(displayHand);
    mainWindow.resize(600, 400);
    mainWindow.show();

    return app.exec();
}
