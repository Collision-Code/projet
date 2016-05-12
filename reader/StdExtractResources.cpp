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

#include "StdExtractResources.h"

#include "StdExtractFactory.h"

StdExtractResources::StdExtractResources()
{
  m_extractFactory = new StdExtractFactory();
}

StdExtractResources::~StdExtractResources()
{
  delete m_extractFactory;
}

std::vector<Molecule*>* StdExtractResources::getGeometriesFromFile(std::string fileName)
{
  FileReader* fR = m_extractFactory->getReader(fileName);
  if (fR == nullptr) {
    return nullptr;
  }

  std::vector<Molecule*>* vectorMol = fR->loadResources();
  delete fR;

  return vectorMol;
}
