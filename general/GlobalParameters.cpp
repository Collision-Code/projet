#include "GlobalParameters.h"

// Global instance.
GlobalParameters* GlobalParameters::m_instance = new GlobalParameters();


GlobalParameters::GlobalParameters()
  : m_temperature(298.0), m_potentialEnergyStart(0.00005),
  m_timeStepStart(0.5), m_potentialEnergyCloseCollision(0.0025),
  m_timeStepCloseCollision(0.05), m_nbCompleteCycles(10),
  m_nbVelocityPoints(40), m_nbPointsMCIntegrationTM(25),
  m_nbPointsMCIntegrationEHSSPA(250000), m_energyConservationThreshold(99.0)
{
}

GlobalParameters::~GlobalParameters()
{
}
