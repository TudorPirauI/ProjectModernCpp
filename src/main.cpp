#include "Interface/IAntrenament.h"
#include "Interface/MainWindow.h"

int main(int argc, char *argv[]) {
    [[maybe_unused]] QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}

/*
 * facut: load reload json
 * todo: duelulElementelor (-+ same with duelulVrajitorilor), turneu, rapid
 * todo: duelulElementelor + duelulVrajitorilor (is a separated class with 1 wizard power and 1
 * todo: elemental power) -+ same with duelulVrajitorilor
 */

// todo: verify wizard power and elemental power:)
