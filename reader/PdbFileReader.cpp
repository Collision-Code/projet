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

#include "PdbFileReader.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <boost/tokenizer.hpp>

#include "../molecule/StdMolecule.h"
#include "../molecule/StdAtom.h"

// BUILDER
PdbFileReader::PdbFileReader(std::string filename)
{
  setFileName(filename);
}

// DESTRUCTOR
PdbFileReader::~PdbFileReader() {}

// COMMANDS
void PdbFileReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("PdbFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".pdb") != 0) {
    throw std::string("PdbFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* PdbFileReader::loadResources() {
  std::ifstream file(m_filename.c_str(), std::ios::in);

  std::vector<Molecule*>* moleculevector = new std::vector<Molecule*>();

  if (file) {
    std::string line;
    std::string lineState("HETATM");

    Molecule* newMol = new StdMolecule();

    for (int i = 0; std::getline(file, line); i++) {
      boost::char_separator<char> sep(" ");
      boost::tokenizer<boost::char_separator<char> > tokenizeParam(line, sep);
      boost::tokenizer<boost::char_separator<char> >::iterator param =
        tokenizeParam.begin();

      if (lineState.compare(*param) == 0) {
        double x, y, z; /* Value in angstrom /!\ */
        std::string symbol;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param;
        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param;// Extract atom symbol
        symbol = *param;

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param;
        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param;
        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param; // Extract x coordinate
        x = convertToDouble((*param).c_str());

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param; // Extract y coordinate
        y = convertToDouble((*param).c_str());

        if (param == tokenizeParam.end()) {
            std::ostringstream oss;
            oss << "Invalid format of " << m_filename << ".";
            throw oss.str();
        }
        ++param; // Extract z coordinate
        z = convertToDouble((*param).c_str());

        newMol->addAtom(new StdAtom(new Vector3D(x, y, z), symbol, 0.0));
      }
    }
    if (newMol->getAtomNumber() > 0) {
      // Répartition des charges homogène.
      std::vector<Atom*> atoms = *(newMol->getAllAtoms());
      for (int i = 0; i < atoms.size(); ++i) {
        atoms[i]->setCharge(1.0 / atoms.size());
      }
      moleculevector->push_back(newMol);
    } else {
      std::ostringstream oss;
      oss << "Empty file or wrong format : " << m_filename << ".";
      throw oss.str();
    }
  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  file.close();

  return moleculevector;
}

double PdbFileReader::convertToDouble(const std::string& s)
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
