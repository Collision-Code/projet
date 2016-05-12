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

#ifndef STDMATHLIB_H
#define STDMATHLIB_H

#include "MathLib.h"
#include "Vector3D.h"
#include "../molecule/Atom.h"
#include "../molecule/Molecule.h"

#include <vector>


/**
* StdMathLib implements the interface MathLib to give implementation to
* all the mathematic functions.
*/
class StdMathLib : public MathLib
{
  public:
    StdMathLib();
    virtual ~StdMathLib();

    /**
     * Rotates the molecule by angles specified on each axis.
     * \param mol the molecule to rotate.
     * \param angleX the angle of rotation one the X axis.
     * \param angleY the angle of rotation one the Y axis.
     * \param angleZ the angle of rotation one the Z axis.
     */
    void rotate(Molecule* mol, double angleX, double angleY, double angleZ);

    /**
     * Rotates the position by angles specified on each axis.
     * \param pos the positions to rotate.
     * \param angleX the angle of rotation one the X axis.
     * \param angleY the angle of rotation one the Y axis.
     * \param angleZ the angle of rotation one the Z axis.
     */
    void rotate(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos, double angleX, double angleY, double angleZ);

    /**
     * Rotates the molecule by random angles on each axis.
     * \param mol the molecule to rotate.
     */
    void randomRotation(Molecule* mol);

    /**
     * Rotates the positions by random angles on each axis.
     * \param pos the positions to rotate.
     */
    void randomRotation(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos);

    /**
    * Calculates the center of mass of a molecule.
    * \param mol the molecule.
    * \return the coordinates of the center of mass of the molecule mol.
    */
    Vector3D calculateMassCenter(const Molecule& mol);

    /**
    * Finds the atom the farthest of the center of mass.
    * \param mol the molecule.
    * \return a pointer to the atom which is the farthest of the center of mass of mol.
    */
    Atom* findFarthestAtom(const Molecule& mol);

    /**
    * Implementation of the Monte-Carlo method for calculating integrals.
    * \param f the function to integrate.
    * \param minLimit the lower limit of the integral.
    * \param maxLimit the upper limit of the integral.
    * \param n the number of points generate to calculate the integral. More points increase the result precision.
    * \return the result of the integration.
    */
    double monteCarloIntegration(double (*f)(double), double minLimit, double maxLimit, int n);
};

#endif // STDMATHLIB_H
