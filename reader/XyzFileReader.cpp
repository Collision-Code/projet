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

#include "XyzFileReader.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "../lib/boost/tokenizer.hpp"

#include "../molecule/StdMolecule.h"
#include "../molecule/StdAtom.h"

#include "../general/AtomInformations.h"

// BUILDER
XyzFileReader::XyzFileReader(std::string filename) {
  setFileName(filename);
}

// DESTRUCTOR
XyzFileReader::~XyzFileReader() {}

// COMMANDS
void XyzFileReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("XyzFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".xyz") != 0) {
    throw std::string("XyzFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* XyzFileReader::loadResources() {
  std::ifstream file(m_filename.c_str(), std::ios::in);

  std::string line;
  int atomNb = 0;

  std::vector<Molecule*>* moleculevector = new std::vector<Molecule*>();

  if (file) {
    std::string molName;
    std::string molNameTmp;

    while (std::getline(file, line)) {
      Molecule* newMol = new StdMolecule();

      if(line.empty()) break;

      boost::tokenizer<> tokenizeParam(line);
      boost::tokenizer<>::iterator firstParam = tokenizeParam.begin();
      atomNb = convertToDouble((*firstParam).c_str());
      std::getline(file, line); /* Eat comment line */

      for (int i = 0; i < atomNb; i++) {
        if (!(std::getline(file, line))) throw std::ostringstream("Invalid file.").str();

        if (line == "") {
          std::ostringstream oss;
          oss << "Invalid format of " << m_filename << ".";
          throw oss.str();
        }


        double x, y, z; /* Les valeurs recuperees sont en angstrom /!\ */
        std::string symbol;
        boost::char_separator<char> sep("	 ");
        boost::tokenizer<boost::char_separator<char>> tokenizeParam(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator param = tokenizeParam.begin();

        if (param == tokenizeParam.end()) throw std::string("Invalid file.");

        auto symb = AtomInformations::getInstance()->getAtomicNumber(*param);

        if (param == tokenizeParam.end()) {
          std::ostringstream oss;
          oss << "Invalid data in " << m_filename << ".";
          throw oss.str();
        }
        symbol = (*(param)).back();
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data in " << m_filename << ".";
            throw oss.str();
        }
        x = convertToDouble((*(param)).c_str());
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data in " << m_filename << ".";
            throw oss.str();
        }
        y = convertToDouble((*(param)).c_str());
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data in " << m_filename << ".";
            throw oss.str();
        }
        z = convertToDouble((*(param)).c_str());
        newMol->addAtom(new StdAtom(new Vector3D(x, y, z), symbol, 0.0));
      }

      if (molName.size() == 0) {
          molName = newMol->getName();
      } else {
          molNameTmp = newMol->getName();
          if (molName != molNameTmp) {
              std::ostringstream oss;
              oss << "Invalid vector of atoms in " << m_filename << ".";
              throw oss.str();
          }
      }

      // Répartition des charges homogène.
      std::vector<Atom*> atoms = *(newMol->getAllAtoms());
      for (int i = 0; i < atoms.size(); ++i) {
        atoms[i]->setCharge(1.0 / atoms.size());
      }

      moleculevector->push_back(newMol);
    }

    if (moleculevector->empty()) {
      std::ostringstream oss;
      oss << "Empty file : " << m_filename;
      throw oss.str();
    }
  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  if (std::getline(file, line)) {
    std::ostringstream oss;
    oss << "Incorrect format for " << m_filename << ".";
    throw oss.str();
  }

  file.close();

  return moleculevector;
}

double XyzFileReader::convertToDouble(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x)) {
    std::ostringstream oss;
    oss << "Impossible to convert " << s << " in number in " << m_filename;
    throw oss.str();
  }
  return x;
}
