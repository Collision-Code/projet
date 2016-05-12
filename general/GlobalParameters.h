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
 * \file GlobalParameters.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 08 may 2016
 * \brief Class implementing a singleton to access global parameters.
 */

#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

class GlobalParameters
{
  public:
    /**
     * \return an instance of GlobalParameters to work with.
     */
    static GlobalParameters* getInstance() {
      return m_instance;
    }

    /**
     * Destroys all allocated resources.
     */
    virtual ~GlobalParameters();

    /**
     * Returns the temperature.
     * \return the temperature.
     */
    double getTemperature() const {
      return m_temperature;
    }

    /**
     * Returns the potential energy at the start of a trajectory.
     * \return the potential energy at the start of a trajectory.
     */
     // sw1 in Mobcal
    double getPotentialEnergyStart() const {
      return m_potentialEnergyStart;
    }

    /**
     * Returns the time step at the start of a trajectory.
     * \return the time step at the start of a trajectory.
     */
    // dtsf1 in Mobcal.
    double getTimeStepStart() const {
      return m_timeStepStart;
    }

    /**
     * Returns the potential energy when close to a collision.
     * \return the potential energy when close to a collision.
     */
     // sw2 in Mobcal
    double getPotentialEnergyCloseCollision() const {
      return m_potentialEnergyCloseCollision;
    }

    /**
     * Returns the time step when close to a collision.
     * \return the time step when close to a collision.
     */
    // dtsf2 in Mobcal.
    double getTimeStepCloseCollision() const {
      return m_timeStepCloseCollision;
    }

    /**
     * Returns the number of complete cycles for TM method.
     * \return the number of complete cycles for TM method.
     */
    // itn in Mobcal.
    int getNumberCompleteCycles() const {
      return m_nbCompleteCycles;
    }

    /**
     * Returns the number of points in velocity integration.
     * \return the number of points in velocity integration.
     */
    // inp in Mobcal.
    int getNumberVelocityPoints() const {
      return m_nbVelocityPoints;
    }

    /**
     * Returns the number of points in Monte-Carlo integrations of impact
     * parameter and orientation for TM method.
     * \return the number of points in Monte-Carlo integrations of impact
     * parameter and orientation for TM method.
     */
    // imp in Mobcal.
    int getNbPointsMCIntegrationTM() const {
      return m_nbPointsMCIntegrationTM;
    }

    /**
     * Returns the number of points in Monte-Carlo integrations for EHSS and PA methods.
     * \return the number of points in Monte-Carlo integrations for EHSS and PA methods.
     */
    int getNbPointsMCIntegrationEHSSPA() const {
      return m_nbPointsMCIntegrationEHSSPA;
    }

    /**
     * Returns the energy conservation threshold, in percent.
     * \return the energy conservation threshold, in percent.
     */
    double getEnergyConservationThreshold() const {
      return m_energyConservationThreshold;
    }

    /**
     * Sets the temperature to t.
     * \param t the new temperature.
     */
    void setTemperature(double t) {
      m_temperature = t;
    }

    /**
     * Sets the potential energy at the start of a trajectory to pES.
     * \param pES the new potential energy at the start of a trajectory.
     */
     // sw1 in Mobcal
    void setPotentialEnergyStart(double pES) {
      m_potentialEnergyStart = pES;
    }

    /**
     * Sets the time step at the start of a trajectory to dt.
     * \param dt the new time step at the start of a trajectory.
     */
    // dtsf1 in Mobcal.
    void setTimeStepStart(double dt) {
      m_timeStepStart = dt;
    }

    /**
     * Sets the potential energy when close to a collision to pECC.
     * \param pECC the new potential energy when close to a collision.
     */
     // sw2 in Mobcal
    void setPotentialEnergyCloseCollision(double pECC) {
      m_potentialEnergyCloseCollision = pECC;
    }

    /**
     * Sets the time step when close to a collision to dt.
     * \param dt the new time step when close to a collision.
     */
    // dtsf2 in Mobcal.
    void setTimeStepCloseCollision(double dt) {
      m_timeStepCloseCollision = dt;
    }

    /**
     * Sets the number of complete cycles for TM method to n.
     * \param n the new number of complete cycles for TM method.
     */
    // itn in Mobcal.
    void setNumberCompleteCycles(int n) {
      m_nbCompleteCycles = n;
    }

    /**
     * Sets the number of points in velocity integration to n.
     * \param n the new number of points in velocity integration.
     */
    // inp in Mobcal.
    void setNumberVelocityPoints(int n) {
      m_nbVelocityPoints = n;
    }

    /**
     * Sets the number of points in Monte-Carlo integrations of impact
     * parameter and orientation for TM method to n.
     * \param n the new number of points in Monte-Carlo integrations of impact
     * parameter and orientation for TM method.
     */
    // imp in Mobcal.
    void setNbPointsMCIntegrationTM(int n) {
      m_nbPointsMCIntegrationTM = n;
    }

    /**
     * Sets the number of points in Monte-Carlo integrations for EHSS and PA methods to n.
     * \param n the new number of points in Monte-Carlo integrations for EHSS and PA methods.
     */
    void setNbPointsMCIntegrationEHSSPA(int n) {
      m_nbPointsMCIntegrationEHSSPA = n;
    }

    /**
     * Sets the energy conservation threshold, in percent, to eCT.
     * \param eCT the new energy conservation threshold, in percent.
     */
    void setEnergyConservationThreshold(double eCT) {
      m_energyConservationThreshold = eCT;
    }


  private:
    /**
     * Constructs a new GlobalParameters instance.
     */
    GlobalParameters();

  private:
    /**
     * Static instance of GlobalParameters to work with.
     */
    static GlobalParameters* m_instance;

  private:
    /**
     * Temperature.
     * Default value : 298.0.
     */
    double m_temperature;

    /**
     * Potential energy at the start of a trajectory.
     * Default value : 0.00005.
     */
     // sw1 in Mobcal
    double m_potentialEnergyStart;

    /**
     * Time step at the start of a trajectory.
     * Default value : 0.5.
     */
    // dtsf1 in Mobcal.
    double m_timeStepStart;

    /**
     * Potential energy when close to a collision.
     * Default value : 0.0025.
     */
     // sw2 in Mobcal
    double m_potentialEnergyCloseCollision;

    /**
     * Time step when close to a collision.
     * Default value : 0.05.
     */
    // dtsf2 in Mobcal.
    double m_timeStepCloseCollision;

    /**
     * Number of complete cycles for TM method.
     * Default value : 10.
     */
    // itn in Mobcal.
    int m_nbCompleteCycles;

    /**
     * Number of points in velocity integration.
     * Default value : 40.
     */
    // inp in Mobcal.
    int m_nbVelocityPoints;

    /**
     * Number of points in Monte-Carlo integrations of impact
     * parameter and orientation for TM method.
     * Default value : 25.
     */
    // imp in Mobcal.
    int m_nbPointsMCIntegrationTM;

    /**
     * Number of points in Monte-Carlo integrations for EHSS and PA methods.
     * Default value : 250000.
     */
    int m_nbPointsMCIntegrationEHSSPA;

    /**
     * Energy conservation threshold, in percent.
     * Default value : 99%.
     */
    double m_energyConservationThreshold;
};

#endif
