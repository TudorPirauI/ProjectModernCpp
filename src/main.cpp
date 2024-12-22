#include <QLoggingCategory>

#include <Interface/MainWindow.h>

#include "GameModes/Antrenament.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.utils=false");

    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();

    // Antrenament antrenament("Player 1", "Player 2");
    //
    // auto       cardOne = antrenament.GetPlayer1().GetHand()[0];
    // const auto success = antrenament.GetBoard().InsertCard(cardOne, {0, 0}, PlayerTurn::Player1);
    //
    // std::cout << "Card inserted: " << success << '\n';
    //
    // return 0;
}
