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
 * @file StdAtom.h
 *
 * Implements an atom.
 */

#include "StdAtom.h"
#include "../math/Vector3D.h"

#include "../general/AtomInformations.h"

#include <sstream>
#include <string>

StdAtom::StdAtom(Vector3D* pos, std::string symb, double ch)
  : m_position(pos), m_charge(ch)
{
  if (m_position == nullptr) {
    throw std::string("Atom position can't be null (constructor).");
  }

  if (!AtomInformations::getInstance()->isExistingSymbol(symb)) {
    std::ostringstream oss;
    oss << symb << " is not an existing symbol for an atom.";
    throw oss.str();
  }

  m_symbol = symb;
  m_initialPosition = new Vector3D(m_position->x, m_position->y, m_position->z);
}

StdAtom::~StdAtom() {
  delete m_initialPosition;
  delete m_position;
}

// COMMANDS
void StdAtom::setPosition(Vector3D *c) {
  if (c == nullptr) {
    throw std::string("Atom position can't be null.");
  }

  delete m_position;
  m_position = c;
}

void StdAtom::setSymbol(std::string s) {
  if (!AtomInformations::getInstance()->isExistingSymbol(s)) {
    std::ostringstream oss;
    oss << s << " is not an existing symbol for an atom.";
    throw oss.str();
  }

  m_symbol = s;
}


