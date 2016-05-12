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

#include "StdFileWriter.h"

#include <iostream>

StdFileWriter::StdFileWriter(std::ofstream& file)
  : m_file(file)
{

}

StdFileWriter::~StdFileWriter()
{

}

void StdFileWriter::visitResult(Result* result) {
  // EHSS et PA
  if (result->isEHSSPrintable()) {
      m_file << "EHS cross section = " << result->getEHSS() << std::endl;
      if (!result->isPAPrintable()) {
        // Pas de PA ensuite, on saute une ligne.
        m_file << std::endl;
      }
  }
  if (result->isPAPrintable()) {
      m_file << "PA cross section = " << result->getPA() << std::endl;
      // On saute une ligne pour la TM.
      m_file << std::endl;
  }

  // TM
  if (result->isTMPrintable()) {
      m_file << "TM cross section = " << result->getTM() << std::endl;
      m_file << "Structural asymmetry parameter = " << result->getStructAsymParam() << std::endl;
      m_file << "Standard deviation (in percent) = " << result->getStandardDeviation() << std::endl;
      m_file << "Number of failed trajectories = " << result->getNumberOfFailedTrajectories() << std::endl;
  }
}
