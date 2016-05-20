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
 * \file CalculationState.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 14 may 2016
 * \brief Describe a state of a calculation by a CalculationOperator instance.
 * \details Allow access to data like finished methods booleans or percentage of progression for TM method.
 */

#ifndef CALCULATIONSTATE_H
#define CALCULATIONSTATE_H

#include "../Observable.h"
#include "../../molecule/Molecule.h"


class CalculationState : public Observable
{
  public:
    /**
     * Constructor.
     * \param molecule the molecule on which the calculations are proceeded.
     * \param totalTrajectories the total number of trajectories in TM calculation.
     */
    CalculationState(Molecule* molecule, int totalTrajectories);

    /**
     * Destructor.
     */
    virtual ~CalculationState();

    /**
     * \return the molecule on which the calculations are proceeded.
     */
    Molecule* getMolecule() const {
      return m_molecule;
    }

    /**
     * \return the percentage of trajectories finished.
     */
    double getPercentageFinishedTrajectories() const {
      return ((double) m_finishedTMTrajectories / (double) m_totalTMTrajectories) * 100.0;
    }

    /**
     * \return the number of finished trajectories.
     */
    int getNumberFinishedTractories() const {
      return m_finishedTMTrajectories;
    }

    /**
     * \return the total number of trajectories to calculate.
     */
    int getNumberTotalTractories() const {
      return m_totalTMTrajectories;
    }

    /**
     * \return true if EHSS calculations have started, false otherwise.
     */
    bool hasEHSSStarted() const {
      return m_hasEHSSStarted;
    }

    /**
     * \return true if EHSS calculations have ended, false otherwise.
     */
    bool hasEHSSEnded() const {
      return m_hasEHSSEnded;
    }

    /**
     * \return true if PA calculations have started, false otherwise.
     */
    bool hasPAStarted() const {
      return m_hasPAStarted;
    }

    /**
     * \return true if PA calculations have ended, false otherwise.
     */
    bool hasPAEnded() const {
      return m_hasPAEnded;
    }

    /**
     * \return true if TM calculations have started, false otherwise.
     */
    bool hasTMStarted() const {
      return m_hasTMStarted;
    }

    /**
     * \return true if TM calculations have ended, false otherwise.
     */
    bool hasTMEnded() const {
      return m_hasTMEnded;
    }

    /**
     * Sets the number of trajectories finished to n.
     * \param n the number of trajectories finished by the TM calculations.
     */
    void setFinishedTrajectories(int n);

    /**
     * Indicates that EHSS calculations have started.
     */
    void setEHSSStarted() {
      m_hasEHSSStarted = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::EHSS_STARTED);
    }

    /**
     * Indicates that EHSS calculations have ended.
     */
    void setEHSSEnded() {
      m_hasEHSSEnded = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::EHSS_ENDED);
    }

    /**
     * Indicates that EHSS calculations have started.
     */
    void setPAStarted() {
      m_hasPAStarted = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::PA_STARTED);
    }

    /**
     * Indicates that EHSS calculations have ended.
     */
    void setPAEnded() {
      m_hasPAEnded = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::PA_ENDED);
    }

    /**
     * Indicates that EHSS calculations have started.
     */
    void setTMStarted() {
      m_hasTMStarted = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::TM_STARTED);
    }

    /**
     * Indicates that EHSS calculations have ended.
     */
    void setTMEnded() {
      m_hasTMEnded = true;
      // Notification des observateurs.
      notifyObservers(ObservableEvent::TM_ENDED);
    }

    /**
     * Sets the EHSS result.
     * \param r the EHSS result.
     */
    void setEHSSResult(double r) {
      m_EHSSResult = r;
    }

    /**
     * \return the EHSS result.
     */
    double getEHSSResult() const {
      return m_EHSSResult;
    }

    /**
     * Sets the PA result.
     * \param r the PA result.
     */
    void setPAResult(double r) {
      m_PAResult = r;
    }

    /**
     * \return the PA result.
     */
    double getPAResult() const {
      return m_PAResult;
    }

    /**
     * Sets the TM result.
     * \param r the TM result.
     */
    void setTMResult(double r) {
      m_TMResult = r;
    }

    /**
     * \return the TM result.
     */
    double getTMResult() const {
      return m_TMResult;
    }

    void oneCalculationFinished() {
      notifyObservers(ObservableEvent::ONE_CALCULATION_FINISHED);
    }

  private:
    /**
     * The molecule on which the calculations are proceeded.
     */
    Molecule* m_molecule;

    /**
     * The number of trajectories finished in TM calculation.
     */
    int m_finishedTMTrajectories;

    /**
     * The total number of trajectories which will be calculated with TM method.
     */
    const int m_totalTMTrajectories;

    /**
     * Indicates if EHSS calculation has started.
     */
    bool m_hasEHSSStarted;

    /**
     * Indicates if EHSS calculation has ended.
     */
    bool m_hasEHSSEnded;

    /**
     * Indicates if PA calculation has started.
     */
    bool m_hasPAStarted;

    /**
     * Indicates if PA calculation has ended.
     */
    bool m_hasPAEnded;

    /**
     * Indicates if TM calculation has started.
     */
    bool m_hasTMStarted;

    /**
     * Indicates if TM calculation has ended.
     */
    bool m_hasTMEnded;

    /**
     * The EHSS result.
     */
    double m_EHSSResult;

    /**
     * The PA result.
     */
    double m_PAResult;

    /**
     * The TM result.
     */
    double m_TMResult;
};

#endif
