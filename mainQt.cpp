/*
 * Collision-Code
 * Free software to calculate collision cross-section with Helium.
 * Université de Rouen
 * 2016
 *
 * Anthony BREANT
 * Clement POINSOT
 * Jeremie PANTIN
 * Mohamed TAKHTOUKH
 * Thomas CAPET
 */

#include <QtWidgets>
#include "gui/CCFrame.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CCFrame frame;
    frame.show();

    return app.exec();
}
