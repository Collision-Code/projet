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

#include "MolFileReader.h"

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
MolFileReader::MolFileReader(std::string filename)
{
  setFileName(filename);
}

// DESTRUCTOR
MolFileReader::~MolFileReader() {}

// COMMANDS
void MolFileReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("MolFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".mol") != 0) {
    throw std::string("MolFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* MolFileReader::loadResources() {
  std::ifstream file(m_filename.c_str(), std::ios::in);

  std::vector<Molecule*>* moleculevector = new std::vector<Molecule*>();

  if (file) {
    std::string line;
    int atomNb = 0;
    Molecule* newMol = new StdMolecule();

    while (std::getline(file, line)) {
      if (atomNb == 0) {
        if (line.find("V2") != std::string::npos
            || line.find("V1") != std::string::npos) {
          boost::tokenizer<> tokenizeParam(line);
          boost::tokenizer<>::iterator firstParam = tokenizeParam.begin();

          atomNb = atoi((*firstParam).c_str());
        }
      } else {
        boost::char_separator<char> sep(" \t");
        boost::tokenizer<boost::char_separator<char>> tokenizeParam(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator param = tokenizeParam.begin();

        while (param == tokenizeParam.end()) {
          if (!(std::getline(file, line))) {
            throw std::string("MolFileReader(loadResources) : file invalid.");
            return nullptr;
          }
          boost::tokenizer<boost::char_separator<char>> tokenizeParam(line, sep);
          param = tokenizeParam.begin();
        }

        for (auto i = 0; i < atomNb; i++) {
          double x, y, z;
          std::string symbol;

          boost::tokenizer<boost::char_separator<char>> tokenizePara(line, sep);
          param = tokenizePara.begin();

          if (param == tokenizeParam.end()) throw std::string("Invalid file.");

          x = convertToDouble((*param).c_str());
          y = convertToDouble((*(++param)).c_str());
          z = convertToDouble((*(++param)).c_str());
          symbol = *(++param);

          auto symb = AtomInformations::getInstance()->getAtomicNumber(*param);

          newMol->addAtom(new StdAtom(new Vector3D(x, y, z), symbol, 0.0));
          std::getline(file, line);
        }
        break;
      }
    }

    if (newMol->getAllAtoms()->empty()) {
      std::ostringstream oss;
      oss << "Empty file : " << m_filename << ".";
      throw oss.str();
    }

    // Répartition des charges homogène.
    std::vector<Atom*> atoms = *(newMol->getAllAtoms());
    for (int i = 0; i < atoms.size(); ++i) {
      atoms[i]->setCharge(1.0 / atoms.size());
    }

    moleculevector->push_back(newMol);
  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  if (moleculevector->empty()) {
    std::ostringstream oss;
    oss << "Empty file : " << m_filename << ".";
    throw oss.str();
  }

  file.close();

  return moleculevector;
}

double MolFileReader::convertToDouble(const std::string& s)
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
