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

#include "StdGeometryCalculator.h"

#include "GlobalParameters.h"
#include "SystemParameters.h"

#include "../math/CalculationOperator.h"
#include "../math/MonoThreadCalculationOperator.h"
#include "../math/MultiThreadCalculationOperator.h"
#include "../observer/state/CalculationState.h"

#include <string>


StdGeometryCalculator::StdGeometryCalculator()
  : m_geometries(nullptr), m_EHSSWillBeCalculated(true), m_PAWillBeCalculated(true), m_TMWillBeCalculated(true)
{
  saveCalculationValues();
}

StdGeometryCalculator::~StdGeometryCalculator()
{
  // On détruit tous les résultats.
  for (auto it = m_results.begin(); it != m_results.end(); ++it) {
    delete it->second;
  }
}

bool StdGeometryCalculator::areCalculationsFinished(Molecule* mol) const
{
  if (mol == nullptr) {
    throw std::string("Can't test calculation ending for a null pointer to molecule.");
  }

  // On teste si la molécule est dans la vectore des molécules à calculer.
  auto state = m_calculationsState.find(mol);
  if (state == m_calculationsState.end()) {
    // Géométrie non trouvée.
    throw std::string("Can't test calculation ending for a geometry not save for calculation.");
  }

  return state->second;
}

Result* StdGeometryCalculator::getResults(Molecule* mol) const
{
  if (mol == nullptr) {
    throw std::string("Can't obtain results for a null pointer to molecule.");
  }

  if (!areCalculationsFinished(mol)) {
    // L'appel a déjà testé si la molécule était dans la vectore des
    // géométries à calculer.
    throw std::string("Can't obtain results of a non-finished calculation.");
  }

  return m_results.find(mol)->second;
}

void StdGeometryCalculator::saveCalculationValues()
{
  m_calculationValues.temperature = GlobalParameters::getInstance()->getTemperature();
  m_calculationValues.potentialEnergyStart = GlobalParameters::getInstance()->getPotentialEnergyStart();
  m_calculationValues.timeStepStart = GlobalParameters::getInstance()->getTimeStepStart();
  m_calculationValues.potentialEnergyCloseCollision = GlobalParameters::getInstance()->getPotentialEnergyCloseCollision();
  m_calculationValues.timeStepCloseCollision = GlobalParameters::getInstance()->getTimeStepCloseCollision();
  m_calculationValues.numberCyclesTM = GlobalParameters::getInstance()->getNumberCompleteCycles();
  m_calculationValues.numberPointsVelocity = GlobalParameters::getInstance()->getNumberVelocityPoints();
  m_calculationValues.numberPointsMCIntegrationTM = GlobalParameters::getInstance()->getNbPointsMCIntegrationTM();
  m_calculationValues.energyConservationThreshold = GlobalParameters::getInstance()->getEnergyConservationThreshold();
  m_calculationValues.numberPointsMCIntegrationEHSSPA = GlobalParameters::getInstance()->getNbPointsMCIntegrationEHSSPA();
}

void StdGeometryCalculator::setGeometries(std::vector<Molecule*>* geometries)
{
  if (geometries == nullptr) {
    throw std::string("Can't save a null vector of geometries.");
  }

  // On vide la map des états.
  m_calculationsState.clear();
  // On vide la map des résultats, après avoir détruit tous les résultats.
  for (auto it = m_results.begin(); it != m_results.end(); ++it) {
    delete it->second;
  }
  m_results.clear();

  m_geometries = geometries;
  // On rempli la vectore des états de calcul pour les géométries.
  for (auto it = m_geometries->begin(); it != m_geometries->end(); ++it) {
    m_calculationsState.insert(std::pair<Molecule*, bool>(*it, false));
  }
}

void StdGeometryCalculator::launchCalculations()
{
  if (m_geometries == nullptr) {
      throw std::string("Can't calculate on null vector of geometries.");
  }

  unsigned int maxNumberOfThreads = SystemParameters::getInstance()->getMaximalNumberThreads();

  // Pour toutes les géométries.
  for (auto it = m_geometries->begin(); it != m_geometries->end(); ++it) {
    // On a un besoin d'un nouveau calculateur.
    CalculationOperator* calculator;

    // Pour le pattern Observer.
    CalculationState* calculationState = new CalculationState(*it,
                                          m_calculationValues.numberCyclesTM *
                                          m_calculationValues.numberPointsVelocity *
                                          m_calculationValues.numberPointsMCIntegrationTM);
    // On ajoute tous les observeurs.
    std::for_each(m_obsList.begin(), m_obsList.end(), [&](Observer* obs){ calculationState->addObserver(obs); });

    if (maxNumberOfThreads <= 1) {
      // 0 ou 1 thread -> MonoThread.
      calculator =
      new MonoThreadCalculationOperator(calculationState,
                                        *it,
                                        m_calculationValues.temperature,
                                        m_calculationValues.potentialEnergyStart,
                                        m_calculationValues.timeStepStart,
                                        m_calculationValues.potentialEnergyCloseCollision,
                                        m_calculationValues.timeStepCloseCollision,
                                        m_calculationValues.numberCyclesTM,
                                        m_calculationValues.numberPointsVelocity,
                                        m_calculationValues.numberPointsMCIntegrationTM,
                                        m_calculationValues.energyConservationThreshold,
                                        m_calculationValues.numberPointsMCIntegrationEHSSPA);
    } else {
      // Plus d'un thread -> MultiThread
      calculator =
      new MultiThreadCalculationOperator(calculationState,
                                         *it,
                                         maxNumberOfThreads,
                                         m_calculationValues.temperature,
                                         m_calculationValues.potentialEnergyStart,
                                         m_calculationValues.timeStepStart,
                                         m_calculationValues.potentialEnergyCloseCollision,
                                         m_calculationValues.timeStepCloseCollision,
                                         m_calculationValues.numberCyclesTM,
                                         m_calculationValues.numberPointsVelocity,
                                         m_calculationValues.numberPointsMCIntegrationTM,
                                         m_calculationValues.energyConservationThreshold,
                                         m_calculationValues.numberPointsMCIntegrationEHSSPA);
    }

    // Si on doit calculer EHSS ou PA, on se lance.
    if (willEHSSBeCalculated() || willPABeCalculated()) {
      calculator->runEHSSAndPA();
    }
    // Si on doit calculer TM, go aussi !
    if (willTMBeCalculated()) {
      calculator->runTM();
    }

    calculator->getResults()->EHSSNeedsToBePrinted(willEHSSBeCalculated());
    calculator->getResults()->PANeedsToBePrinted(willPABeCalculated());
    calculator->getResults()->TMNeedsToBePrinted(willTMBeCalculated());

    // Calcul terminé, enregistrement des résultats et passage du booléen
    // de l'état à true.
    m_calculationsState[*it] = true;
    m_results.insert(std::pair<Molecule*, Result*>(*it, calculator->getResults()));

    //delete calculator->getCalculationState();
    delete calculator;
  }
}
