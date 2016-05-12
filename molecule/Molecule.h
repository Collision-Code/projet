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
 * @file Molecule.h
 *
 * Representation d'une molecule.
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
     * \pre c != null
     * \param c a coordinate
     * \return the atom from the specified position.
     */
    virtual Atom* getAtom(const Vector3D& c) const = 0;

    /**
     * Replaces the molecule at its initial position.
     */
    virtual void toInitialPosition() = 0;
    /**
     * Replace the current name of molecule by a new one.
     * \pre n != ""
     * \param n a string value.
     */
    virtual void setName(std::string n) = 0;
    /**
     * Add an atom on the molecule.
     * \pre
     *  l <- getAllAtoms()
     *  for i <- l[0...l.length()-1]
     *    i.getPosition() != a->getPosition()
     * \param a a pointer on an atom.
     */
    virtual void addAtom(Atom* a) = 0;
    /**
     * Delete the specified atom.
     * \pre a != null
     * \param a a pointer on an atom.
     */
    virtual void deleteAtom(Atom* a) = 0;
    /**
     * Delete the atom at specified position.
     * \pre c != null
     * \param c a coordinate.
     */
    virtual void deleteAtom(const Vector3D& c) = 0;
};

#endif
