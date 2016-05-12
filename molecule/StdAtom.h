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
 * \file StdAtom.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Class implementing the interface Atom.h
 * \details An atom is representing by a vector in a three-dimensional coordinate, a charge and a symbol.
 */


#ifndef STDATOM_H
#define STDATOM_H

#include "Atom.h"
#include "../math/Vector3D.h"

class StdAtom : public Atom {
  public:
    /**
     * Creates an atom at position pos of atomic symbol symb. Charge is set to
     * ch.
     */
    StdAtom(Vector3D* pos, std::string symb, double ch = 0.0);

    /**
     * Releases allocated resources.
     */
    virtual ~StdAtom();

    /**
     * Returns position of atom.
     */
    Vector3D* getPosition() const {return m_position;}
    /**
     * Returns the initial position of atom.
     */
    Vector3D* getInitialPosition() const {return m_initialPosition;}
    /**
     * Return symbol of atom.
     */
    std::string getSymbol() const {return m_symbol;}
    /**
     * Returns charge value of atom.
     */
    double getCharge() const {return m_charge;}

    /**
     * Sets a new position for the atom.
     * @param One coordinate.
     * @pre c != NULL
     */
    void setPosition(Vector3D *c);
    /**
     * Sets a new symbol value for atom.
     * @param A string value.
     * @pre n != ""
     */
    void setSymbol(std::string s);
    /**
     * Sets a new charge value for atom.
     * @param One double.
     */
    void setCharge(double c) {m_charge = c;}

  private:
    /**
     * Initial position of the atom.
     */
    Vector3D* m_initialPosition;
    /**
     * Actual position of the atom.
     */
    Vector3D* m_position;
    /**
     * Symbol of the atom.
     */
    std::string m_symbol;
    /**
     * Charge of the atom.
     */
    double m_charge;
};

#endif
