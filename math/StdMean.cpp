#include "StdMean.h"

StdMean::StdMean()
{

}

StdMean::~StdMean()
{

}

double StdMean::getMeanEHSS() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getEHSS();
  }
  return m / m_listResults.size();
}

double StdMean::getMeanPA() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getPA();
  }
  return m / m_listResults.size();
}

double StdMean::getMeanTM() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getTM();
  }
  return m / m_listResults.size();
}

double StdMean::getMeanStructAsymParam() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getStructAsymParam();
  }
  return m / m_listResults.size();
}

double StdMean::getMeanStandardDeviation() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getStandardDeviation();
  }
  return m / m_listResults.size();
}

int StdMean::getMeanNumberOfFailedTrajectories() {
  double m = 0.0;
  for (unsigned int i = 0; i < m_listResults.size(); ++i) {
    m += m_listResults[i]->getNumberOfFailedTrajectories();
  }
  return m / m_listResults.size();
}

bool StdMean::isEHSSSaved() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isEHSSSaved();
}

bool StdMean::isPASaved() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isPASaved();
}

bool StdMean::isTMSaved() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isTMSaved();
}

bool StdMean::isEHSSPrintable() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isEHSSPrintable();
}

bool StdMean::isPAPrintable() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isPAPrintable();
}

bool StdMean::isTMPrintable() {
  if (m_listResults.size() == 0) {
    return false;
  }
  return m_listResults[0]->isTMPrintable();
}

void StdMean::accept(class FileWriter& fileWriter) {
  fileWriter.visitMean(this);
}
