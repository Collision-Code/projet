#include "ConsoleView.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <sstream>
#include <iomanip>

#include "../general/StdCmdView.h"
#include "../general/AtomInformations.h"
#include "../general/SystemParameters.h"
#include "../general/GlobalParameters.h"
#include "../observer/Event.h"
#include "../observer/state/CalculationState.h"


ConsoleView::ConsoleView(int argc, char* const argv[])
  : m_cmdView(nullptr), m_dataFile("resources/atomInformations.csv"),
  m_outFile("resCollision.ccout"), m_error(false),
  m_geometriesFinished(0u), m_verbose(true)
{
  if (argc < 2) {
    // Pas de nom de fichier, c'est une erreur, on va rien faire.
    printError(argv[0], "");
  } else {
    // On instancie m_cmdView.
    m_cmdView = new StdCmdView();
    m_cmdView->addObserver(this);
    // On parse la ligne de commande.
    parse(argc, argv);
  }
}

ConsoleView::~ConsoleView() {
  delete m_cmdView;
}

void ConsoleView::update(ObservableEvent cond, Observable* obs) {
  // On n'affiche rien si jamais on est pas en mode "verbose".
  if (m_verbose) {
    CalculationState* cS;
    CmdView* cV;
    std::vector<std::string> inputFiles;

    switch(cond) {
    case ObservableEvent::EHSS_STARTED:
      if (m_cmdView->willEHSSBeCalculated()) {
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : Start EHSS" << std::endl;
      }
      break;

    case ObservableEvent::PA_STARTED:
      if (m_cmdView->willPABeCalculated()) {
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : Start PA" << std::endl;
      }
      break;

    case ObservableEvent::TM_STARTED:
      std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : Start TM" << std::endl;
      break;

    case ObservableEvent::EHSS_ENDED:
      if (m_cmdView->willEHSSBeCalculated()) {
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : End EHSS" << std::endl;
        cS = dynamic_cast<CalculationState*>(obs);
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : EHSS = " << cS->getEHSSResult() << std::endl;
      }
      break;

    case ObservableEvent::PA_ENDED:
      if (m_cmdView->willPABeCalculated()) {
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : End PA" << std::endl;
        cS = dynamic_cast<CalculationState*>(obs);
        std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : PA = " << cS->getPAResult() << std::endl;
      }
      break;

    case ObservableEvent::TM_ENDED:
      cS = dynamic_cast<CalculationState*>(obs);
      std::cout << std::fixed;
      std::cout << std::setprecision(2) << "\r " << cS->getPercentageFinishedTrajectories() << "% (" << cS->getNumberFinishedTractories() << "/" << cS->getNumberTotalTractories() << ")";
      std::cout << std::endl << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : End TM" << std::endl;
      std::cout << "[Geom. " << m_geometriesFinished + 1 << "/" << m_totalNumberGeometries << "] : TM = " << cS->getTMResult() << std::endl;
      std::cout << "-----" << std::endl;
      m_geometriesFinished++;
      break;

    case ObservableEvent::TRAJECTORY_NUMBER_UPDATE:
      cS = dynamic_cast<CalculationState*>(obs);
      std::cout << std::fixed;
      std::cout << std::setprecision(2) << "\r " << cS->getPercentageFinishedTrajectories() << "% (" << cS->getNumberFinishedTractories() << "/" << cS->getNumberTotalTractories() << ")";
      break;

    case ObservableEvent::GEOMETRIES_LOADED:
      cV = dynamic_cast<CmdView*>(obs);
      if (cV->getLoadedGeometries().size() == 1) {
        std::cout << cV->getLoadedGeometries().size() << " geometry loaded from : " << std::endl;
      } else {
        std::cout << cV->getLoadedGeometries().size() << " geometries loaded from : " << std::endl;
      }
      inputFiles = cV->getInputFiles();
      for (auto it = inputFiles.begin(); it != inputFiles.end(); ++it) {
        std::cout << " - " << *it << std::endl;
      }
      std::cout << "-----" << std::endl;
      break;

    case ObservableEvent::CHARGES_LOADED:
      cV = dynamic_cast<CmdView*>(obs);
      std::cout << "Charges loaded from " << cV->getChargeFile() << std::endl;
      std::cout << "-----" << std::endl;
      break;

    case ObservableEvent::FILE_SAVED:
      cV = dynamic_cast<CmdView*>(obs);
      std::cout << "Results saved in " << cV->getOutputFile() << std::endl;
      break;

    default:
      break;
    }
  }
}

void ConsoleView::launch() {
  try {
    // On charge les données sur les atomes.
    AtomInformations::getInstance()->loadFile(m_dataFile);

    // On va la chronométrer.
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    // On charge les fichiers
    m_totalNumberGeometries = m_cmdView->loadInputFiles();

    // On lance les calculs
    m_cmdView->launch();

    // Sauvegarde des resultats.
    m_cmdView->setOutputFile(m_outFile);
    m_cmdView->saveResults();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    // On calcule le temps dexécution.
    std::chrono::duration<double, std::milli> duration = t2 - t1;
    // Le calcul est terminé.
    // On affiche le temps total seulement si on est en mode "verbose".
    std::cout << "Total time : " << duration.count() / 1000 << "s." << std::endl;
  }
  catch(std::string caught) {
      std::cerr << caught << std::endl;
      return;
  }
}

void ConsoleView::parse(int argc, char* const argv[]) {
  // Tant que les options ne commencent pas par un tiret, ce sont des
  // fichiers d'input.
  int i;
  // Lecture des inputs.
  // UN SEUL FICHIER EN INPUT
  for (i = 1; i < 2; ++i) {
    if (argv[i][0] == '-') {
      // C'est une option, on sort de la boucle des inputs.
      break;
    }

    // On enregistre le fichier dans les inputs.
    m_cmdView->addInputFile(argv[i]);
  }

  // Lecture des options.
  for (; i < argc;) {
    if (strcmp(argv[i], "-tab") == 0) {
      /// Fichier de données sur les atomes.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de donnees sur les atomes.");
        return;
      }
      // On a un nom de fichier, on le prend.
      m_dataFile = argv[i];
      i++;
    } else if (strcmp(argv[i], "-out") == 0) {
      /// Fichier de sortie.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de sortie.");
        return;
      }
      // On a un nom de fichier, on le prend.
      m_outFile = argv[i];
      i++;
    } else if (strcmp(argv[i], "-chg") == 0) {
      /// Fichier de charge.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de charges.");
        return;
      }
      // On a un nom de fichier, on le prend.
      m_cmdView->setChargeFile(argv[i]);
      i++;
    } else if (strcmp(argv[i], "-nopa") == 0) {
      /// Pas de calcul de PA.
      m_cmdView->shouldPABeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "-noehss") == 0) {
      /// Pas de calcul de EHSS.
      m_cmdView->shouldEHSSBeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "-notm") == 0) {
      /// Pas de calcul de TM.
      m_cmdView->shouldTMBeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "--help") == 0) {
      /// Affichage de l'aide.
      printHelp(argv[0]);
      return;
    } else if (strcmp(argv[i], "-sil") == 0) {
      /// Mode silencieux.
      m_verbose = false;
      i++;
    } else if (strcmp(argv[i], "-th") == 0) {
      /// Nombre de threads.
      i++;
      // Si on n'a pas de nombre de threads après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de threads.");
        return;
      }
      // On prend le nombre de threads.
      try {
        int nbThreads = convertToInteger(std::string(argv[i]));
        if (nbThreads < 1) {
          printError(argv[0], "Veuillez entrer un nombre de threads valide.");
          return;
        }
        SystemParameters::getInstance()->setMaximalNumberThreads(nbThreads);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de threads valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-temp") == 0) {
      /// Temperature
      i++;
      // Si on n'a pas de temperature après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une temperature.");
        return;
      }
      // On prend la temperature.
      try {
        double temperature = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTemperature(temperature);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une temperature valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-mtp") == 0) {
      /// Nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA.");
        return;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNbPointsMCIntegrationEHSSPA(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-sw1") == 0) {
      /// Energie potentielle de depart.
      i++;
      // Si on n'a pas d'energie apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une energie potentielle de depart.");
        return;
      }
      // On prend l'energie potentielle.
      try {
        double ePot = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setPotentialEnergyStart(ePot);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une energie potentielle de depart valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-sw2") == 0) {
      /// Energie potentielle proche collision.
      i++;
      // Si on n'a pas d'energie apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une energie potentielle de proximite de collision.");
        return;
      }
      // On prend l'energie potentielle.
      try {
        double ePot = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setPotentialEnergyCloseCollision(ePot);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une energie potentielle de proximite de collision valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-dt1") == 0) {
      /// Pas de temps de depart.
      i++;
      // Si on n'a pas de pas de temps apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un pas de temps de depart.");
        return;
      }
      // On prend le pas de temps.
      try {
        double step = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTimeStepStart(step);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un pas de temps de depart valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-dt2") == 0) {
      /// Pas de temps proche collision.
      i++;
      // Si on n'a pas de pas de temps apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un pas de temps de proximite de collision.");
        return;
      }
      // On prend le pas de temps.
      try {
        double step = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTimeStepCloseCollision(step);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un pas de temps de proximite de collision valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-et") == 0) {
      /// Seuil de conservation de l'energie.
      i++;
      // Si on n'a pas de seuil apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un seuil de conservation de l'energie.");
        return;
      }
      // On prend le seuil.
      try {
        double threshold = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setEnergyConservationThreshold(threshold);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un seuil de conservation de l'energie valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-itn") == 0) {
      /// Nombre de cycles TM.
      i++;
      // Si on n'a pas de nombre de cycles apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de cycles pour la methode TM.");
        return;
      }
      // On prend le nombre de cycles.
      try {
        int nbCycles = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNumberCompleteCycles(nbCycles);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de cycles pour la methode TM valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-inp") == 0) {
      /// Nombre de points dans les integrations de vitesse.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points pour les integrations de vitesse.");
        return;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNumberVelocityPoints(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points pour les integrations de vitesse valide.");
        return;
      }
      i++;
    } else if (strcmp(argv[i], "-imp") == 0) {
      /// Nombre de points dans les integrations de Monte-Carlo pour la methode TM.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour la methode TM.");
        return;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNbPointsMCIntegrationTM(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour la methode TM valide.");
        return;
      }
      i++;
    } else {
      /// C'est une erreur.
      printError(argv[0], "Option inconnue.");
      return;
    }
  }
}

/**
 * \return a string describing the command parameters.
 */
std::string getCmdStr() {
  return std::string(" inFile [-chg chargesFile] [-tab dataFile] [-out outputFile] [-nopa] [-noehss] [-notm] [-th nbThreads] [-mtp nbPoints] [-temp temperature] [-sw1 potEnergyStart] [-sw2 potEnergyClose] [-dt1 timeStepStart] [-dt2 timeStepClose] [-et energyThreshold] [-itn nbCycles] [-inp nbPoints] [-imp nbPoints] [-sil] [--help]");
}

void ConsoleView::printHelp(std::string progName) {
  std::cout << "Aide pour " << progName << " : " << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Utilisation : " << std::endl;
  std::cout << progName << getCmdStr() << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "   inFile : Obligatoire. Le fichier des geometries de la molecule a etudier. Formats supportes : .log/.out, .mfj, .mol, .xyz, .pdb." << std::endl;
  std::cout << "   -chg chargesFile : Permet de preciser le fichier de charges, si souhaite." << std::endl;
  std::cout << "   -tab dataFile : Permet de preciser le fichier de donnees de modelisation atomique. Si non precise, le fichier sera cherche a resources/atomInformations.csv." << std::endl;
  std::cout << "   -out outputFile : Permet de preciser le nom du fichier de sortie. Si non precisee, le fichier de sortie sera resCollision.ccout." << std::endl;
  std::cout << "   -nopa : Precise que la methode PA ne devra pas etre calculee." << std::endl;
  std::cout << "   -noehss : Precise que la methode EHSS ne devra pas etre calculee." << std::endl;
  std::cout << "   -notm : Precise que la methode TM ne devra pas etre calculee." << std::endl;
  std::cout << "   -th nbThreads : Nombre de threads pour le calcul. Par defaut, " << SystemParameters::getInstance()->getMaximalNumberThreads() << "." << std::endl;
  std::cout << "   -temp temperature : Temperature. Par defaut, " << GlobalParameters::getInstance()->getTemperature() << " degres." << std::endl;
  std::cout << "   -mtp nbPoints : Nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA. Par defaut, " << GlobalParameters::getInstance()->getNbPointsMCIntegrationEHSSPA() << "." << std::endl;
  std::cout << "   -sw1 potEnergyStart : L'energie potentielle au debut du calcul d'une trajectoire par methode TM. Par defaut, " << GlobalParameters::getInstance()->getPotentialEnergyStart() << "." << std::endl;
  std::cout << "   -sw2 potEnergyClose : L'energie potentielle lorsqu'on est proche d'une collision dans le calcul d'une trajectoire par methode TM. Par defaut, " << GlobalParameters::getInstance()->getPotentialEnergyCloseCollision() << "." << std::endl;
  std::cout << "   -dt1 timeStepStart : Le pas entre deux points d'une trajectoire au debut de la trajectoire dans le calcul par methode TM. Par defaut, " << GlobalParameters::getInstance()->getTimeStepStart() << "." << std::endl;
  std::cout << "   -dt2 timeStepClose : Le pas entre deux points d'une trajectoire lorsqu'on est proche d'une collision dans le calcul par methode TM. Par defaut, " << GlobalParameters::getInstance()->getTimeStepCloseCollision() << "." << std::endl;
  std::cout << "   -et energyThreshold : Le seuil de conservation de l'energie entre deux points d'une trajectoire dans le calcul par methode TM. Par defaut, " << GlobalParameters::getInstance()->getEnergyConservationThreshold() << "%." << std::endl;
  std::cout << "   -itn nbCycles : Nombre de cycles complets pour la methode TM. Par defaut, " << GlobalParameters::getInstance()->getNumberCompleteCycles() << "." << std::endl;
  std::cout << "   -inp nbPoints : Nombre de points dans les integrations de vitesse. Par defaut, " << GlobalParameters::getInstance()->getNumberVelocityPoints() << "." << std::endl;
  std::cout << "   -imp nbPoints : Nombre de points dans les integrations de Monte-Carlo pour la methode TM. Par defaut, " << GlobalParameters::getInstance()->getNbPointsMCIntegrationTM() << "." << std::endl;
  std::cout << "   -sil : Mode \"silencieux\". Aucune information ne sera affichee dans la console durant le calcul." << std::endl;
  std::cout << "   --help : Affiche l'aide." << std::endl;
  std::cout << "-----" << std::endl;

  // On va indiquer une erreur, meme si yen a pas, pour pouvoir quitter
  // l'application sans faire de calcul.
  m_error = true;
}

void ConsoleView::printError(std::string progName, std::string error) {
  if (error != "") {
    std::cerr << error << std::endl;
  }
  std::cerr << "Utilisation : " << std::endl;
  std::cerr << progName << getCmdStr() << std::endl;

  // On indique une erreur.
  m_error = true;
}

double ConsoleView::convertToDouble(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x)) {
    // Conversion impossible.
    throw std::invalid_argument("Cannot convert to double.");
  }
  return x;
}

int ConsoleView::convertToInteger(const std::string& s)
{
  std::istringstream i(s);
  int x;
  if (!(i >> x)) {
    // Conversion impossible.
    throw std::invalid_argument("Cannot convert to integer.");
  }
  return x;
}
