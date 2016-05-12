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

#include "MfjFileReader.h"

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
MfjFileReader::MfjFileReader(std::string filename) {
  setFileName(filename);
}

// DESTRUCTOR
MfjFileReader::~MfjFileReader() {}

// COMMANDS
void MfjFileReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("MfjFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".mfj") != 0) {
    throw std::string("MfjFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* MfjFileReader::loadResources() {
  std::ifstream file(m_filename.c_str(), std::ios::in);

  std::vector<Molecule*>* moleculevector = new std::vector<Molecule*>();

  AtomInformations* aI = AtomInformations::getInstance();

  if (file) {
    std::string line;
    std::string label;
    std::string unitLength;
    std::string chargeDistribution;
    std::string molName;
    std::string molNameTmp;

    int lineNb = 1;
    if (!(std::getline(file, label))) {
        std::ostringstream oss;
        oss << "Empty file : " << m_filename << ".";
        throw oss.str();
    }
    lineNb++;

    std::getline(file, line);
    int geometriesNb = atoi(line.c_str());
    lineNb++;

    std::getline(file, line);
    int atomsNb = atoi(line.c_str());
    lineNb++;

    std::getline(file, unitLength);
    lineNb++;

    std::getline(file, chargeDistribution);
    lineNb++;

    std::getline(file, line);
    double correcter = strtod(line.c_str(), nullptr);
    const double bohrRadiusToAngstrom = 0.5291772108;
    double corrections = correcter;

    if (unitLength == "au") {
        corrections *= bohrRadiusToAngstrom;
    }

    double charge;
    if (chargeDistribution == "equal") {
        charge = 1.0 / atomsNb;
    } else {
        charge = 0.0;
    }

    for (int i = 0; i < geometriesNb; i++) {
      Molecule* newMol = new StdMolecule();

      for (int j = 0; j < atomsNb; j++) {
        lineNb++;
        if (!std::getline(file, line)) {
            std::ostringstream oss;
            oss << "Don't contain the right number of atoms in at least one molecule in " << m_filename << ".";
            throw oss.str();
        }
        double x, y, z; /* Les valeurs recuperees sont en angstrom /!\ */
        int symbol;
        boost::char_separator<char> sep("\t ");

        boost::tokenizer<boost::char_separator<char>> tokenizeParam(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator param = tokenizeParam.begin();

        int colNb = 1;
        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data on line " << lineNb << " column " << colNb << " in " << m_filename << ".";
            throw oss.str();
        }
        x = convertToDouble((*(param)).c_str()) * corrections;
        colNb++;
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data on line " << lineNb << " column " << colNb << " in " << m_filename << ".";
            throw oss.str();
        }
        y = convertToDouble((*(param)).c_str()) * corrections;
        colNb++;
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data on line " << lineNb << " column " << colNb << " in " << m_filename << ".";
            throw oss.str();
        }
        z = convertToDouble((*(param)).c_str()) * corrections;
        colNb++;
        param++;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid data on line " << lineNb << " column " << colNb << " in " << m_filename << ".";
            throw oss.str();
        }
        symbol = atoi((*(param)).c_str());
        colNb++;
        param++;

        if (chargeDistribution == "calc") {
            if (param == tokenizeParam.end()) {
                std::ostringstream oss;
                oss << "Invalid data on line " << lineNb << " column " << colNb << " in " << m_filename << ".";
                throw oss.str();
            }
            charge = convertToDouble((*(param)).c_str());
        }
        newMol->addAtom(new StdAtom(new Vector3D(x, y, z), aI->getSymbol(symbol), charge));
      }

      if (i < geometriesNb - 1) {
        std::getline(file, line); // Eat blank line
        lineNb++;
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

      moleculevector->push_back(newMol);
    }
  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  file.close();

  return moleculevector;
}



double MfjFileReader::convertToDouble(const std::string& s)
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
