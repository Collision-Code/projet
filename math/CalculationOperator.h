#ifndef CALCULATIONOPERATOR_H
#define CALCULATIONOPERATOR_H

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

#include "Result.h"

#include "../observer/state/CalculationState.h"

/**
 * Interface describing how to run cross-section calculations.
 */
class CalculationOperator {
  public:
    virtual ~CalculationOperator() {};

    /**
     * Returns the results.
     * \return a pointer to the results of calculations.
     */
    virtual Result* getResults() = 0;

    /**
     * \return the calculation state associated with this calculation operator.
     */
    virtual CalculationState* getCalculationState() const = 0;

    /**
     * Launches the calculation of EHSS and PA.
     */
    virtual void runEHSSAndPA() = 0;

    /**
     * Launches the calculation of TM.
     */
    virtual void runTM() = 0;
};

#endif // CALCULATIONOPERATOR_H
