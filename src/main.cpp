#include "Interface/IAntrenament.h"
#include "Interface/MainWindow.h"

int main(int argc, char *argv[]) {
    [[maybe_unused]] QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}

/*
 *todo: reload to json file
 */
