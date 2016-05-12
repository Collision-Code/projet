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

#include "AtomInformations.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>

AtomInformations* AtomInformations::m_instance = new AtomInformations();

AtomInformations::AtomInformations()
{
  //ctor
}

AtomInformations::~AtomInformations()
{
  //dtor
}

void AtomInformations::loadFile(std::string fileName)
{
  std::ifstream file(fileName, std::ifstream::in);
  if (!file.is_open()) {
    std::ostringstream oss;
    oss << "Impossible to open " << fileName << " to load atom informations.";
    throw oss.str();
  }

  // On vide la map.
  m_atomInfos.clear();

  // Le fichier est ouvert.
  std::string line;
  // On ignore la première ligne puisqu'elle contient les noms des
  // colonnes.
  if (!std::getline(file, line)) {
    std::ostringstream oss;
    oss << "File " << fileName << "is empty.";
    throw oss.str();
  }

  // On boucle jusqu'à la fin du fichier.
  while (file.good()) {
    std::getline(file, line);
    std::stringstream sline(line);

    // On lit le symbol chimique.
    std::string symbol;
    if (!std::getline(sline, symbol, ',')) {
      break;
    }

    // Le tableau pour les colonnes.
    std::vector<std::string> columns;
    std::string value;
    // On boucle sur toutes les colonnes.
    for (int i = 0; i < AtomInformations::COLUMN_NUMBER; ++i) {
      if (!std::getline(sline, value, ',')) {
        // Pas de valeur dans la colonne.
        std::ostringstream oss;
        oss << "No value in " << fileName << ", column " << i + 2 << ".";
        throw oss.str();
      }
      if (value == "") {
        // Chaine vide dans la colonne.
        std::ostringstream oss;
        oss << "Empty value in " << fileName << ", column " << i + 2 << ".";
        throw oss.str();
      }
      columns.push_back(value);
    }
    // On ajoute l'association symbol-colonnes à la map.
    m_atomInfos.insert(std::pair<std::string, std::vector<std::string>>(symbol, columns));
  }

  if (m_atomInfos.empty()) {
        // Fichier ne contenant aucune donnée.
        std::ostringstream oss;
        oss << "File " << fileName << "is empty.";
        throw oss.str();
  }
}

std::string AtomInformations::getSymbol(int atomicMass)
{
  // On va boucler sur toute la map, et chercher la masse arrondie.
  // Si on la trouve, on la retourne.
  for (auto it = m_atomInfos.begin(); it != m_atomInfos.end(); ++it) {
    std::string s = it->second[AtomInformations::ATOMIC_MASS];

    // Si s ne commence pas par un chiffre, c'est une erreur.
    if (!isdigit(s[0])) {
      std::ostringstream oss;
      oss << "Atomic mass for symbol " << it->first << " can't be " << s << "." << std::endl;
      oss << "Error in searching for symbol for integer atomic mass " << atomicMass << ".";
      throw oss.str();
    }

    float massF = atof(s.c_str());
    if (std::round(massF) == atomicMass) {
      return it->first;
    }
  }

  // On n'a rien trouvé, exception.
  std::ostringstream oss;
  oss << "Symbol for integer atomic mass " << atomicMass << " can't be found.";
  throw oss.str();
}

int AtomInformations::getAtomicNumber(std::string symb) const
{
  auto it = m_atomInfos.find(symb);
  if (it == m_atomInfos.end()) {
    throw std::string("Atomic symbol " + symb + " not known.");
  }

  std::string s = it->second[AtomInformations::ATOMIC_NUMBER];

  // Si s ne commence pas par un chiffre, c'est une erreur.
  if (!isdigit(s[0])) {
    std::ostringstream oss;
    oss << "Atomic number for symbol " << symb << " can't be " << s << ".";
    throw oss.str();
  }

  return atoi(s.c_str());
}

double AtomInformations::getAtomicMass(std::string symb)
{
  auto mass = m_massesValues.find(symb);
  if (mass == m_massesValues.end()) {
    // Si la masse n'est pas encore enregistrée, on l'enregistre.
    auto it = m_atomInfos.find(symb);
    if (it == m_atomInfos.end()) {
      throw std::string("Atomic symbol " + symb + " not known.");
    }

    std::string s = it->second[AtomInformations::ATOMIC_MASS];

    // Si s ne commence pas par un chiffre, c'est une erreur.
    if (!isdigit(s[0])) {
      std::ostringstream oss;
      oss << "Atomic mass for symbol " << symb << " can't be " << s << ".";
      throw oss.str();
    }

    float massF = atof(s.c_str());
    m_massesValues.insert(std::pair<std::string, float>(symb, massF));
    return massF;
  }

  // La masse est déjà enregistrée, on la retourne juste.
  return mass->second;
}

double AtomInformations::getEOLJHe(std::string symb)
{
  auto eolj = m_EOLJHeValues.find(symb);
  if (eolj == m_EOLJHeValues.end()) {
    // Si la masse n'est pas encore enregistrée, on l'enregistre.
    auto it = m_atomInfos.find(symb);
    if (it == m_atomInfos.end()) {
      throw std::string("Atomic symbol " + symb + " not known.");
    }

    std::string s = it->second[AtomInformations::EOLJ_He];

    // Si s ne commence pas par un chiffre, c'est une erreur.
    if (!isdigit(s[0])) {
      std::ostringstream oss;
      oss << "EOLJ for Helium for symbol " << symb << " can't be " << s << ".";
      throw oss.str();
    }

    float eoljF = atof(s.c_str());
    m_EOLJHeValues.insert(std::pair<std::string, float>(symb, eoljF));
    return eoljF;
  }

  // La masse est déjà enregistrée, on la retourne juste.
  return eolj->second;
}

double AtomInformations::getROLJHe(std::string symb)
{
  auto rolj = m_ROLJHeValues.find(symb);
  if (rolj == m_ROLJHeValues.end()) {
    // Si la masse n'est pas encore enregistrée, on l'enregistre.
    auto it = m_atomInfos.find(symb);
    if (it == m_atomInfos.end()) {
      throw std::string("Atomic symbol " + symb + " not known.");
    }

    std::string s = it->second[AtomInformations::ROLJ_He];

    // Si s ne commence pas par un chiffre, c'est une erreur.
    if (!isdigit(s[0])) {
      std::ostringstream oss;
      oss << "ROLJ for Helium for symbol " << symb << " can't be " << s << ".";
      throw oss.str();
    }

    float roljF = atof(s.c_str());
    m_EOLJHeValues.insert(std::pair<std::string, float>(symb, roljF));
    return roljF;
  }

  // La masse est déjà enregistrée, on la retourne juste.
  return rolj->second;
}

double AtomInformations::getHSRadius(std::string symb)
{
  auto rhs = m_rhsValues.find(symb);
  if (rhs == m_rhsValues.end()) {
    // Si RHS n'est pas encore enregistré, on l'enregistre.
    auto it = m_atomInfos.find(symb);
    if (it == m_atomInfos.end()) {
      throw std::string("Atomic symbol " + symb + " not known.");
    }

    std::string s = it->second[AtomInformations::HARD_SPHERE_RADIUS];

    // Si s ne commence pas par un chiffre, c'est une erreur.
    if (!isdigit(s[0])) {
      std::ostringstream oss;
      oss << "Hard sphere radius for symbol " << symb << " can't be " << s << ".";
      throw oss.str();
    }

    float val = atof(s.c_str());
    m_rhsValues.insert(std::pair<std::string, float>(symb, val));
    return val;
  }

  // RHS est déjà enregistré, on le retourne juste.
  return rhs->second;
}
