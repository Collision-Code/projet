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
 * \file StdGeometryCalculator.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Class implementing the interface GeometryCalculator.h
 * \details A class which oversees calculations and results.
 */

#ifndef STDGEOMETRYCALCULATOR_H
#define STDGEOMETRYCALCULATOR_H

#include "GeometryCalculator.h"

#include <map>

class StdGeometryCalculator : public GeometryCalculator
{
  public:
    /**
     * Constructs an instance of StdGeometryCalculator.
     * By default, EHSS, PA and TM are calculated.
     */
    StdGeometryCalculator();
    /**
     * Releases all allocated resources.
     */
    virtual ~StdGeometryCalculator();

    /**
     * \return true if EHSS will be calculated, false otherwise.
     */
    bool willEHSSBeCalculated() const {return m_EHSSWillBeCalculated;}

    /**
     * \return true if PA will be calculated, false otherwise.
     */
    bool willPABeCalculated() const {return m_PAWillBeCalculated;}

    /**
     * \return true if TM will be calculated, false otherwise.
     */
    bool willTMBeCalculated() const {return m_TMWillBeCalculated;}

    /**
     * Indicates if calculations are finished for the molecule.
     * \param mol the molecule.
     * \return true if calculations are finished for the molecule, false otherwise.
     */
    bool areCalculationsFinished(Molecule* mol) const;

    /**
     * Returns the results of CCS calculation of a molecule.
     * \param mol the molecule.
     * \return the results for the molecule.
     */
    Result* getResults(Molecule* mol) const;

    /**
     * Returns the values used for the calculations.
     * \return the values used for the calculations.
     */
    struct CalculationValues getCalculationValues() const {
      return m_calculationValues;
    }

    /**
     * Forces the GeometryCalculator to save the calculation values from
     * GlobalParameters at this moment.
     */
    void saveCalculationValues();

    /**
     * Indicates if yes or no, EHSS should be calculated.
     * \param b true if EHSS should be calculated, else otherwise.
     */
    void shouldEHSSBeCalculated(bool b) {m_EHSSWillBeCalculated = b;}

    /**
     * Indicates if yes or no, PA should be calculated.
     * \param b true if PA should be calculated, else otherwise.
     */
    void shouldPABeCalculated(bool b) {m_PAWillBeCalculated = b;}

    /**
     * Indicates if yes or no, TM should be calculated.
     * \param b true if TM should be calculated, else otherwise.
     */
    void shouldTMBeCalculated(bool b) {m_TMWillBeCalculated = b;}

    /**
     * Sets a vector of molecules (geometries) for CCS calculation.
     * \param geometries a vector of geometries.
     */
    void setGeometries(std::vector<Molecule*>* geometries);

    /**
     * Launches all the calculations, on all geometries.
     */
    void launchCalculations();

  private:
    /**
     * The geometries on which calculations will be done.
     */
    std::vector<Molecule*>* m_geometries;

    /**
     * A map containing the different results for all geometries, when
     * calculations are over.
     */
    std::map<Molecule*, Result*> m_results;

    /**
     * A map indicating if calculations are over, for all geometries.
     */
    std::map<Molecule*, bool> m_calculationsState;

    /**
     * A boolean indicating if EHSS should be calculated.
     */
    bool m_EHSSWillBeCalculated;

    /**
     * A boolean indicating if PA should be calculated.
     */
    bool m_PAWillBeCalculated;

    /**
     * A boolean indicating if TM should be calculated.
     */
    bool m_TMWillBeCalculated;

    /**
     * Values used in calculations.
     */
    CalculationValues m_calculationValues;
};

#endif
