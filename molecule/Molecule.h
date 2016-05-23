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
 * \file Molecule.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief An interface describing a way of representing a molecule.
 */

#ifndef MOLECULE_H
#define MOLECULE_H

#include "Atom.h"
#include "../math/Vector3D.h"

#include <vector>
#include <string>

class Molecule {
  public:
    /**
     * Release allocates resources.
     */
    virtual ~Molecule() {};

    /**
     * \return the name of molecule.
     */
    virtual std::string getName() = 0;

    /**
     * \return the total number of atom forming molecule composition.
     */
    virtual unsigned int getAtomNumber() const = 0;

    /**
     * \return the mass of the molecule.
     */
    virtual double getTotalMass() const = 0;

    /**
     * \return a pointer on atom collection.
     */
    virtual std::vector<Atom*>* getAllAtoms() const = 0;

    /**
     * \param c a coordinate
     * \return the atom from the specified position.
     */
    virtual Atom* getAtom(const Vector3D& c) const = 0;

    /**
     * Replaces the molecule at its initial position.
     */
    virtual void toInitialPosition() = 0;

    /**
     * Replaces the current name of molecule by a new one.
     * \param n a string value.
     */
    virtual void setName(std::string n) = 0;

    /**
     * Adds an atom on the molecule.
     * \param a a pointer on an atom.
     */
    virtual void addAtom(Atom* a) = 0;

    /**
     * Deletes the specified atom.
     * \param a a pointer on an atom.
     */
    virtual void deleteAtom(Atom* a) = 0;

    /**
     * Deletes the atom at specified position.
     * \param c a coordinate.
     */
    virtual void deleteAtom(const Vector3D& c) = 0;
};

#endif
