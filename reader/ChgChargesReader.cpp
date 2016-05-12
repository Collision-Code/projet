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

#include "ChgChargesReader.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "../lib/boost/tokenizer.hpp"
#include "../molecule/Molecule.h"
#include "../molecule/StdMolecule.h"
#include "../molecule/Atom.h"
#include "../molecule/StdAtom.h"

// BUILDER
ChgChargesReader::ChgChargesReader(std::string filename) {
  setFileName(filename);
}

// DESTRUCTOR
ChgChargesReader::~ChgChargesReader() {}

// COMMANDS
void ChgChargesReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("ChgFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".chg") != 0) {
    throw std::string("ChgFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* ChgChargesReader::loadResources(std::vector<Molecule*>* molGeometries) {
  std::ifstream file(m_filename.c_str(), std::ios::in);

  std::vector<std::vector<double>> chargesVector;

  if (file) {
    std::string line;
    int geometriesNb = molGeometries->size();
    int atomsNb = (*molGeometries)[0]->getAtomNumber();

    for (int i = 0; i < geometriesNb; i++) {
      std::vector<double> newCharges;

      for (int j = 0; j < atomsNb; j++) {
        std::getline(file, line);
        double charge = strtod(line.c_str(), nullptr);
        newCharges.push_back(charge);
      }
      if (atomsNb != newCharges.size()) {
        std::ostringstream oss;
        oss << "Invalid vector of charge in " << m_filename << ".";
        throw oss.str();
      }
      chargesVector.push_back(newCharges);

      if (i < geometriesNb - 1) {
        std::getline(file, line); // Eat blank line
      }
    }

    for (int i = 0; i < geometriesNb; i++) {
        Molecule* mol = (*molGeometries)[i];
        std::vector<Atom*> atoms = *(mol->getAllAtoms());
        std::vector<double> charges = chargesVector[i];
        for (int j = 0; j < atomsNb; j++) {
            atoms[j]->setCharge(charges[j]);
        }
    }

  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  file.close();

  return molGeometries;
}
