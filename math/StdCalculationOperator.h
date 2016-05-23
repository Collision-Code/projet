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
 * \file StdCalculationOperator.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements methods which will launch calculations on EHSS, PA and TM methods.
 */

#ifndef STDCALCULATIONOPERATOR_H
#define STDCALCULATIONOPERATOR_H

#include "CalculationOperator.h"

#include "../molecule/Molecule.h"
#include "Vector3D.h"

#include <array>
#include <vector>


class StdCalculationOperator : public CalculationOperator
{
  public:
    /**
     * StdCalculationOperator's constructor.
     * \param mol the molecule to work with.
     */
    StdCalculationOperator(CalculationState* calculationState,
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
    virtual ~StdCalculationOperator();

    /**
     * Returns the results.
     * \return a pointer to the results of calculations.
     * Pointer is destroy when the instance of StdCalculationOperator is destroyed.
     */
    Result* getResults() {
      return m_result;
    }

    /**
     * \return the calculation state associated with this calculation operator.
     */
    CalculationState* getCalculationState() const {
      return m_calculationState;
    }

    /**
     * Launches the calculation of EHSS and PA.
     */
    void runEHSSAndPA();

    /**
     * Launches the calculation of TM.
     */
    void runTM();


  protected:
    // EHSS et PA
    /**
     * Calculates EHSS and PA and put the results in
     * m_result attribute.
     */
    void calculateEHSSAndPA(Molecule* mol);

    /**
     * Guides hard sphere scattering trajectory.
     */
    void che(Molecule* mol, int refl, double& halfCos, double cop, double& yRand, double& zRand, bool& kp, Vector3D& initialIncidenceVector);

  protected:
    /**
     * The state to update during calculations to notify the views.
     */
    CalculationState* m_calculationState;


  protected:
    // TM
    /**
     * Calculates TM and put the results in
     * m_result attribute.
     */
    virtual void calculateTM() = 0;

    /**
     * Calculates the structural asymmetry parameter and
     * puts it in m_asymmetryParameter.
     */
    void calculateAsymmetryParameter();

    /**
     * Calculates the potential and the derivates of the potential.
     * The potential is given by a sum of 6-12 two body ...
     * \param p the position for the calculation
     * \param dPot the derivates of the potential
     * \return the potential
     */
    double calculatePotentials(std::vector<Vector3D>& molPos, const Vector3D& p, Vector3D& dPot, double& dMax);

    /**
     * Calculates a trajectory.
     * \return angle of deviation
     */
    double calculateTrajectory(std::vector<Vector3D>& molPos, double v, double b);

    /**
     * Defines Hamilton's equations of motion ad the time derivates of
     * the coordinates and momenta.
     * \return the potential
     */
    double calculateHamilton(std::vector<Vector3D>& molPos, std::array<double, 6>& w, std::array<double, 6>& dw, double& dMax);

    /**
     * Integration method. Uses 5th order Runge-Kutta-Gill to initiate and 5th order
     * Adams-Moulton predictor-corrector to propagate.
     * \return the potential
     */
    double calculateRKandAM(std::vector<Vector3D>& molPos, int& l, double& tim, double& dt, std::array<double, 6>& w, std::array<double, 6>& dw, std::array<std::array<double, 6>, 6>& arrayDouble, double& dMax, double& hVar, double& hcVar);



  protected:
    // EHSS, PA et TM
    /**
     * Maximum of successive reflections followed.
     */
    static const int m_MaxSuccRefl;



  protected:
    // TM
    /**
     * Constant for ion-induced dipole potential.
     */
    static const double m_IonInducedDipolePotential;

    /**
     * xe from Mobcal.
     */
    static const double m_XeFromMobcal;

    /**
     * xk from Mobcal.
     */
    static const double m_XkFromMobcal;

    /**
     * xmv from Mobcal.
     */
    static const double m_XmvFromMobcal;

    /**
     * Lennard-Jones scaling parameter.
     */
    static const double m_EoFromMobcal;

    /**
     * Lennard-Jones scaling parameter.
     */
    static const double m_RoFromMobcal;

    /**
     * Number of integration steps before the program tests to see if
     * the trajectory is done or lost.
     */
    static const int m_NbIntegrationStep;

    /**
     * Determines the maximum impact parameter at each velocity.
     */
    static const double m_MaxImpactParameter;



  protected:
    /**
     * The molecule to work with.
     */
    Molecule* m_mol;

    /**
     * The pointer where the results will be put.
     */
    Result* m_result;

    /**
     * RHS values.
     */
    std::vector<double> m_rhsTab;

    /**
     * The temperature.
     */
    double m_temperature;

    // itn
    /**
     * Number of complete cycles for TM calculation.
     */
    int m_numberCyclesTM;

    // inp
    /**
     * Number of points in velocity integration.
     */
    int m_numberPointsVelocity;

    // imp
    /**
     * Number of points in Monte-Carlo integrations of impact
     * parameter and orientation.
     */
    int m_numberPointsMCIntegrationTM;

    /**
     * Number of points in Monte-Carlo integration in EHSS/PA methods.
     */
    int m_numberPointsMCIntegrationEHSSPA;



  protected:
    // TM
    /**
     * EOLJ for Helium values.
     */
    std::vector<double> m_EOLJTab;

    /**
     * ROLJ for Helium values.
     */
    std::vector<double> m_ROLJTab;

    /**
     * ROLJ maximum.
     */
    double m_maxROLJ;

    /**
     * Structural asymmetry parameter.
     */
    double m_asymmetryParameter;

    /**
     * Mass constant (mu in Mobcal).
     */
    double m_massConstant;

    /**
     * Mobility constant (mconst in Mobcal).
     */
    double m_mobilityConstant;

    /**
     * Potential energy where the trajectory starts.
     */
    double m_potentialEnergyStart;

    /**
     * Time step at the start of the trajectory.
     */
    double m_timeStepStart;

    /**
     * Potential energy where the trajectory comes close to a collision.
     */
    double m_potentialEnergyCloseCollision;

    /**
     * Time step when the trajectory comes close to a collision.
     */
    double m_timeStepCloseCollision;

    /**
     * Energy conservation threshold.
     */
    double m_energyConservationThreshold;

    /**
     * Initial positions of the atoms of the molecule. For calculations.
     */
    std::vector<Vector3D> m_molInitPos;

    /**
     * Positions of the atoms of the molecule. For calculations.
     */
    std::vector<Vector3D> m_molPos;

    /**
     * Number of atoms in the molecule. For calculations.
     */
    unsigned int m_molNbAtoms;

    /**
     * Charges of each atoms of the molecule. For calculations.
     */
    std::vector<double> m_molChg;

    /**
     * Mass of the molecule. For calculations.
     */
    double m_molMass;
};

#endif
