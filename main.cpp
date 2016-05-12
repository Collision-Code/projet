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

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <sstream>

#include "general/CmdView.h"
#include "general/StdCmdView.h"
#include "general/AtomInformations.h"
#include "general/SystemParameters.h"
#include "general/GlobalParameters.h"


void printHelp(std::string progName);
void printError(std::string progName, std::string error);
double convertToDouble(const std::string& s);
int convertToInteger(const std::string& s);


int main(int argc, char* const argv[])
{
  // Paramètres en entrée :
  // [inFile]+ [-tab dateFile] [-out outFile]

  // Pas de paramètres, c'est une erreur.
  if (argc < 2) {
    printError(argv[0], "");
    return -1;
  }

  // Instance de CmdView.
  CmdView* cmdView = new StdCmdView();
  // Fichier de données sur les atomes.
  std::string dataFile = "resources/atomInformations.csv";
  // Fichier de sortie.
  std::string outFile = "resCollision.ccout";
  // Fichier de charges.
  std::string chargeFile = "";

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
    cmdView->addInputFile(argv[i]);
  }

  // Lecture des options.
  for (; i < argc;) {
    if (strcmp(argv[i], "-tab") == 0) {
      /// Fichier de données sur les atomes.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de donnees sur les atomes.");
        return -1;
      }
      // On a un nom de fichier, on le prend.
      dataFile = argv[i];
      i++;
    } else if (strcmp(argv[i], "-out") == 0) {
      /// Fichier de sortie.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de sortie.");
        return -1;
      }
      // On a un nom de fichier, on le prend.
      outFile = argv[i];
      i++;
    } else if (strcmp(argv[i], "-chg") == 0) {
      /// Fichier de charge.
      i++;
      // Si on n'a pas de nom de fichier après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nom de fichier de charges.");
        return -1;
      }
      // On a un nom de fichier, on le prend.
      chargeFile = argv[i];
      i++;
    } else if (strcmp(argv[i], "-nopa") == 0) {
      /// Pas de calcul de PA.
      cmdView->shouldPABeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "-noehss") == 0) {
      /// Pas de calcul de EHSS.
      cmdView->shouldEHSSBeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "-notm") == 0) {
      /// Pas de calcul de TM.
      cmdView->shouldTMBeCalculated(false);
      i++;
    } else if (strcmp(argv[i], "--help") == 0) {
      /// Affichage de l'aide.
      printHelp(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "-th") == 0) {
      /// Nombre de threads.
      i++;
      // Si on n'a pas de nombre de threads après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de threads.");
        return -1;
      }
      // On prend le nombre de threads.
      try {
        int nbThreads = convertToInteger(std::string(argv[i]));
        if (nbThreads < 1) {
          printError(argv[0], "Veuillez entrer un nombre de threads valide.");
          return -1;
        }
        SystemParameters::getInstance()->setMaximalNumberThreads(nbThreads);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de threads valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-temp") == 0) {
      /// Temperature
      i++;
      // Si on n'a pas de temperature après, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une temperature.");
        return -1;
      }
      // On prend la temperature.
      try {
        double temperature = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTemperature(temperature);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une temperature valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-mtp") == 0) {
      /// Nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA.");
        return -1;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNbPointsMCIntegrationEHSSPA(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour les methodes EHSS et PA valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-sw1") == 0) {
      /// Energie potentielle de depart.
      i++;
      // Si on n'a pas d'energie apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une energie potentielle de depart.");
        return -1;
      }
      // On prend l'energie potentielle.
      try {
        double ePot = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setPotentialEnergyStart(ePot);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une energie potentielle de depart valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-sw2") == 0) {
      /// Energie potentielle proche collision.
      i++;
      // Si on n'a pas d'energie apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer une energie potentielle de proximite de collision.");
        return -1;
      }
      // On prend l'energie potentielle.
      try {
        double ePot = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setPotentialEnergyCloseCollision(ePot);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer une energie potentielle de proximite de collision valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-dt1") == 0) {
      /// Pas de temps de depart.
      i++;
      // Si on n'a pas de pas de temps apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un pas de temps de depart.");
        return -1;
      }
      // On prend le pas de temps.
      try {
        double step = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTimeStepStart(step);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un pas de temps de depart valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-dt2") == 0) {
      /// Pas de temps proche collision.
      i++;
      // Si on n'a pas de pas de temps apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un pas de temps de proximite de collision.");
        return -1;
      }
      // On prend le pas de temps.
      try {
        double step = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setTimeStepCloseCollision(step);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un pas de temps de proximite de collision valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-et") == 0) {
      /// Seuil de conservation de l'energie.
      i++;
      // Si on n'a pas de seuil apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un seuil de conservation de l'energie.");
        return -1;
      }
      // On prend le seuil.
      try {
        double threshold = convertToDouble(std::string(argv[i]));
        GlobalParameters::getInstance()->setEnergyConservationThreshold(threshold);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un seuil de conservation de l'energie valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-itn") == 0) {
      /// Nombre de cycles TM.
      i++;
      // Si on n'a pas de nombre de cycles apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de cycles pour la methode TM.");
        return -1;
      }
      // On prend le nombre de cycles.
      try {
        int nbCycles = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNumberCompleteCycles(nbCycles);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de cycles pour la methode TM valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-inp") == 0) {
      /// Nombre de points dans les integrations de vitesse.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points pour les integrations de vitesse.");
        return -1;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNumberVelocityPoints(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points pour les integrations de vitesse valide.");
        return -1;
      }
      i++;
    } else if (strcmp(argv[i], "-imp") == 0) {
      /// Nombre de points dans les integrations de Monte-Carlo pour la methode TM.
      i++;
      // Si on n'a pas de nombre de points apres, c'est une erreur.
      if (i == argc) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour la methode TM.");
        return -1;
      }
      // On prend le nombre de points.
      try {
        int nbPoints = convertToInteger(std::string(argv[i]));
        GlobalParameters::getInstance()->setNbPointsMCIntegrationTM(nbPoints);
      } catch(std::invalid_argument e) {
        printError(argv[0], "Veuillez entrer un nombre de points dans les integrations de Monte-Carlo pour la methode TM valide.");
        return -1;
      }
      i++;
    } else {
      /// C'est une erreur.
      printError(argv[0], "Option inconnue.");
      return -1;
    }
  }

  try {
    // On charge les données sur les atomes.
    AtomInformations::getInstance()->loadFile(dataFile);

    // On ajoute le fichier de sortie.
    cmdView->setOutputFile(outFile);
    // On ajoute le fichier de charge.
    cmdView->setChargeFile(chargeFile);

    // On lance le calcul.
    std::cout << "Lancement des calculs." << std::endl;
    // On va la chronométrer.
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    cmdView->launch();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    // On calcule le temps dexécution.
    std::chrono::duration<double, std::milli> duration = t2 - t1;
    // Le calcul est terminé.
    std::cout << "Calculs termines en " << duration.count() / 1000 << "s." << std::endl;
  }
  catch(std::string caught) {
      std::cerr << caught << std::endl;
      return -1;
  }

  delete cmdView;

  return 0;
}

std::string getCmdStr() {
  return std::string(" inFile [-chg chargesFile] [-tab dataFile] [-out outputFile] [-nopa] [-noehss] [-notm] [-th nbThreads] [-mtp nbPoints] [-temp temperature] [-sw1 potEnergyStart] [-sw2 potEnergyClose] [-dt1 timeStepStart] [-dt2 timeStepClose] [-et energyThreshold] [-itn nbCycles] [-inp nbPoints] [-imp nbPoints] [--help]");
}

void printHelp(std::string progName) {
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
  std::cout << "   -itn nbCycles : Nombre de cycles complets pour la méthode TM. Par defaut, " << GlobalParameters::getInstance()->getNumberCompleteCycles() << "." << std::endl;
  std::cout << "   -inp nbPoints : Nombre de points dans les integrations de vitesse. Par defaut, " << GlobalParameters::getInstance()->getNumberVelocityPoints() << "." << std::endl;
  std::cout << "   -imp nbPoints : Nombre de points dans les integrations de Monte-Carlo pour la methode TM. Par defaut, " << GlobalParameters::getInstance()->getNbPointsMCIntegrationTM() << "." << std::endl;
  std::cout << "   --help : Affiche l'aide." << std::endl;
  std::cout << "-----" << std::endl;
}

void printError(std::string progName, std::string error) {
  if (error != "") {
    std::cerr << error << std::endl;
  }
  std::cerr << "Utilisation : " << std::endl;
  std::cerr << progName << getCmdStr() << std::endl;
}

double convertToDouble(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x)) {
      // Conversion impossible.
    throw std::invalid_argument("Cannot convert to double.");
  }
  return x;
}

int convertToInteger(const std::string& s)
{
  std::istringstream i(s);
  int x;
  if (!(i >> x)) {
      // Conversion impossible.
    throw std::invalid_argument("Cannot convert to integer.");
  }
  return x;
}
