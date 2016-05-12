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
 * \file GeometryCalculator.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Interface describing calculations and results.
 * \details A facade which oversees calculation and results methods.
 */

#ifndef GEOMETRYCALCULATOR_H
#define GEOMETRYCALCULATOR_H

#include <vector>

#include "../molecule/Molecule.h"
#include "../math/Result.h"

class GeometryCalculator {
    public:
    /**
     * Structure containing values for calculation.
     */
    struct CalculationValues {
      double temperature;
      double potentialEnergyStart;
      double timeStepStart;
      double potentialEnergyCloseCollision;
      double timeStepCloseCollision;
      double numberCyclesTM;
      double numberPointsVelocity;
      double numberPointsMCIntegrationTM;
      double energyConservationThreshold;
      double numberPointsMCIntegrationEHSSPA;
    };

  public:
    /**
     * Releases all allocated resources.
     */
    virtual ~GeometryCalculator() {};

    /**
     * \return true if EHSS will be calculated, false otherwise.
     */
    virtual bool willEHSSBeCalculated() const = 0;

    /**
     * \return true if PA will be calculated, false otherwise.
     */
    virtual bool willPABeCalculated() const = 0;

    /**
     * \return true if TM will be calculated, false otherwise.
     */
    virtual bool willTMBeCalculated() const = 0;

    /**
     * Indicates if calculations are finished for the molecule.
     * \param mol the molecule.
     * \return true if calculations are finished for the molecule, false otherwise.
     */
    virtual bool areCalculationsFinished(Molecule* mol) const = 0;

    /**
     * Returns the results of CCS calculation of a molecule.
     * \param mol the molecule.
     * \return the results for the molecule.
     */
    virtual Result* getResults(Molecule* mol) const = 0;

    /**
     * Returns the values used for the calculations.
     * \return the values used for the calculations.
     */
    virtual CalculationValues getCalculationValues() const = 0;

    /**
     * Forces the GeometryCalculator to save the calculation values from
     * GlobalParameters at this moment.
     */
    virtual void saveCalculationValues() = 0;

    /**
     * Indicates if yes or no, EHSS should be calculated.
     * \param b true if EHSS should be calculated, else otherwise.
     */
    virtual void shouldEHSSBeCalculated(bool b) = 0;

    /**
     * Indicates if yes or no, PA should be calculated.
     * \param b true if PA should be calculated, else otherwise.
     */
    virtual void shouldPABeCalculated(bool b) = 0;

    /**
     * Indicates if yes or no, TM should be calculated.
     * \param b true if TM should be calculated, else otherwise.
     */
    virtual void shouldTMBeCalculated(bool b) = 0;

    /**
     * Sets a vector of molecules (geometries) for CCS calculation.
     * \param geometries a vector of geometries.
     */
    virtual void setGeometries(std::vector<Molecule*>* geometries) = 0;

    /**
     * Launches all the calculations, on all geometries.
     */
    virtual void launchCalculations() = 0;
};

#endif
