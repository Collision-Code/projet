#include <QtWidgets>
#include "gui/CCFrame.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CCFrame frame;
    frame.show();
    return app.exec();
}
