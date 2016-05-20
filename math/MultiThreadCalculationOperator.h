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
