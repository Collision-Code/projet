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

#include "StdCalculationOperator.h"

#include "../general/AtomInformations.h"
#include "../molecule/StdMolecule.h"
#include "../molecule/StdAtom.h"
#include "StdResult.h"
#include "MathLib.h"
#include "StdMathLib.h"
#include "RandomGenerator.h"

#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

#include <boost/math/special_functions/pow.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ANGSTROMTOMETER (1e-10)

/**
 * Maximum of successive reflections followed.
 */
const int StdCalculationOperator::m_MaxSuccRefl = 30;


/// TM
// dipol in Mobcal.
// Polarizability of Helium = 0.204956 * 10^-30 m^3.
// Permitivity of vacuum = 8.854187817 * 10^-12 F.m^-1.
// xe = 1.60217733 * 10^-19.
const double StdCalculationOperator::m_IonInducedDipolePotential =
  (0.204956 * pow(10, -30) / (8.0 * M_PI * 8.854187817 * pow(10, -12)))
    * pow(1.60217733 * pow(10, -19), 2);

const double StdCalculationOperator::m_XeFromMobcal =
  1.60217733 * pow(10, -19);

const double StdCalculationOperator::m_XkFromMobcal =
  1.380658 * pow(10, -23);

const double StdCalculationOperator::m_XmvFromMobcal =
  0.0224141;

const double StdCalculationOperator::m_EoFromMobcal =
  1.34 * pow(10, -3) * 1.60217733 * pow(10, -19);

const double StdCalculationOperator::m_RoFromMobcal =
  3.043 * pow(10, -10);

// inwr dans Mobcal.
const int StdCalculationOperator::m_NbIntegrationStep = 1;

// cmin dans Mobcal.
const double StdCalculationOperator::m_MaxImpactParameter = 0.0005;


// Constantes globales pour diffeq.
const double var = 2.97013888888;
const double cvar = 0.990972222222;
const double acst = 0.332866152768;
const double a[] = {
  0.5,
  0.292893218814,
  1.70710678118,
  0.1666666666667
};
const double b[] = {
  2.0,
  1.0,
  1.0,
  2.0
};
const double c[] = {
  -0.5,
  -0.292893218814,
  -1.70710678118,
  -0.5
};
const double ampc[] = {
  -0.111059153612,
  0.672667757774,
  -1.70633621697,
  2.33387888707,
  -1.8524668225
};
const double amcc[] = {
  0.0189208128941,
  -0.121233356692,
  0.337771548703,
  -0.55921513665
};




StdCalculationOperator::StdCalculationOperator(Molecule* mol,
                                               double temperature,
                                               double potentialEnergyStart,
                                               double timeStepStart,
                                               double potentialEnergyCloseCollision,
                                               double timeStepCloseCollision,
                                               double numberCyclesTM,
                                               double numberPointsVelocity,
                                               double numberPointsMCIntegrationTM,
                                               double energyConservationThreshold,
                                               double numberPointsMCIntegrationEHSSPA)
  : m_mol(mol), m_maxROLJ(0.0), m_temperature(temperature),
  m_potentialEnergyStart(potentialEnergyStart), m_numberCyclesTM(numberCyclesTM),
  m_numberPointsVelocity(numberPointsVelocity), m_numberPointsMCIntegrationTM(numberPointsMCIntegrationTM),
  m_numberPointsMCIntegrationEHSSPA(numberPointsMCIntegrationEHSSPA), m_timeStepStart(timeStepStart),
  m_potentialEnergyCloseCollision(potentialEnergyCloseCollision),
  m_timeStepCloseCollision(timeStepCloseCollision), m_energyConservationThreshold(energyConservationThreshold)
{
  m_result = new StdResult(m_mol);

  // Calcul de la constante de mobilité.
  const double m1 = 4.0026;
  const double m2 = mol->getTotalMass();
  const double xn = 6.0221367 * boost::math::pow<23>(10);

  // mu dans Mobcal.
  // Ligne 199.
  m_massConstant = ((m1 * m2) / (m1 + m2)) / (xn * boost::math::pow<3>(10));

  // mconst dans Mobcal.
  // Ligne 203.
  m_mobilityConstant = sqrt(18.0 * M_PI) / 16.0;
  m_mobilityConstant *= sqrt(xn * 1.0 * boost::math::pow<3>(10)) * sqrt((1.0 / m1) + (1.0 / m2));
  m_mobilityConstant *= m_XeFromMobcal / sqrt(m_XkFromMobcal);
  double dens = xn / m_XmvFromMobcal;
  m_mobilityConstant /= dens;
}

StdCalculationOperator::~StdCalculationOperator()
{
  // Le résultat perdure car récupéré en amont.
}

/**
 * Launches the calculation of EHSS and PA.
 */
void StdCalculationOperator::runEHSSAndPA()
{
  // Préparation d'une molécule pour le calcul, à partir
  // de la molécule originelle.

  // Outil mathématique.
  MathLib* mathLib = new StdMathLib();
    // Acces aux donnees de la base de donnees.
  AtomInformations* atomInf = AtomInformations::getInstance();
  // Centre de masse de la molécule :
  Vector3D massCenter = mathLib->calculateMassCenter(*m_mol);

  m_rhsTab.clear();

  Molecule* newMol = new StdMolecule();
  // On recupere la vectore des atomes de la molecule à étudier.
  std::vector<Atom*> atoms = *(m_mol->getAllAtoms());
  // On parcourt tous les atomes.
  for (unsigned int i = 0; i < atoms.size(); ++i) {
    // On extrait la position de l'atome.
    Vector3D* pos = atoms[i]->getPosition();
    // On extrait le symbole de l'atome.
    std::string symb = atoms[i]->getSymbol();
    // On extrait la charge de l'atome.
    double charge = atoms[i]->getCharge();

    // Pour chacun des atomes, on va rajouter un nouvelle atome
    // à la copie de la molécule avec comme position :
    // x_newMol = (pos.x - massCenter.x) * 10^-10
    // y_newMol = (pos.y - massCenter.y) * 10^-10
    // z_newMol = (pos.z - massCenter.z) * 10^-10
    newMol->addAtom(new StdAtom(new Vector3D((pos->x - massCenter.x),
                                             (pos->y - massCenter.y),
                                             (pos->z - massCenter.z)),
                                symb,
                                charge));

    // Enregistremen du RHS pour l'accélération des calculs.
    m_rhsTab.push_back(atomInf->getHSRadius(atoms[i]->getSymbol()));
  }

  delete mathLib;

  calculateEHSSAndPA(newMol);

  delete newMol;
}

/**
 * Launches the calculation of TM.
 */
void StdCalculationOperator::runTM()
{
  // PrÃ©paration d'une molÃ©cule pour le calcul, Ã  partir
  // de la molÃ©cule originelle.

  // Outil mathÃ©matique.
  MathLib* mathLib = new StdMathLib();
    // Acces aux donnees de la base de donnees.
  AtomInformations* atomInf = AtomInformations::getInstance();
  // Centre de masse de la molÃ©cule :
  Vector3D massCenter = mathLib->calculateMassCenter(*m_mol);

  m_rhsTab.clear();

  Molecule* newMol = new StdMolecule();
  // On recupere la vectore des atomes de la molecule Ã  Ã©tudier.
  std::vector<Atom*> atoms = *(m_mol->getAllAtoms());
  // On parcourt tous les atomes.
  for (unsigned int i = 0; i < atoms.size(); ++i) {
    // On extrait la position de l'atome.
    Vector3D* pos = atoms[i]->getPosition();
    // On extrait le symbole de l'atome.
    std::string symb = atoms[i]->getSymbol();
    // On extrait la charge de l'atome.
    double charge = atoms[i]->getCharge();

    // Pour chacun des atomes, on va rajouter un nouvelle atome
    // Ã  la copie de la molÃ©cule avec comme position :
    // x_newMol = (pos.x - massCenter.x) * 10^-10
    // y_newMol = (pos.y - massCenter.y) * 10^-10
    // z_newMol = (pos.z - massCenter.z) * 10^-10
    newMol->addAtom(new StdAtom(new Vector3D((pos->x - massCenter.x) * ANGSTROMTOMETER,
                                             (pos->y - massCenter.y) * ANGSTROMTOMETER * -1,
                                             (pos->z - massCenter.z) * ANGSTROMTOMETER * -1),
                                symb,
                                charge));

    // Enregistremen du RHS pour l'accÃ©lÃ©ration des calculs.
    m_rhsTab.push_back(atomInf->getHSRadius(atoms[i]->getSymbol()));
    // Enregistremen de EOLJ pour l'accÃ©lÃ©ration des calculs.
    // Conversion en mÃ¨tres.
    double eolj = atomInf->getEOLJHe(atoms[i]->getSymbol()) * m_XeFromMobcal * boost::math::pow<-3>(10);
    m_EOLJTab.push_back(eolj);
    // Enregistremen de ROLJ pour l'accÃ©lÃ©ration des calculs.
    // Conversion en mÃ¨tres.
    double rolj = atomInf->getROLJHe(atoms[i]->getSymbol()) * ANGSTROMTOMETER;
    m_ROLJTab.push_back(rolj);
    if (rolj > m_maxROLJ) {
      m_maxROLJ = rolj;
    }
  }

  m_molInitPos.clear();
  m_molPos.clear();
  m_molChg.clear();

  // On ajoute les positions et les charges de la molÃ©cule dans les tableaux en attribut.
  std::vector<Atom*> atoms2 = *(newMol->getAllAtoms());
  for (int i = 0; i < atoms2.size(); ++i) {
    m_molInitPos.push_back(*(atoms2[i]->getPosition()));
    m_molPos.push_back(*(atoms2[i]->getPosition()));
    m_molChg.push_back(atoms2[i]->getCharge());
  }
  m_molNbAtoms = atoms2.size();
  m_molMass = newMol->getTotalMass();


  delete mathLib;

  calculateAsymmetryParameter();

  // On enregistre le parametre d'asymetrie dans les resultats.
  m_result->setStructAsymParam(m_asymmetryParameter);

  calculateTM();

  delete newMol;
}

/**
 * Calculate EHSS and PA and put the results in
 * m_result attribute.
 */
void StdCalculationOperator::calculateEHSSAndPA(Molecule* mol)
{
  // Stocke les cosinus des "moitiés" d'angle entre les vecteurs
  // d'incidence et les vecteurs de reflexion (par collisions
  // successives le long d'une trajectoire unique).
  std::array<double, StdCalculationOperator::m_MaxSuccRefl + 1> halfCos;
  // Initialise le tableau des CCS à zéro.
  std::array<double, StdCalculationOperator::m_MaxSuccRefl + 1> ccsArray;

  // Initialisation des deux tableaux à 0.
  for (int i = 0; i < StdCalculationOperator::m_MaxSuccRefl + 1; ++i) {
    halfCos[i] = 0.0;
    ccsArray[i] = 0.0;
  }

  // La projection ("hard-sphere cross-section").
  double projection = 0.0;
  // Le plus grand ordre rencontré pour n'importe quelle trajectoire.
  int highestCollOrder = 1;

  // Un objet pour manipuler les fonctions mathématiques.
  MathLib* mathLib = new StdMathLib();

  // On détermine les extrémités sur l'axe des y et des z de
  // la boîte.
  double ymin = 0.0;
  double ymax = 0.0;
  double zmin = 0.0;
  double zmax = 0.0;

  Vector3D pos;
  double rhs;
  double y1;
  double y2;
  double z1;
  double z2;
  double yDim;
  double zDim;
  double area;
  double yRand;
  double zRand;


  // Début de l'intégration de Monte-Carlo.
  for (int i = 0; i < m_numberPointsMCIntegrationEHSSPA; ++i) {
    // Rotation aléatoire.
    mathLib->randomRotation(mol);

    // On recupere la vectore des atomes de la molecule à étudier.
    std::vector<Atom*> atoms = *(mol->getAllAtoms());
    // On parcourt tous les atomes.
    for (unsigned int i = 0; i < atoms.size(); ++i) {
      // On extrait la position de l'atome.
      pos = *(atoms[i]->getPosition());
      // On extrait le RHS de l'atome.
      rhs = m_rhsTab[i];

      y1 = pos.y + rhs;
      y2 = pos.y - rhs;
      z1 = pos.z + rhs;
      z2 = pos.z - rhs;

      // On actualise les cotés de la boite si on a trouvé une position plus
      // lointaine que les précédentes.
      if (y1 > ymax)
        ymax = y1;
      if (y2 < ymin)
        ymin = y2;
      if (z1 > zmax)
        zmax = z1;
      if (z2 < zmin)
        zmin = z2;
    }

    // yDim et zDim sont les longueurs des cotés de la boite.
    yDim = ymax - ymin;
    zDim = zmax - zmin;
    // L'aire de la boite.
    area = yDim * zDim;

    // On tire des coordonnées aléatoires dans la boite.
    yRand = ymin + yDim * RandomGenerator::getInstance()->getRandomNumber();
    zRand = zmin + zDim * RandomGenerator::getInstance()->getRandomNumber();

    // Le vecteur d'incidence initial (colinéaire à l'axe des x).
    Vector3D vecIncidInit(1.0, 0.0, 0.0);
    bool kp = false;

    for (int refl = 1; refl <= StdCalculationOperator::m_MaxSuccRefl; ++refl) {
      che(mol, refl, halfCos[refl], halfCos[refl - 1], yRand, zRand, kp, vecIncidInit);

      // Si l'inclusion du prochain ordre de collision n'a pas changé
      // l'angle d'indice/de réflexion (c'est à dire qu'il n'y a eu aucune
      // collision, on arrête de suivre cette trajectoire.
      if (halfCos[refl] == halfCos[refl - 1]) {
        // Si nécessaire, on met à jour highestCollOrder.
        if (refl - 1 > highestCollOrder) {
          highestCollOrder = refl - 1;
        }

        // Construit tous les prochains cosinus d'angles d'incidence/
        // de réflexion de cette trajectoire au dernier angle trouvé
        // (comme il ne changera plus).
        for (int k = refl + 1; k <= StdCalculationOperator::m_MaxSuccRefl; ++k) {
          halfCos[k] = halfCos[refl];
        }
        // On sort du for.
        break;
      } else {
        if(refl > highestCollOrder) {
          highestCollOrder = refl;
        }
      }
    }

    // On ajoute les contributions de la trajectoire aux calculs de CCS
    // pour tous les ordres.
    for (int refl = 1; refl <= StdCalculationOperator::m_MaxSuccRefl; ++refl) {
      ccsArray[refl] += area * halfCos[refl] * halfCos[refl];
    }

    if (kp) {
      projection += area;
    }
  }
  // Fin de l'intégration de Monte-Carlo.
  // On normalise toutes les CCS et la projection.
  for (int i = 1; i <= highestCollOrder; ++i) {
    ccsArray[i] /= 0.5 * m_numberPointsMCIntegrationEHSSPA;
  }
  projection /= m_numberPointsMCIntegrationEHSSPA;

  double averagePACS = projection;
  double averagePAMobility = StdCalculationOperator::m_mobilityConstant / (sqrt(m_temperature) * averagePACS);
  double averageEHSSCS = ccsArray[highestCollOrder];
  double averageEHSSMobility = StdCalculationOperator::m_mobilityConstant / (sqrt(m_temperature) * averageEHSSCS);

  // Enregistrement des résultats.
  m_result->setEHSS(averageEHSSCS);
  m_result->setPA(averagePACS);

  delete mathLib;
}

void StdCalculationOperator::che(Molecule* mol, int refl, double& halfCos, double cop, double& yRand, double& zRand, bool& kp, Vector3D& initialIncidenceVector)
{
  // If this is a secondary collision, the incident vector lies on the
  // x axis in transformed coordinates.
  if (refl != 1) {
    yRand = 0.0;
    zRand = 0.0;
  }

  // xl est un grand nombre.
  double xl = pow(10, 6);
  // L'atome avec lequel la collision il y a (ou null si
  // il n'y a pas eu de collision).
  Atom* collidingAtom = nullptr;

  // Variables de travail.
  Vector3D* pos;
  double yImp;
  double zImp;
  double ras;
  double dev;
  double rhs;
  double xCol;

  double dixPMoinsSix = pow(10, -6);

  // On recupere la vectore des atomes de la molecule à étudier.
  std::vector<Atom*> atoms = *(mol->getAllAtoms());
  unsigned int atomsSize = atoms.size();
  // On parcourt tous les atomes.
  for (unsigned int i = 0; i < atomsSize; ++i) {
    // On extrait la position de l'atome.
    pos = atoms[i]->getPosition();

    if (pos->x > dixPMoinsSix || refl == 1) {
      // yImp et zImp sont les coordonnées des points d'impact pour
      // l'atome, en respectant ses propres coordonnées (si un tel
      // point existe).
      yImp = yRand - pos->y;
      zImp = zRand - pos->z;
      // dev est le paramètre d'impact.
      ras = (yImp * yImp) + (zImp * zImp);
      dev = sqrt(ras);

      // Le rayon de sphere dure de l'atome.
      rhs = m_rhsTab[i];

      // Si une collision avec l'atome est detectee.
      if (dev <= rhs) {
        // On cherche la coordonnée de collision sur l'axe des X.
        xCol = pos->x - sqrt((rhs * rhs) - ras);

        // On cherche le plus petit xCol (la collision qui arrive
        // en première).
        if (xCol < xl) {
          xl = xCol;
          collidingAtom = atoms[i];
        }
      }
    }
  }

  // Si il y a eu collision, alors xVec, yVec et zVec sont
  // les vecteurs allant du centre de l'atome de la collision
  // jusqu'au point de la collision.
  if (collidingAtom != nullptr) {
    kp = true;

    // On extrait la position de l'atome de la collision.
    Vector3D posCol = *(collidingAtom->getPosition());

    double xVec = xl - posCol.x;
    double yVec = yRand - posCol.y;
    double zVec = zRand - posCol.z;

    // Transforme toutes les coordonnees par un mouvement
    // parallèle pour le point de collision soit à (0, 0, 0).
    for (unsigned int i = 0; i < atomsSize; ++i) {
      pos = atoms[i]->getPosition();
      pos->x = pos->x - xl;
      pos->y = pos->y - yRand;
      pos->z = pos->z - zRand;
    }

    // Transforme les coordonnes de tous les atomes de façon
    // à ce que le vecteur de direction de la particule réfléchie
    // soit collinéaire à l'axe des X.
    // (A noter : le nombre de telles transformations est infini,
    // donc la direction sur les axes des x et z est arbitraire).
    // Les cosinus des directions du rayon arrivant sont aussi
    // transformés.
    // xve1, xve2 et xve3 sont les vecteurs de direction du rayon
    // réfléchi dans le système de coordonnées du rayon arrivant.

    // On évalue les éléments de la matrice de transformation.
    double xxVec = 2.0 * xVec * xVec;
    double xyVec = 2.0 * xVec * yVec;
    double xzVec = 2.0 * xVec * zVec;
    double xyz = xyVec * xzVec;

    // Le rayon de sphere dure de l'atome.
    double rhs = AtomInformations::getInstance()->getHSRadius(collidingAtom->getSymbol());
    double rhs2 = rhs * rhs;
    double rad2 = rhs2 - xxVec;
    double ad1 = rad2 * rad2 + xyVec * xyVec;
    double adr1 = sqrt(ad1);
    double adr2 = sqrt(ad1 * ad1 + xyz * xyz + xzVec * xzVec * rad2 * rad2);

    double xve1 = 1.0 - 2.0 * xVec * xVec / rhs2;
    double xve2 = -xyVec / rhs2;
    double xve3 = -xzVec / rhs2;

    double yve1 = xyVec / adr1;
    double yve2 = rad2 / adr1;
    double yve3 = 0.0;

    double zve1 = rad2 * xzVec / adr2;
    double zve2 = -xyz / adr2;
    double zve3 = ad1 / adr2;

    // Transforme les coordonnées et les cosinus de direction du rayon arrivant.
    for (unsigned int i = 0; i < atomsSize; ++i) {
      pos = atoms[i]->getPosition();
      double xne = xve1 * pos->x + xve2 * pos->y + xve3 * pos->z;
      double yne = yve1 * pos->x + yve2 * pos->y + yve3 * pos->z;
      double zne = zve1 * pos->x + zve2 * pos->y + zve3 * pos->z;
      pos->x = xne;
      pos->y = yne;
      pos->z = zne;
    }
    // La même pour le vecteur d'incidence.
    double xne = xve1 * initialIncidenceVector.x + xve2 * initialIncidenceVector.y + xve3 * initialIncidenceVector.z;
    double yne = yve1 * initialIncidenceVector.x + yve2 * initialIncidenceVector.y + yve3 * initialIncidenceVector.z;
    double zne = zve1 * initialIncidenceVector.x + zve2 * initialIncidenceVector.y + zve3 * initialIncidenceVector.z;
    initialIncidenceVector.x = xne;
    initialIncidenceVector.y = yne;
    initialIncidenceVector.z = zne;

    // Calcule halfCos : le cosinus de l'angle entre le rayon incident
    // et la normal à un plan imaginaire, la reflection résultante serait
    // équivalent à la réflection acutelle multicorps.
    // (Calculate cof - the cosine of the angle between the incident ray
    //  and the normal to an imaginary plane, the reflection from which
    //  would be equivalent to the actual multibody reflection.)
    halfCos = cos((M_PI - acos(initialIncidenceVector.x)) / 2.0);
  } else {
    // Si il n'y a pas eu de collision, le cosinus n'a pas changé.
    halfCos = cop;
  }
}


/**
 * Calculates the structural asymmetry parameter and
 * puts it in m_asymmetryParameter.
 */
void StdCalculationOperator::calculateAsymmetryParameter()
{
  // Outil mathÃ©matique.
  MathLib* mathLib = new StdMathLib();

  double angleX = 0.0;
  m_asymmetryParameter = 0.0;

  // Variables de travail.
  double xyzSum = 0.0;
  double yzSum = 0.0;
  double yzTemp;
  double hold;

  // On tourne la molÃ©cule complÃ©tement autour de deux axes.
  for (double angleY = 0.0; angleY < 360.0; angleY += 2.0) {
    for (double angleZ = 0.0; angleZ < 360.0; angleZ += 2.0) {
      mathLib->rotate(m_molInitPos, m_molPos, angleX, angleY, angleZ);
      xyzSum = 0.0;
      yzSum = 0.0;

      for (unsigned int i = 0; i < m_molNbAtoms; ++i) {
        Vector3D pos = m_molPos[i];
        yzTemp = pos.y * pos.y + pos.z * pos.z;
        xyzSum += sqrt(pos.x * pos.x + yzTemp);
        yzSum += sqrt(yzTemp);
      }

      hold = ((M_PI / 4.0) * xyzSum) / yzSum;
      if (hold > m_asymmetryParameter) {
        m_asymmetryParameter = hold;
      }
    }
  }

  delete mathLib;
}

/**
 * Calculates the potential and the derivates of the potential.
 * The potential is given by a sum of 6-12 two body ...
 * \param p the position for the calculation
 * \param dPot the derivates of the potential
 * \return the potential
 */
double StdCalculationOperator::calculatePotentials(std::vector<Vector3D>& molPos, const Vector3D& p, Vector3D& dPot, double& dMax)
{
  // Variables de travail.
  Vector3D rPos(0.0, 0.0, 0.0);
  double e00 = 0.0;
  double de00 = 0.0;
  Vector3D de00Vec(0.0, 0.0, 0.0);

  double xx = 0.0;
  double xx2 = 0.0;
  double yy = 0.0;
  double yy2 = 0.0;
  double zz = 0.0;
  double zz2 = 0.0;
  double rxyz = 0.0;
  double rxyz2 = 0.0;
  double rxyz3 = 0.0;
  double rxyz5 = 0.0;
  double rxyz6 = 0.0;
  double rxyz8 = 0.0;
  double rxyz12 = 0.0;
  double rxyz14 = 0.0;
  double eox4 = 0.0;
  double rolj6 = 0.0;
  double rolj12 = 0.0;
  double charge = 0.0;
  double rxyz3i = 0.0;
  double rxyz5i = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum3 = 0.0;
  double sum4 = 0.0;
  double sum5 = 0.0;
  double sum6 = 0.0;

  // Resultat.
  double pot = 0.0;

  dMax = 2.0 * m_maxROLJ;

  // On parcourt tous les atomes.
  for (unsigned int i = 0; i < m_molNbAtoms; ++i) {
    // La position de l'atome.
    Vector3D pos = molPos[i];

    xx = p.x - pos.x;
    xx2 = boost::math::pow<2>(xx);
    yy = p.y - pos.y;
    yy2 = boost::math::pow<2>(yy);
    zz = p.z - pos.z;
    zz2 = boost::math::pow<2>(zz);
    rxyz2 = xx2 + yy2 + zz2;
    rxyz = sqrt(rxyz2);

    if (rxyz < dMax) {
      dMax = rxyz;
    }

    rxyz3=rxyz2*rxyz;
    rxyz5=rxyz3*rxyz2;
    rxyz6=rxyz5*rxyz;
    rxyz8=rxyz5*rxyz3;
    rxyz12=rxyz6*rxyz6;
    rxyz14=rxyz12*rxyz2;

    // Puissances de Lennard-Jones.
    eox4 = (4.0 * m_EOLJTab[i]);
    rolj6 = boost::math::pow<6>(m_ROLJTab[i]);
    rolj12 = rolj6 * rolj6;

    // Potentiel de Lennard-Jones.
    e00 += eox4 * ((rolj12 / rxyz12)
        - (rolj6 / rxyz6));

    // DÃ©rivÃ©s du potentiel de Lennard-Jones.
    de00 = eox4 * (((6.0 * rolj6) / rxyz8)
        - ((12.0 * rolj12) / rxyz14));
    de00Vec.x += de00 * xx;
    de00Vec.y += de00 * yy;
    de00Vec.z += de00 * zz;

    // Potentiel des ions induits.
    charge = m_molChg[i];
    if (charge != 0.0) {
      rxyz3i = charge / rxyz3;
      rxyz5i = -3.0 * charge / rxyz5;
      rPos.x += xx * rxyz3i;
      rPos.y += yy * rxyz3i;
      rPos.z += zz * rxyz3i;
      // DÃ©rivÃ©s des ions induits.
      sum1 += rxyz3i + (xx2 * rxyz5i);
      sum2 += xx * yy * rxyz5i;
      sum3 += xx * zz * rxyz5i;
      sum4 += rxyz3i + (yy2 * rxyz5i);
      sum5 += yy * zz * rxyz5i;
      sum6 += rxyz3i + (zz2 * rxyz5i);
    }
  }

  pot = e00 - (m_IonInducedDipolePotential
    * (rPos.x * rPos.x + rPos.y * rPos.y + rPos.z * rPos.z));
  dPot.x = de00Vec.x - (m_IonInducedDipolePotential
    * ((2.0 * rPos.x * sum1) + (2.0 * rPos.y * sum2) + (2.0 * rPos.z * sum3)));
  dPot.y = de00Vec.y - (m_IonInducedDipolePotential
    * ((2.0 * rPos.x * sum2) + (2.0 * rPos.y * sum4) + (2.0 * rPos.z * sum5)));
  dPot.z = de00Vec.z - (m_IonInducedDipolePotential
    * ((2.0 * rPos.x * sum3) + (2.0 * rPos.y * sum5) + (2.0 * rPos.z * sum6)));

  return pot;
}

// Dans Mobcal, il y a erat. Mais apparemment, elle est seulement utilisÃ© en interne
// de gsang, donc retirÃ©e ici.
// d1 inutile dans Mobcal ?
// istep inutile dans Mobcal ?
double StdCalculationOperator::calculateTrajectory(std::vector<Vector3D>& molPos, double v, double b)
{
  Vector3D vVec(0.0, -v, 0.0);

  // Normalement en parametres.
  double ang = 0.0;
  double erat;
  double etot;
  //double d1;
  double dMax = 0.0;

  // Tableau global w dans Mobcal -> attention, indices dÃ©calÃ©s,
  // en C++ Ã§a commence Ã  0, Ã§a commence Ã  1 en Fortran.
  std::array<double, 6> w;
  // Tableau global dw dans Mobcal -> attention, indices dÃ©calÃ©s,
  // en C++ Ã§a commence Ã  0, Ã§a commence Ã  1 en Fortran.
  std::array<double, 6> dw;


  // On calcule le pas entre chaque point de la trajectoire
  double top;
  if (v >= 3000.0) {
    top = 2.5;
  } else if (v >= 2000.0) {
    top = 10.0 - ((v - 2000.0) * 7.5 * boost::math::pow<-3>(10));
  } else if (v >= 1000.0) {
    top = 10.0;
  } else {
    top = (v / 95.2381) - 0.5;
  }

  double dt1 = (top * m_timeStepStart * 1.0 * boost::math::pow<-11>(10)) / v;
  double dt2 = dt1 * m_timeStepCloseCollision;
  double dt = dt1;

  // On calcule le point de dÃ©part de la trajectoire.
  double e0 = 0.5 * m_massConstant * v * v;
  // Variable pour x, y et z dans Mobcal.
  Vector3D xyz(b, 0.0, 0.0);

  double yMin = 0.0;
  double yMax = 0.0;
  for (unsigned int i = 0; i < m_molNbAtoms; ++i) {
    Vector3D pos = molPos[i];
    if (pos.y > yMax) {
      yMax = pos.y;
    }
    if (pos.y < yMin) {
      yMin = pos.y;
    }
  }
  // Conversion en metres
  yMax /= 1.0 * ANGSTROMTOMETER;
  yMin /= 1.0 * ANGSTROMTOMETER;
  int iyMin = (int) yMin - 1;
  int iyMax = (int) yMax + 1;

  int id2 = iyMax;
  xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
  Vector3D dpot(0.0, 0.0, 0.0);
  double pot = calculatePotentials(molPos, xyz, dpot, dMax);
  // Le if puis do while permet de remplacer le if / goto 300 lignes 1006-1019
  if (fabs(pot / e0) <= m_potentialEnergyStart) {
    do {
      id2 -= 1.0;
      xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
      pot = calculatePotentials(molPos, xyz, dpot, dMax);
      if (id2 < iyMin) {
        ang = 0.0;
        erat = 1.0;
        // On retourne
        return ang;
      }

      xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
      pot = calculatePotentials(molPos, xyz, dpot, dMax);
    } while (fabs(pot / e0) < m_potentialEnergyStart);
  } else {
    // Ligne 1022 -> goto 302
    // Le do while remplace le if/goto 302
    do {
      id2 += 10.0;
      xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
      pot = calculatePotentials(molPos, xyz, dpot, dMax);
    } while (fabs(pot / e0) > m_potentialEnergyStart);

    // Ligne 1026
    // Le do while remplace le if/goto 301
    do {
      id2 -= 1.0;
      xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
      pot = calculatePotentials(molPos, xyz, dpot, dMax);
    } while(fabs(pot / e0) < m_potentialEnergyStart);
  }

  // Ligne 1031 : goto 304
  xyz.y = id2 * 1.0 * ANGSTROMTOMETER;
  etot = e0 + pot;

  // CoordonnÃ©es initiales et momentum.
  w[0] = xyz.x;
  w[1] = vVec.x * m_massConstant;
  w[2] = xyz.y;
  w[3] = vVec.y * m_massConstant;
  w[4] = xyz.z;
  w[5] = vVec.z * m_massConstant;
  double tim = 0.0;

  // Initialise les derivÃ©es du temps des coordonnÃ©es et du momentum.
  // deriv ligne 1052
  pot = calculateHamilton(molPos, w, dw, dMax);
  int ns = 0;
  int nw = 0;
  std::array<std::array<double, 6>, 6> arrayDouble = {{0.0}};

  // ATTENTION : imbrication de boucles do/while car ya des goto partout
  double e;
  int l = 0;
  double hVar = 0.0;
  double hcVar = 0.0;
  do {
    do {
      do {
        // do/while pour remplacer le goto ligne 1058
        do {
          pot = calculateRKandAM(molPos, l, tim, dt, w, dw, arrayDouble, dMax, hVar, hcVar);
          nw += 1;
        } while (nw != m_NbIntegrationStep);
        ns += nw;
        nw = 0;

        // On verifie si on a "perdu" la trajectoire (trop d'essais)
        // A ameliorer
        if (ns > 30000) {
          ang = M_PI / 2.0;
          e = 0.5 * m_massConstant * (dw[0] * dw[0] + dw[2] * dw[2] + dw[4] * dw[4]);
          erat = (e + pot) / etot;
          // istep inutile
          return ang;
        }


      // On vÃ©rifie si la trajectoire est terminÃ©e.
      // boucle do/while pour le goto 15 de la ligne 1086
      } while (dMax < m_maxROLJ);

      if (fabs(pot / e0) > m_potentialEnergyCloseCollision && dt == dt1) {
        dt = dt2;
        l = 0.0;
      }
      if (fabs(pot / e0) < m_potentialEnergyCloseCollision && dt == dt2) {
        dt = dt1;
        l = 0.0;
      }
    // Boucle do/while pour le goto 15, ligne 1095
    } while(fabs(pot / e0) > m_potentialEnergyStart);
    // Boucle do/while pour le goto 15 ligne 1096
  } while (ns < 50);


  // On dÃ©termine l'angle de dÃ©viation
  double num = dw[2] * (-v);
  double den = v * sqrt(dw[0] * dw[0] + dw[2] * dw[2] + dw[4] * dw[4]);
  if (dw[0] > 0.0) {
    ang = acos(num / den);
  } else if (dw[0] < 0.0) {
    ang = -acos(num / den);
  }


  // On vÃ©rifie la conservation de l'Ã©nergie.
  e = 0.5 * m_massConstant * (dw[0] * dw[0] + dw[2] * dw[2] + dw[4] * dw[4]);
  erat = (e + pot) / etot;
  // Ici, pourcentage de conservation d'Ã©nergie, Ã  modifier pour prendre en input.
  if (erat < 2.0 - (m_energyConservationThreshold / 100.0) && erat > m_energyConservationThreshold / 100.0) {
    // Energie conservÃ©e.
    return ang;
  } else {
    m_result->setNumberOfFailedTrajectories(m_result->getNumberOfFailedTrajectories() + 1);
    // Energie non conservÃ©e, on retourne quoi ?
    return ang;
  }
}

/**
 * Defines Hamilton's equations of motion ad the time derivates of
 * the coordinates and momenta.
 * \return the potential
 */
double StdCalculationOperator::calculateHamilton(std::vector<Vector3D>& molPos, std::array<double, 6>& w, std::array<double, 6>& dw, double& dMax)
{
  // Dans les Ã©quations d'Hamilton, les dÃ©rivÃ©es des coordonnÃ©es selon le temps
  // sont les conjuguÃ©s divisÃ©s par la masse.
  dw[0] = w[1] / m_massConstant;
  dw[2] = w[3] / m_massConstant;
  dw[4] = w[5] / m_massConstant;
  // Les Ã©quations d'Hamilton pour les dÃ©rivÃ©es du momentum selon le temps
  // sont Ã©valuÃ©es en utilisation les dÃ©rivÃ©es des coordonnÃ©es.
  // Ce sont des dÃ©rivÃ©es analytiques.
  Vector3D dPot(0.0, 0.0, 0.0);
  double pot = calculatePotentials(molPos, Vector3D(w[0], w[2], w[4]), dPot, dMax);
  dw[1] = -dPot.x;
  dw[3] = -dPot.y;
  dw[5] = -dPot.z;

  return pot;
}

/**
 * Integration method. Uses 5th order Runge-Kutta-Gill to initiate and 5th order
 * Adams-Moulton predictor-corrector to propagate.
 * \return the potential
 */
double StdCalculationOperator::calculateRKandAM(std::vector<Vector3D>& molPos, int& l, double& tim, double& dt, std::array<double, 6>& w, std::array<double, 6>& dw, std::array<std::array<double, 6>, 6>& arrayDouble, double& dMax, double& hVar, double& hcVar)
{
  // pot inutile Ã  mon avis
  double pot = 0.0;
  double q[6];
  double savw[6];
  double savdw[6];
  // k Ã  retirer, Ã  mon avis, remplacÃ©e par la variable redo plus bas.
  double r;

  if (l >= 0) {
    // goto 1, ligne 1171
    if (l == 0) {
      for (int j = 0; j < 6; ++j) {
        q[j] = 0.0;
      }
      hVar = dt * var;
      hcVar = dt * cvar;
      dt *= 0.5;
    }

    // goto 3, ligne 1176
    l += 1;

    // Runge-Kutta. Les Ã©tapes sont sÃ©parÃ©es en demi-Ã©tapes pour plus
    // de prÃ©cision.
    // pour le goto 15, ligne 1193, la boucle k (on le fait deux fois en fait)
    for (int k = 0; k < 2; ++k) {
      for (int j = 0; j < 4; ++j) {
        if (pow(-1.0, (j + 1)) > 0.0) {
          tim += 0.5 * dt;
        }
        pot = calculateHamilton(molPos, w, dw, dMax);

        for (int i = 0; i < 6; ++i) {
          dw[i] *= dt;
          r = a[j] * (dw[i] - b[j] * q[i]);
          w[i] += r;
          q[i] = q[i] + 3.0 * r + c[j] * dw[i];
        }
      }
      pot = calculateHamilton(molPos, w, dw, dMax);
      // goto 15, ligne 1193
    }

    if (l - 6 >= 0) {
      l = -1;
      dt *= 2.0;
    } else {
      for (int j = 0; j < 6; ++j) {
        arrayDouble[l - 1][j] = dw[j];
      }
    }
    return pot;
  } else {
    // goto 4, ligne 1204
    for (int j = 0; j < 6; ++j) {
      savw[j] = w[j];
      savdw[j] = dw[j];
      arrayDouble[5][j] = savdw[j];

      for (int i = 0; i < 5; ++i) {
        arrayDouble[5][j] += ampc[i] * arrayDouble[i][j];
      }
      w[j] += arrayDouble[5][j] * hVar;
    }
    tim += dt;

    pot = calculateHamilton(molPos, w, dw, dMax);
    for (int j = 0; j < 6; ++j) {
      arrayDouble[5][j] = acst * dw[j];
      for (int i = 0; i < 4; ++i) {
        arrayDouble[i][j] = arrayDouble[i + 1][j];
        arrayDouble[5][j] += arrayDouble[i][j] * amcc[i];
      }
      arrayDouble[4][j] = savdw[j];
      w[j] = savw[j] + hcVar * (arrayDouble[4][j] + arrayDouble[5][j]);
    }

    pot = calculateHamilton(molPos, w, dw, dMax);
    return pot;
  }
}
