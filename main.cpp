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

#include "console/ConsoleView.h"

int main(int argc, char* const argv[])
{
  ConsoleView cV(argc, argv);
  if (!cV.isThereAnError()) {
    cV.launch();
  }

  return 0;
}
