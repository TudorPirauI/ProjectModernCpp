#include <QLoggingCategory>

#include <Interface/MainWindow.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.utils=false");

    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}
