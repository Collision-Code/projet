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
 * \file Atom.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Interface describing the Atom model.
 * \details An atom is representing by a vector in a three-dimensional coordinate, a charge and a symbol.
 */

#ifndef ATOM_H
#define ATOM_H

#include <string>

#include "../math/Vector3D.h"

class Atom {
  public:
    /**
     * Releases allocated resources.
     */
    virtual ~Atom() {};

    /**
     * Returns position of atom.
     */
    virtual Vector3D* getPosition() const = 0;
    /**
     * Returns the initial position of atom.
     */
    virtual Vector3D* getInitialPosition() const = 0;
    /**
     * Returns symbol of atom.
     */
    virtual std::string getSymbol() const = 0;
    /**
     * Returns charge value of atom.
     */
    virtual double getCharge() const = 0;

    /**
     * Sets a new position for the atom.
     * @param One coordinate.
     * @pre c != NULL
     */
    virtual void setPosition(Vector3D *c) = 0;
    /**
     * Sets a new symbol value for atom.
     * @param A string value.
     * @pre n != ""
     */
    virtual void setSymbol(std::string s) = 0;
    /**
     * Sets a new charge value for atom.
     * @param One double.
     */
    virtual void setCharge(double c) = 0;
};

#endif // __ATOM_H
