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


StdFileWriter::StdFileWriter(std::ostream& stream)
  : m_stream(stream)
{

}

StdFileWriter::~StdFileWriter()
{

}

void StdFileWriter::visitResult(Result* result) {
  // EHSS et PA
  if (result->isEHSSPrintable()) {
      m_stream << "\t|  " << result->getEHSS();
  }
  if (result->isPAPrintable()) {
      m_stream << "\t|  " << result->getPA();
  }

  // TM
  if (result->isTMPrintable()) {
      m_stream << "\t|  " << result->getTM();
      m_stream << "\t|\t  " << result->getStructAsymParam() << "\t";
      m_stream << "\t|\t" << result->getStandardDeviation() << "\t";
      m_stream << "\t|\t    " << result->getNumberOfFailedTrajectories() << "\t";
  }

  m_stream << "\t|";
}

void StdFileWriter::visitMean(Mean* mean) {
  // EHSS et PA
  if (mean->isEHSSPrintable()) {
      m_stream << "\t|  " << mean->getMeanEHSS();
  }
  if (mean->isPAPrintable()) {
      m_stream << "\t|  " << mean->getMeanPA();
  }

  // TM
  if (mean->isTMPrintable()) {
      m_stream << "\t|  " << mean->getMeanTM();
      m_stream << "\t|\t  " << mean->getMeanStructAsymParam() << "\t";
      m_stream << "\t|\t" << mean->getMeanStandardDeviation() << "\t";
      m_stream << "\t|\t    " << mean->getMeanNumberOfFailedTrajectories() << "\t";
  }

  m_stream << "\t|";
}
