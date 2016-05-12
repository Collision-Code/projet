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

#ifndef MATHLIB_H
#define MATHLIB_H

#include <cstdlib>

#include "Vector3D.h"
#include "../molecule/Atom.h"
#include "../molecule/Molecule.h"

#include <vector>

/**
* Interface describing all the available mathematic functions.
*/
class MathLib
{
  public:
    virtual ~MathLib() {};

    /**
     * Rotates the molecule by angles specified on each axis.
     * \param mol the molecule to rotate.
     * \param angleX the angle of rotation one the X axis.
     * \param angleY the angle of rotation one the Y axis.
     * \param angleZ the angle of rotation one the Z axis.
     */
    virtual void rotate(Molecule* mol, double angleX, double angleY, double angleZ) = 0;

    /**
     * Rotates the position by angles specified on each axis.
     * \param pos the positions to rotate.
     * \param angleX the angle of rotation one the X axis.
     * \param angleY the angle of rotation one the Y axis.
     * \param angleZ the angle of rotation one the Z axis.
     */
    virtual void rotate(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos, double angleX, double angleY, double angleZ) = 0;

    /**
     * Rotates the molecule by random angles on each axis.
     * \param mol the molecule to rotate.
     */
    virtual void randomRotation(Molecule* mol) = 0;

    /**
     * Rotates the positions by random angles on each axis.
     * \param pos the positions to rotate.
     */
    virtual void randomRotation(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos) = 0;

    /**
    * Calculates the center of mass of a molecule.
    * \param mol the molecule.
    * \return the coordinates of the center of mass of the molecule mol.
    */
    virtual Vector3D calculateMassCenter(const Molecule& mol) = 0;

    /**
    * Finds the atom the farthest of the center of mass.
    * \param mol the molecule.
    * \return a pointer to the atom which is the farthest of the center of mass of mol.
    */
    virtual Atom* findFarthestAtom(const Molecule& mol) = 0;

    /**
    * Implementation of the Monte-Carlo method for calculating integrals.
    * \param f the function to integrate.
    * \param minLimit the lower limit of the integral.
    * \param maxLimit the upper limit of the integral.
    * \param n the number of points generate to calculate the integral. More points increase the result precision.
    * \return the result of the integration.
    */
    virtual double monteCarloIntegration(double (*f)(double), double minLimit, double maxLimit, int n) = 0;
};

#endif // MATHLIB_H
