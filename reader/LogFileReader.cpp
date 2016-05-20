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

#include "LogFileReader.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

#include "../lib/boost/tokenizer.hpp"

#include "../molecule/StdMolecule.h"
#include "../molecule/StdAtom.h"

#include "../general/AtomInformations.h"

// BUILDER
LogFileReader::LogFileReader(std::string filename)
{
  setFileName(filename);
}

// DESTRUCTOR
LogFileReader::~LogFileReader() {}

// COMMANDS
void LogFileReader::setFileName(std::string filename) {
  size_t len = filename.length();
  if (len < 1) {
    throw std::string("LogFileReader(setFileName) : Empty filename.");
    return;
  }
  if (filename.substr(len-4, len).compare(".out") != 0
      && filename.substr(len-4, len).compare(".log") != 0) {
    throw std::string("LogFileReader(setFileName) : Wrong file name extension.");
    return;
  }
  m_filename = filename;
}

std::vector<Molecule*>* LogFileReader::loadResources() {
  std::ifstream file(m_filename.c_str(), std::ios::in); // Main open file
  std::ifstream reFile(m_filename.c_str(), std::ios::in); // For trying to backup informations

  std::vector<Molecule*>* moleculeList = new std::vector<Molecule*>(); // Result to send
  bool geometryFound = false; // Back-up watch

  std::string isPopOption(""); // Initializing pop option finder

  if (file) {
    std::string line; // Line of file we are currently working
    int atomNumber = -1; // Atom number of current molecule
    Molecule* newMol = new StdMolecule();

    // We are searching for first command line begining by '#'
    if (!(std::getline(file, line)))
      throw std::string("LogFileReader(loadResources) : empty file.");

    // Verify if file is valid
    while (std::getline(file, line)) {
      boost::char_separator<char> sep(", \t");
      boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
      boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();

      if (itLine != tokLine.end()) {
        if ((*itLine).find("#") != std::string::npos) {
          break;
        }
      }
    }

    // We save command line
    std::string commandLine(line);

    // We add rest of command line now
    while (line.find("----") == std::string::npos) {
      if (!(std::getline(file, line))) {
        throw std::string("LogFileReader(loadResources) : command line error.");
      }

      if (line.find("----") == std::string::npos) commandLine.append(line);
    }

    // We are looking for opt option.
    // Search of molecule composition of atoms.
    if (commandLine.find(" opt") != std::string::npos
        || commandLine.find(" /opt") != std::string::npos
        || commandLine.find(" opt=(") != std::string::npos
        || commandLine.find(" opt(") != std::string::npos) {
      while (line.find("Stationary") == std::string::npos) {
        if (atomNumber < 0) atomNumber = isThereAtomNumber(line);
        if (!(std::getline(file, line))) {
          throw std::string("LogFileReader(loadResources) : stationary not found.");
          return nullptr;
        }
      }
      // We are now searching for element position.
      while (line.find("Multiplicity") == std::string::npos) {
        if (atomNumber < 0) {
          atomNumber = isThereAtomNumber(line);
        }
        if (!(std::getline(file, line))) break;
      }

      if (line.find("Multiplicity") != std::string::npos) geometryFound = true;

      if (geometryFound) {
        boost::char_separator<char> sep(", \t");
        boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();
        int ret = 1;
        while (ret) {
          try {
            if (itLine != tokLine.end()) {
              AtomInformations::getInstance()->getAtomicNumber(*(itLine));
              break;
            }

          } catch (std::string &exc) {

          }
          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : error for searching atoms.");
            return nullptr;
          }
          boost::tokenizer<boost::char_separator<char>> tok(line, sep);
          itLine = tok.begin();
        }

        // Position of molecule composition found.
        // Now we insert in molecule the atoms.
        for (auto j = 0; j < atomNumber; j++) {
          boost::char_separator<char> sep(", \t");
          boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();

          double pos[3];
          std::string symbol;

          for (auto k = 0; itLine != tokLine.end(); k++) {
            if (k == 1) {
              ++itLine;
              continue;
            }
            if (k == 0) {
              symbol = (*itLine);
              ++itLine;
              continue;
            }
            pos[k-2] = strtod((*itLine).c_str(), nullptr);
            ++itLine;
          }

          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : error for searching atoms.");
            return nullptr;
          }
          newMol->addAtom(new StdAtom(
              new Vector3D(pos[0], pos[1], pos[2]), symbol, 0.0)
          );
        }
      } else {
        file.close();

        if (atomNumber < 0) {
          throw std::string("LogFileReader(loadResources) : File invalid.");
          return nullptr;
        }
        while (std::getline(reFile, line))
          if (line.find("Multiplicity") != std::string::npos) break;

        for (auto k = 0; k < atomNumber; k++) {
          if (!(std::getline(reFile, line))) {
            throw std::string("LogFileReader(loadResources) : File invalid.");
            return nullptr;
          }
          boost::char_separator<char> sep(" \t");
          boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();

          // We reach an empty line then end of statement.
          if (itLine == tokLine.end()) break;

          double pos[3];
          std::string symbol;

          for (auto k = 0; itLine != tokLine.end(); k++) {
            if (k == 0) {
              symbol = (*itLine);
              ++itLine;
              continue;
            }
            pos[k-1] = strtod((*itLine).c_str(), nullptr);
            ++itLine;
          }
          newMol->addAtom(new StdAtom(
              new Vector3D(pos[0], pos[1], pos[2]), symbol, 0.0)
          );
        }
        // We test if file is valid.
        while (std::getline(reFile, line)) {
          if (line.find("Stationary") != std::string::npos) break;
        }
        if (line.find("Stationary") == std::string::npos) {
          throw std::string("LogFileReader(loadResources) : reFile invalid");
          return nullptr;
        }
      }

      isPopOption = findPopOption(commandLine);
    } else {
      // We are searching for element position.
      while (line.find("Multiplicity") == std::string::npos) {
        if (atomNumber < 0) {
          atomNumber = isThereAtomNumber(line);
        }
        if (!(std::getline(file, line))) {
          throw std::string("LogFileReader(loadResources) : no geometry in file.");
          return nullptr;
        }
      }
      if (line.find("Multiplicity") != std::string::npos) geometryFound = true;
      // Position of molecule composition found.
      // Now we insert in molecule the atoms.
      while (std::getline(file, line)) {
        boost::char_separator<char> sep(" \t");
        boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();

        // We reach an empty line then end of statement.
        if (itLine == tokLine.end()) break;

        double pos[3];
        std::string symbol;

        for (auto k = 0; itLine != tokLine.end(); k++) {
          if (k == 0) {
            symbol = (*itLine);
            ++itLine;
            continue;
          }
          pos[k-1] = strtod((*itLine).c_str(), nullptr);
          ++itLine;
        }
        newMol->addAtom(new StdAtom(
            new Vector3D(pos[0], pos[1], pos[2]), symbol, 0.0)
        );
      }

      isPopOption = findPopOption(commandLine);
    }

    if (geometryFound)
      chargeLoading(isPopOption, file, commandLine, newMol);
    else
      chargeLoading(isPopOption, reFile, commandLine, newMol);

    moleculeList->push_back(newMol);
  } else {
    std::ostringstream oss;
    oss << "Cannot open file " << m_filename << ".";
    throw oss.str();
  }

  if (!geometryFound) file.close();
  reFile.close();

  return moleculeList;
}

/**
 * Check in current line if there is an atom number information
 * \param string containing current line we work with
 * \return an integer meaning number of atom composing molecule
 */
int LogFileReader::isThereAtomNumber(std::string line) {
  boost::char_separator<char> sep(" \t");
  boost::tokenizer<boost::char_separator<char>> tokLine(line, sep);
  boost::tokenizer<boost::char_separator<char>>::iterator itLine = tokLine.begin();
  // If void line
  if (itLine == tokLine.end()) return -1;
  // Else
  if ((*itLine).compare("NAtoms=") == 0) return atoi((*(++itLine)).c_str());

  return -1;
}
/**
 * Find in line given option choice to make for reaching charges
 * \param string containing a line (command line)
 * \return string containing option to make
 */
std::string LogFileReader::findPopOption(std::string commandLine) {
  std::string option("none"); // option return by function
  int foundPopEnd = 0; // last char concerning pop option
  int found = commandLine.find(" pop="); // first char of pop option
  // If pop option does not exists
  if (found == std::string::npos) return option;

  // Now we search what option is in.
  std::string popOptionCommand;

  // If pop option contain pop=(something)
  if (commandLine[found+5] == '(' || commandLine[found+6] == '(')
    foundPopEnd = commandLine.substr(found, commandLine.length()).find_first_of(')');
  else {
    int j = 0;
    const char *c_string = commandLine.c_str();
    for (foundPopEnd = found+1; commandLine[foundPopEnd] != ' '; foundPopEnd++){
      popOptionCommand += c_string[foundPopEnd];
      j++;
    }
  }


  if (popOptionCommand.find("hirshfeld") != std::string::npos
      || popOptionCommand.find("Hirshfeld") != std::string::npos
      || popOptionCommand.find("HIRSHFELD") != std::string::npos) {
    option.clear();
    option.append("hirshfeld");
  } else if (popOptionCommand.find("npa") != std::string::npos
      || popOptionCommand.find("Npa") != std::string::npos
      || popOptionCommand.find("NPA") != std::string::npos) {
    option.clear();
    option.append("npa");
  } else if (popOptionCommand.find("nbo") != std::string::npos
      || popOptionCommand.find("Nbo") != std::string::npos
      || popOptionCommand.find("NBO") != std::string::npos) {
    option.clear();
    option.append("nbo");
  } else if (popOptionCommand.find("mk") != std::string::npos
      || popOptionCommand.find("Mk") != std::string::npos
      || popOptionCommand.find("MK") != std::string::npos) {
    option.clear();
    option.append("mk");
  } else if (popOptionCommand.find("chelp") != std::string::npos
      || popOptionCommand.find("Chelp") != std::string::npos
      || popOptionCommand.find("CHELP") != std::string::npos) {
    option.clear();
    option.append("chelp");
  } else if (popOptionCommand.find("chelpg") != std::string::npos
      || popOptionCommand.find("Chelpg") != std::string::npos
      || popOptionCommand.find("CHELPG") != std::string::npos) {
    option.clear();
    option.append("chelpg");
  }

  return option;
}

/**
 * Function taking parameters for parsing file ending and reach charges atoms
 *  informations.
 * \param string containing result of line command research
 * \param an opened file where we continue to reach informations
 * \param string containing full command line
 * \param pointer of Molecule showing molecule composition
 * 
 */
void LogFileReader::chargeLoading(std::string isPopOption, std::ifstream &file,
        std::string commandLine, Molecule *newMol) {
  std::string line;
  if (isPopOption.compare("none") == 0) {
    if (commandLine.find(" freq ") != std::string::npos) {
      // We are searching for APT charges
      while (line.find("APT charges:") == std::string::npos) {
        if (!(std::getline(file, line))) {
          throw std::string("LogFileReader(loadResources) : freq error file.1");
          return;
        }
      }
      // We jump onto next line
      for (auto k = 0; k < 2; k++)
        if (!(std::getline(file, line))) {
          throw std::string("LogFileReader(loadResources) : freq error file.2");
          return;
        }
      // Extract of apt charges
      for (std::vector<Atom*>::iterator it = newMol->getAllAtoms()->begin();
          it != newMol->getAllAtoms()->end(); it++) {

        boost::char_separator<char> sep(" \t");
        boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
        boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();

        for (auto i = 0; i < 2; i++) ++itChrg;

        (*it)->setCharge(strtod((*itChrg).c_str(), nullptr));

        if (!(std::getline(file, line))) {
          throw std::string("LogFileReader(loadResources) : freq error file.");
          return;
        }
      }
    } else {
      // We are, instead, searching for last Mulliken charge in file
      while (std::getline(file, line)) {
        // if we find an other stationary we stop.
        if (line.find("Stationary point found") != std::string::npos) break;
        // else we are trying to find mullikens charges.
        if (line.find("Mulliken charges:") != std::string::npos) {
          // We jump onto next line
          for (auto k = 0; k < 2; k++)
            if (!(std::getline(file, line))) {
              throw std::string("LogFileReader(loadResources) : freq error file.");
              return;
            }
          // Extract of apt charges
          for (std::vector<Atom*>::iterator it = newMol->getAllAtoms()->begin();
              it != newMol->getAllAtoms()->end(); it++) {

            boost::char_separator<char> sep(" \t");
            boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
            boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();

            for (auto i = 0; i < 2; i++) ++itChrg;

            (*it)->setCharge(strtod((*itChrg).c_str(), nullptr));

            if (!(std::getline(file, line))) {
              throw std::string("LogFileReader(loadResources) : freq error file.");
              return;
            }
          }
        }
      }
    }
  } else if (isPopOption.compare("hirshfeld") == 0) {
    while (std::getline(file, line)) {
      // if we find an other stationary we stop.
      if (line.find("Stationary point found") != std::string::npos) break;

      if (line.find("Hirshfeld charges") != std::string::npos
          && line.find("Hirshfeld charges with") == std::string::npos) {

        boost::char_separator<char> sep(" \t");
        // We jump onto next line
        for (auto i = 0; i < 6; i++) {
          boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();
          if ((*itChrg).compare("1") == 0) break;
          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : hirshfeld error file.");
            return;
          }
          if (i == 5) {
            throw std::string("LogFileReader(loadResources) : hirshfeld format error.");
            return;
          }
        }
        // Extract of apt charges
        for (std::vector<Atom*>::iterator it = newMol->getAllAtoms()->begin();
            it != newMol->getAllAtoms()->end(); it++) {

          boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();

          for (auto i = 0; i < 2; i++) ++itChrg;

          (*it)->setCharge(strtod((*itChrg).c_str(), nullptr));

          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : hirshfeld error file.");
            return;
          }
        }
      }
    }
  } else if (isPopOption.compare("npa") == 0 || isPopOption.compare("nbo") == 0) {
    while (std::getline(file, line)) {
      // if we find an other stationary we stop.
      if (line.find("Stationary point found") != std::string::npos) break;

      if (line.find("Summary of Natural Population Analysis") != std::string::npos) {
        boost::char_separator<char> sep(" \t");
        // We jump onto next line
        for (auto i = 0; i < 10; i++) {
          boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();

          if (itChrg != tokchargeLine.end()) {
            if ((*itChrg).find("-----") == std::string::npos) {
              if ((*(++itChrg)).compare("1") == 0) break;
            }
          }

          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : npa/nbo error file.");
            return;
          }
          if (i == 9) {
            throw std::string("LogFileReader(loadResources) : npa/nbo format error.");
            return;
          }
        }
        // Extract of charges
        for (std::vector<Atom*>::iterator it = newMol->getAllAtoms()->begin();
            it != newMol->getAllAtoms()->end(); it++) {

          boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();
          for (auto i = 0; i < 2; i++) ++itChrg;

          (*it)->setCharge(strtod((*itChrg).c_str(), nullptr));

          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : npa/nbo error file.");
            return;
          }
        }
      }
    }
  } else if (isPopOption.compare("mk") == 0 || isPopOption.compare("chelp") == 0
      || isPopOption.compare("chelpg") == 0) {
    while (std::getline(file, line)) {
      if (line.find("ESP charges:") != std::string::npos) {
        boost::char_separator<char> sep(" \t");
        // We jump onto the right line.
        for (auto i = 0; i < 2; i++) {
          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : npa/nbo error file.");
            return;
          }
        }
        // Extract of charges
        for (std::vector<Atom*>::iterator it = newMol->getAllAtoms()->begin();
            it != newMol->getAllAtoms()->end(); it++) {

          boost::tokenizer<boost::char_separator<char>> tokchargeLine(line, sep);
          boost::tokenizer<boost::char_separator<char>>::iterator itChrg = tokchargeLine.begin();
          for (auto i = 0; i < 2; i++) ++itChrg;

          (*it)->setCharge(strtod((*itChrg).c_str(), nullptr));

          if (!(std::getline(file, line))) {
            throw std::string("LogFileReader(loadResources) : npa/nbo error file.");
            return;
          }
        }
      }
    }
  }
}
