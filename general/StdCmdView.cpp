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
  // On cherche si le fichier es dans la vectore.
  for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); ++it) {
    if (*it == fileName) {
      // Il y est, on le supprime.
      m_inputFiles.erase(it);
      return;
    }
  }
}

void StdCmdView::launch()
{
  // Si on n'a pas de fichier en entrée, c'est une erreur.
  if (m_inputFiles.size() == 0) {
    throw std::string("There is no input file.");
  }
  // Si on n'a pas de fichier de sortie, c'est aussi une erreur.
  if (m_outputFile == "") {
    throw std::string("There is no output file.");
  }

  // Va aider a lire les fichiers d'input.
  ExtractResources* reader = new StdExtractResources();

  // La vectore des géométries.
  std::vector<Molecule*> geometries;
  // Une map associant les géométries avec le fichier d'où elles viennent.
  std::map<Molecule*, std::string> geometriesFromFile;
  // Pour tous les fichiers d'input, on va charger les géométries,
  // puis les ajouter à la vectore.
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
      geometries.push_back(*it2);
      // On pense à les ajouter dans la map aussi, avec leur fichier lié.
      geometriesFromFile.insert(std::pair<Molecule*, std::string>(*it2, *it));
    }
    delete l;
  }

  delete reader;

  // On vérifie si pn doit charger des charges.
  if (m_chargeFile != "") {
    ChargesReader* cR = new ChgChargesReader(m_chargeFile);
    geometries = *(cR->loadResources(&geometries));
    delete cR;
  }

  // On donne les géométries au calculateur.
  m_calculator->setGeometries(&geometries);

  // On sauvegarde les valeurs globales dans le calculateur.
  m_calculator->saveCalculationValues();

  // On lance les calculs.
  m_calculator->launchCalculations();

  // GERER L'ATTENTE DES THREADS DE CALCUL
  // (avec areCalculationsFinished())

  // Les calculs sont finis, on les enregistre dans le fichier output.
  std::ofstream oFile;
  oFile.open(m_outputFile);
  FileWriter* fileWriter = new StdFileWriter(oFile);

  // Variables globales.
  oFile << "****************" << std::endl;
  oFile << "GLOBAL VARIABLES" << std::endl;
  oFile << "****************" << std::endl;
  GeometryCalculator::CalculationValues calculationValues = m_calculator->getCalculationValues();
  oFile << "Temperature = " << calculationValues.temperature << std::endl;
  if (m_calculator->willEHSSBeCalculated() || m_calculator->willPABeCalculated()) {
    oFile << "Number of Monte-Carlo trajectories in EHSS/PA methods = " << GlobalParameters::getInstance()->getNbPointsMCIntegrationEHSSPA() << std::endl;
  }
  if (m_calculator->willTMBeCalculated()) {
    oFile << "**" << std::endl;
    oFile << "Potential energy at start (sw1) = " << calculationValues.potentialEnergyStart << std::endl;
    oFile << "Potential energy when close to a collision (sw2) = " << calculationValues.potentialEnergyCloseCollision << std::endl;
    oFile << "Time step at start (dtsf1) = " << calculationValues.timeStepStart << std::endl;
    oFile << "Time step when close to a collision (dtsf2) = " << calculationValues.timeStepCloseCollision << std::endl;
    oFile << "Energy conservation threshold = " << calculationValues.energyConservationThreshold << "%" << std::endl;
    oFile << "**" << std::endl;
    oFile << "Number of complete cycles for TM method (itn) = " << calculationValues.numberCyclesTM << std::endl;
    oFile << "Number of points in velocity integration (inp) = " << calculationValues.numberPointsVelocity << std::endl;
    oFile << "Number of points in Monte-Carlo integrations for TM method (imp) = " << calculationValues.numberPointsMCIntegrationTM << std::endl;
    oFile << "Total number of points = " << calculationValues.numberCyclesTM * calculationValues.numberPointsVelocity * calculationValues.numberPointsMCIntegrationTM << std::endl;
  }
  oFile << std::endl << std::endl;
  oFile << "*******" << std::endl;
  oFile << "RESULTS" << std::endl;
  oFile << "*******";

  // Resultats des calculs.
  int num = 1;
  for (auto it = geometries.begin(); it != geometries.end(); ++it) {
    Result* result = m_calculator->getResults(*it);

    oFile << std::endl << "-----" << std::endl;

    // D'abord, numéro de la géométrie associé au fichier d'où elle vient.
    oFile << "Geometry " << num << " from " << geometriesFromFile[*it] << " : " << std::endl;
    oFile << "-----" << std::endl;
    result->accept(*fileWriter);
    oFile << std::endl;
    ++num;
  }
  delete fileWriter;
  oFile.close();
}
