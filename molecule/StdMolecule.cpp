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

/**
 * @file StdMolecule.cpp
 *
 * Implements a molecule.
 */

#include "StdMolecule.h"

#include "../general/AtomInformations.h"

#include <sstream>
#include <map>


/**
 * Converts an integer to a string.
 * \param number, the integer to convert.
 * \return the converted integer in string
 */
const std::string intToString(int number);


StdMolecule::StdMolecule()
  : m_name("")
{
  m_atoms = new std::vector<Atom*>();
}

StdMolecule::~StdMolecule() {
  delete m_atoms;
}

std::string StdMolecule::getName()
{
  // On reconstruit le nom seulement si on n'en a pas.
  if (m_name == "") {
    // La map qui va contenir les symboles et leur nombre d'occurence.
    std::map<std::string, int> symbOcc;
    // On rempli la map avec tous les atomes.
    for (unsigned int i = 0; i < m_atoms->size(); ++i) {
      symbOcc[m_atoms->at(i)->getSymbol()]++;
    }
    // La map est remplie, on construit la chaine.
    for (auto it = symbOcc.begin(); it != symbOcc.end(); ++it) {
      m_name.append(it->first + intToString(it->second));
    }
  }

  // On retourne le nom.
  return m_name;
}

double StdMolecule::getTotalMass() const
{
  double m = 0.0;
  AtomInformations* aI = AtomInformations::getInstance();
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
      m += aI->getAtomicMass((*i)->getSymbol());
  }
  return m;
}

Atom* StdMolecule::getAtom(const Vector3D& c) const {
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
    if (*((*i)->getPosition()) == c) {
      return *i;
    }
  }
  return nullptr;
}

void StdMolecule::toInitialPosition()
{
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
    (*i)->setPosition((*i)->getInitialPosition());
  }
}

void StdMolecule::addAtom(Atom* a) {
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
    if (*((*i)->getPosition()) == *(a->getPosition())) {
      return;
    }
  }
  m_atoms->push_back(a);

  // Modification de la molécule, nom remis à zéro.
  m_name = "";
}

void StdMolecule::deleteAtom(Atom* a) {
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
    if (*((*i)->getPosition()) == *(a->getPosition())) {
      m_atoms->erase(i);
      return;
    }
  }

  std::ostringstream oss;
  oss << "Atom at position " << *(a->getPosition()) << " not found";
  throw oss.str();

  // Modification de la molécule, nom remis à zéro.
  m_name = "";
}

void StdMolecule::deleteAtom(const Vector3D& c) {
  for (std::vector<Atom*>::iterator i = m_atoms->begin(); i != m_atoms->end(); ++i) {
    if (*((*i)->getPosition()) == c) {
      i = m_atoms->erase(i);
      return;
    }
  }
  std::ostringstream oss;
  oss << "Atom at position " << c << " not found";
  throw oss.str();

  // Modification de la molécule, nom remis à zéro.
  m_name = "";
}

/**
 * Converts an integer to a string.
 * \param number, the integer to convert.
 * \return the converted integer in string
 */
const std::string intToString(int number)
{
  std::ostringstream oss;
  oss<< number;
  return oss.str();
}
