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
 * \file MultiThreadCalculationOperator.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements the operations for calculating cross-section with EHSS, PA and TM methods and many threads.
 */

#ifndef MULTITHREADCALCULATIONOPERATOR_H
#define MULTITHREADCALCULATIONOPERATOR_H

#include "StdCalculationOperator.h"

#include "../molecule/Molecule.h"
#include "Vector3D.h"

#include <array>
#include <vector>


class MultiThreadCalculationOperator : public StdCalculationOperator
{
  public:
    /**
     * Constructs a mono thread CalculationOperator.
     */
    MultiThreadCalculationOperator(CalculationState* calculationState,
                                   Molecule* mol,
                                   int maximalNumberThreads,
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
    virtual ~MultiThreadCalculationOperator();

  protected:
    /**
     * Calculates TM and put the results in
     * m_result attribute.
     */
    void calculateTM();

  private:
    /**
     * Used to calculate an optimized number of threads in two
     * nested loops.
     */
    struct NumberThread {
      int externLoop;
      int innerLoop;
    };

  private:
    /**
     * Calculates the optimized number of threads for two nested loops.
     */
    NumberThread getOptimizedNumberThreads(double maxThreadOnExternLoop);

  private:
    /**
     * Maximal number of threads.
     */
    unsigned int m_maximalNumberThreads;
};

#endif
