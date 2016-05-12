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

#include "StdMathLib.h"

#include "../general/AtomInformations.h"
#include "RandomGenerator.h"

#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846

StdMathLib::StdMathLib()
{

}

StdMathLib::~StdMathLib()
{

}

void StdMathLib::rotate(Molecule* mol, double angleX, double angleY, double angleZ)
{
  double oangle;
  double nangleX = 0.0;
  double nangleY = 0.0;
  double nangleZ = 0.0;

  double dist;

  // On recupere la vectore des atomes de la molecule.
  std::vector<Atom*> atoms = *(mol->getAllAtoms());

  // Pour tous les atomes de la molecule ...
  for (unsigned int i = 0; i < atoms.size(); ++i) {
    // On extrait la position actuelle de l'atome.
    Vector3D* actualPos = atoms[i]->getPosition();
    // On extrait la position initiale de l'atome.
    Vector3D initialPos = *(atoms[i]->getInitialPosition());

    // Rotation autour de l'axe des X.
    dist = sqrt(initialPos.x * initialPos.x + initialPos.y * initialPos.y);
    if (dist != 0.0) {
      oangle = acos(initialPos.x / dist);
      if (initialPos.y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleX = oangle + angleX;
    }
    actualPos->x = cos(nangleX) * dist;
    actualPos->y = sin(nangleX) * dist;

    // Rotation autour de l'axe des Y.
    dist = sqrt(initialPos.z * initialPos.z + actualPos->y * actualPos->y);
    if (dist != 0.0) {
      oangle = acos(initialPos.z / dist);
      if (actualPos->y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleY = oangle + angleY;
    }
    actualPos->z = cos(nangleY) * dist;
    actualPos->y = sin(nangleY) * dist;

    // Rotation autour de l'axe des Z.
    dist = sqrt(actualPos->x * actualPos->x + actualPos->y * actualPos->y);
    if (dist != 0.0) {
      oangle = acos(actualPos->x / dist);
      if (actualPos->y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleZ = oangle + angleZ;
    }
    actualPos->x = cos(nangleZ) * dist;
    actualPos->y = sin(nangleZ) * dist;
  }
}

void StdMathLib::rotate(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos, double angleX, double angleY, double angleZ)
{
  double oangle;
  double nangleX = 0.0;
  double nangleY = 0.0;
  double nangleZ = 0.0;

  double dist;

  // Pour tous les atomes de la molecule ...
  for (unsigned int i = 0; i < pos.size(); ++i) {
    // On extrait la position initiale de l'atome.
    Vector3D initialPos = initPos[i];

    // Rotation autour de l'axe des X.
    dist = sqrt(initialPos.x * initialPos.x + initialPos.y * initialPos.y);
    if (dist != 0.0) {
      oangle = acos(initialPos.x / dist);
      if (initialPos.y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleX = oangle + angleX;
    }
    pos[i].x = cos(nangleX) * dist;
    pos[i].y = sin(nangleX) * dist;

    // Rotation autour de l'axe des Y.
    dist = sqrt(initialPos.z * initialPos.z + pos[i].y * pos[i].y);
    if (dist != 0.0) {
      oangle = acos(initialPos.z / dist);
      if (pos[i].y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleY = oangle + angleY;
    }
    pos[i].z = cos(nangleY) * dist;
    pos[i].y = sin(nangleY) * dist;

    // Rotation autour de l'axe des Z.
    dist = sqrt(pos[i].x * pos[i].x + pos[i].y * pos[i].y);
    if (dist != 0.0) {
      oangle = acos(pos[i].x / dist);
      if (pos[i].y < 0.0) {
        oangle = 2.0 * M_PI - oangle;
      }
      nangleZ = oangle + angleZ;
    }
    pos[i].x = cos(nangleZ) * dist;
    pos[i].y = sin(nangleZ) * dist;
  }
}

void StdMathLib::randomRotation(Molecule* mol)
{
  // Définition des angles aléatoires.
  double angleX = 2.0 * M_PI * RandomGenerator::getInstance()->getRandomNumber();
  double angleY = asin(RandomGenerator::getInstance()->getRandomNumber() * 2.0 - 1.0) + M_PI / 2.0;
  double angleZ = 2.0 * M_PI * RandomGenerator::getInstance()->getRandomNumber();
  // Rotation.
  rotate(mol, angleX, angleY, angleZ);
}

void StdMathLib::randomRotation(const std::vector<Vector3D>& initPos, std::vector<Vector3D>& pos)
{
  // Définition des angles aléatoires.
  double angleX = 2.0 * M_PI * RandomGenerator::getInstance()->getRandomNumber();
  double angleY = asin(RandomGenerator::getInstance()->getRandomNumber() * 2.0 - 1.0) + M_PI / 2.0;
  double angleZ = 2.0 * M_PI * RandomGenerator::getInstance()->getRandomNumber();
  // Rotation.
  rotate(initPos, pos, angleX, angleY, angleZ);
}

Vector3D StdMathLib::calculateMassCenter(const Molecule& mol)
{
  double mass = 0.0;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  // On recupere la vectore des atomes de la molecule.
  std::vector<Atom*>* atoms = mol.getAllAtoms();

  // Pour récupérer les informations globales sur les atomes.
  AtomInformations* atomInformations = AtomInformations::getInstance();

  // Pour tous les atomes de la molecule ...
  for (auto it = atoms->begin(); it != atoms->end(); ++it) {
    // On extrait la masse de l'atome.
    double m = atomInformations->getAtomicMass((*it)->getSymbol());
    // On extrait la position de l'atome.
    Vector3D* pos = (*it)->getPosition();
    // On somme tout ça.
    mass += m;
    x += pos->x * m;
    y += pos->y * m;
    z += pos->z * m;
  }

  return Vector3D(x / mass, y / mass, z / mass);
}

Atom* StdMathLib::findFarthestAtom(const Molecule& mol)
{
  double maxDist = 0;
  // On recupere la vectore des atomes de la molecule.
  std::vector<Atom*> atoms = *(mol.getAllAtoms());
  // On initialise l'atome au premier de la vectore.
  Atom* farthestAtom = atoms.front();

  // On calcule le centre de masse de la molecule.
  Vector3D massCenter = calculateMassCenter(mol);

  for (auto it = atoms.begin(); it != atoms.end(); ++it) {
    // On extrait la position de l'atome.
    Vector3D* pos = (*it)->getPosition();
    // Calcul des differentes differences.
    double diffX = (massCenter.x - pos->x);
    double diffY = (massCenter.y - pos->y);
    double diffZ = (massCenter.z - pos->z);
    // Obtention de la distance (sans la racine carree).
    double dist = diffX * diffX + diffY * diffY + diffZ * diffZ;

    // Si la distance est superieure, on a trouve un nouvel atome plus loin.
    if (dist > maxDist) {
       maxDist = dist;
       farthestAtom = *it;
     }
  }

   return farthestAtom;
}

double StdMathLib::monteCarloIntegration(double (*f)(double), double minLimit, double maxLimit, int n)
{
  double u;
  double x;
  double sum = 0.0;
  double avgValue;

  for (int i = 0; i < n; ++i)
  {
    // On choisi un nombre entre 0 et 1.
    u = RandomGenerator::getInstance()->getRandomNumber();
    // On s'en sert pour trouver une abscisse aleatoire entre minLimit et maxLimit.
    x = minLimit + (maxLimit - minLimit) * u;
    // On ajoute a la somme l'image de l'abscisse aléatoire generee.
    sum += f(x);
    avgValue = sum / n;
  }

  return (maxLimit - minLimit) * avgValue;
}
