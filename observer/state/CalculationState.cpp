#include "CalculationState.h"

#include <cmath>

CalculationState::CalculationState(Molecule* molecule, int totalTrajectories)
  : m_molecule(molecule), m_finishedTMTrajectories(0),
  m_totalTMTrajectories(totalTrajectories), m_hasEHSSStarted(false),
  m_hasEHSSEnded(false), m_hasPAStarted(false), m_hasPAEnded(false),
  m_hasTMStarted(false), m_hasTMEnded(false)
{

}

CalculationState::~CalculationState()
{

}

void CalculationState::setFinishedTrajectories(int n)
{
  int oldPercentage = std::floor(getPercentageFinishedTrajectories());
  m_finishedTMTrajectories = n;
  int newPercentage = std::floor(getPercentageFinishedTrajectories());

  // On ne notifie que si le pourcentage a augmente d'au moins 1%.
  if (newPercentage != oldPercentage) {
    notifyObservers(ObservableEvent::TRAJECTORY_NUMBER_UPDATE);
  }
}
