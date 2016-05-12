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

#ifndef STDMOLECULE_H
#define STDMOLECULE_H

#include "Atom.h"
#include "Molecule.h"
#include "../math/Vector3D.h"

#include <vector>
#include <string>

class StdMolecule : public Molecule {
  public:
    /**
     * Create an empty molecule.
     */
    StdMolecule();

    /**
     * Release allocates resources.
     */
    virtual ~StdMolecule();

    /**
     * \return the name of molecule.
     */
    std::string getName();
    /**
     * \return the total number of atom forming molecule composition.
     */
    unsigned int getAtomNumber() const {return m_atoms->size();}
    /**
     * \return the mass of the molecule.
     */
    double getTotalMass() const;
    /**
     * \return a pointer on atom collection.
     */
    std::vector<Atom*>* getAllAtoms() const {return m_atoms;}
    /**
     * \pre c != null
     * \param c a coordinate
     * \return the atom from the specified position.
     */
    Atom* getAtom(const Vector3D& c) const;

    /**
     * Replaces the molecule at its initial position.
     */
    void toInitialPosition();
    /**
     * Replace the current name of molecule by a new one.
     * \pre n != ""
     * \param n a string value.
     */
    void setName(std::string n) {m_name = n;}
    /**
     * Add an atom on the molecule.
     * \pre
     *  l <- getAllAtoms()
     *  for i <- l[0...l.length()-1]
     *    i.getPosition() != a->getPosition()
     * \param a a pointer on an atom.
     */
    void addAtom(Atom* a);
    /**
     * Delete the specified atom.
     * \pre a != null
     * \param a a pointer on an atom.
     */
    void deleteAtom(Atom* a);
    /**
     * Delete the atom at specified position.
     * \pre c != null
     * \param c a coordinate.
     */
    void deleteAtom(const Vector3D& c);

  private:
    /**
     * Atoms part of the molecule.
     */
    std::vector<Atom*>* m_atoms;
    /**
     * Molecule's name.
     */
    std::string m_name;
};

#endif
