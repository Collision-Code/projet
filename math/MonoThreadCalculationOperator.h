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
 * \file MonoThreadCalculationOperator.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements the operations for calculating cross-section with EHSS, PA and TM methods and one thread.
 */

#ifndef MONOTHREADCALCULATIONOPERATOR_H
#define MONOTHREADCALCULATIONOPERATOR_H

#include "StdCalculationOperator.h"

#include "../molecule/Molecule.h"
#include "Vector3D.h"

#include <array>
#include <vector>


class MonoThreadCalculationOperator : public StdCalculationOperator
{
  public:
    /**
     * Constructs a mono thread CalculationOperator.
     */
    MonoThreadCalculationOperator(CalculationState* calculationState,
                                  Molecule* mol,
                                  double temperature,
                                  double potentialEnergyStart,
                                  double timeStepStart,
                                  double potentialEnergyCloseCollision,
                                  double timeStepCloseCollision,
                                  double numberCyclesTM,
                                  double numberPointsVelocity,
                                  double numberPointsMCIntegrationTM,
                                  double energyConservationThreshold,
                                  double numberPointsMCIntegrationEHSSPA);

    /**
     * Destructs allocated resources.
     */
    virtual ~MonoThreadCalculationOperator();

  protected:
    /**
     * Calculates TM and put the results in
     * m_result attribute.
     */
    void calculateTM();
};

#endif
