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

#include "StdCmdView.h"

#include "GlobalParameters.h"
#include "StdGeometryCalculator.h"
#include "../reader/StdExtractResources.h"
#include "../reader/ChargesReader.h"
#include "../reader/ChgChargesReader.h"
#include "../molecule/Molecule.h"
#include "../writer/FileWriter.h"
#include "../writer/StdFileWriter.h"
#include "../observer/Event.h"
#include "../math/Mean.h"
#include "../math/StdMean.h"

#include <sstream>
#include <fstream>
#include <iostream>

StdCmdView::StdCmdView()
{
  m_calculator = new StdGeometryCalculator();
}

StdCmdView::~StdCmdView()
{
  delete m_calculator;
}

void StdCmdView::addInputFile(std::string fileName)
{
  // On cherche d'abord si le fichier n'est pas dans la vectore.
  for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); ++it) {
    if (*it == fileName) {
      // Il y est, on s'en va.
      return;
    }
  }

  // Le fichier n'est pas dans la vectore, on l'ajoute.
  m_inputFiles.push_back(fileName);
}

void StdCmdView::removeInputFile(std::string fileName)
{
  // On cherche si le fichier est dans la vectore.
  for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); ++it) {
    if (*it == fileName) {
      // Il y est, on le supprime.
      m_inputFiles.erase(it);
      return;
    }
  }
}

int StdCmdView::loadInputFiles() {
  // Si on n'a pas de fichier en entrée, c'est une erreur.
  if (m_inputFiles.size() == 0) {
    throw std::string("There is no input file.");
  }

  // Va aider a lire les fichiers d'input.
  ExtractResources* reader = new StdExtractResources();

  // On vide le vector des geometries.
  m_geometries.clear();
  // On vide la map d'association.
  m_geometriesFromFile.clear();

  // Pour tous les fichiers d'input, on va charger les géométries,
  // puis les ajouter au vector.
  for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); ++it) {
    std::vector<Molecule*>* l = reader->getGeometriesFromFile(*it);

    // Si on a une valeur nulle, c'est que le fichier ne peut pas être
    // chargé.
    if (l == nullptr) {
      std::ostringstream oss;
      oss << "Impossible to load file " << *it << ".";
      throw oss.str();
    }
    // Sinon, on ajoute toutes les géométries chargées.
    for (auto it2 = l->begin(); it2 != l->end(); ++it2) {
      m_geometries.push_back(*it2);
      // On pense à les ajouter dans la map aussi, avec leur fichier lié.
      m_geometriesFromFile.insert(std::pair<Molecule*, std::string>(*it2, *it));
    }
    delete l;
  }

  // On notifie les observateurs pour les geometries chargees.
  notifyObservers(ObservableEvent::GEOMETRIES_LOADED);

  delete reader;

  // On vérifie si pn doit charger des charges.
  if (m_chargeFile != "") {
    ChargesReader* cR = new ChgChargesReader(m_chargeFile);
    m_geometries = *(cR->loadResources(&m_geometries));
    delete cR;

    // On notifie les observateurs pour le fichier de charges.
    notifyObservers(ObservableEvent::CHARGES_LOADED);
  }

  return m_geometries.size();
}

void doLines(std::ostringstream& oStream, bool EHSS, bool PA, bool TM) {
  oStream << " -----";
  if (EHSS) {
    oStream << "-----------";
  }
  if (EHSS && PA) {
    oStream << "-";
  }
  if (PA) {
    oStream << "-----------";
  }
  if ((EHSS || PA) && TM) {
    oStream << "-";
  }
  if (TM) {
    oStream << "---------------------------------------------------------------";
  }
  oStream << "---" << std::endl;
}

void doEntete(std::ostringstream& oStream, bool EHSS, bool PA, bool TM) {
  doLines(oStream, EHSS, PA, TM);

  oStream << "|   N° ";
  if (EHSS) {
    oStream << "\t|   EHS CS";
  }
  if (PA) {
    oStream << "\t|   PA CS";
  }
  if (TM) {
    oStream << "\t|   TM CS" << "\t| Struct Asym Param" << "\t|  Std dev (%)" << "\t|  Failed traj";
  }
  oStream << "\t|" << std::endl;

  doLines(oStream, EHSS, PA, TM);
}

std::string StdCmdView::getResultFormat() const {
  // Les calculs sont finis, on les enregistre dans le fichier output.
  std::ostringstream oStream;
  FileWriter* fileWriter = new StdFileWriter(oStream);

  // Variables globales.
  oStream << "****************" << std::endl;
  oStream << "GLOBAL VARIABLES" << std::endl;
  oStream << "****************" << std::endl;
  GeometryCalculator::CalculationValues calculationValues = m_calculator->getCalculationValues();
  oStream << "Temperature = " << calculationValues.temperature << std::endl;
  if (m_calculator->willEHSSBeCalculated() || m_calculator->willPABeCalculated()) {
    oStream << "Number of Monte-Carlo trajectories in EHSS/PA methods = " << GlobalParameters::getInstance()->getNbPointsMCIntegrationEHSSPA() << std::endl;
  }
  if (m_calculator->willTMBeCalculated()) {
    oStream << "**" << std::endl;
    oStream << "Potential energy at start (sw1) = " << calculationValues.potentialEnergyStart << std::endl;
    oStream << "Potential energy when close to a collision (sw2) = " << calculationValues.potentialEnergyCloseCollision << std::endl;
    oStream << "Time step at start (dtsf1) = " << calculationValues.timeStepStart << std::endl;
    oStream << "Time step when close to a collision (dtsf2) = " << calculationValues.timeStepCloseCollision << std::endl;
    oStream << "Energy conservation threshold = " << calculationValues.energyConservationThreshold << "%" << std::endl;
    oStream << "**" << std::endl;
    oStream << "Number of complete cycles for TM method (itn) = " << calculationValues.numberCyclesTM << std::endl;
    oStream << "Number of points in velocity integration (inp) = " << calculationValues.numberPointsVelocity << std::endl;
    oStream << "Number of points in Monte-Carlo integrations for TM method (imp) = " << calculationValues.numberPointsMCIntegrationTM << std::endl;
    oStream << "Total number of points = " << calculationValues.numberCyclesTM * calculationValues.numberPointsVelocity * calculationValues.numberPointsMCIntegrationTM << std::endl;
  }
  oStream << std::endl << std::endl;
  oStream << "*******" << std::endl;
  oStream << "RESULTS" << std::endl;
  oStream << "*******";

  // Entete des colonnes.
  std::string lastFile = "";
  Mean* mean = new StdMean();

  // Resultats des calculs : lignes.
  int num = 1;
  for (auto it = m_geometries.begin(); it != m_geometries.end(); ++it) {
    Result* result = m_calculator->getResults(*it);

    std::string file = m_geometriesFromFile.find(*it)->second;
    if (file != lastFile) {
      oStream << std::endl;
      oStream << "File : " << file << std::endl;
      doEntete(oStream, m_calculator->willEHSSBeCalculated(), m_calculator->willPABeCalculated(), m_calculator->willTMBeCalculated());
      lastFile = file;
    } else {
      doLines(oStream, m_calculator->willEHSSBeCalculated(), m_calculator->willPABeCalculated(), m_calculator->willTMBeCalculated());
    }

    // D'abord, numéro de la géométrie associé au fichier d'où elle vient.
    oStream << "|\t" << num;
    mean->addResult(result);
    result->accept(*fileWriter);
    oStream << std::endl;
    ++num;
  }
  doLines(oStream, m_calculator->willEHSSBeCalculated(), m_calculator->willPABeCalculated(), m_calculator->willTMBeCalculated());
  oStream << "|  Mean";
  mean->accept(*fileWriter);
  oStream << std::endl;
  doLines(oStream, m_calculator->willEHSSBeCalculated(), m_calculator->willPABeCalculated(), m_calculator->willTMBeCalculated());

  delete mean;
  delete fileWriter;

  return oStream.str();
}

void StdCmdView::saveResults() {
  // Si on n'a pas de fichier de sortie, c'est une erreur.
  if (m_outputFile == "") {
    throw std::string("There is no output file.");
  }

  // Les calculs sont finis, on les enregistre dans le fichier output.
  std::ofstream oFile;
  oFile.open(m_outputFile);
  oFile << getResultFormat();
  oFile.close();

  // On notifie les observateurs.
  notifyObservers(ObservableEvent::FILE_SAVED);
}

void StdCmdView::launch()
{
  // On donne les géométries au calculateur.
  m_calculator->setGeometries(&m_geometries);

  // On sauvegarde les valeurs globales dans le calculateur.
  m_calculator->saveCalculationValues();

  // On passe les observateurs du calcul.
  m_calculator->takeObservers(m_observers);

  // On lance les calculs.
  m_calculator->launchCalculations();

  // On indique que les calculs sont termines.
  notifyObservers(ObservableEvent::CALCULATIONS_FINISHED);
}
