#include "MultiThreadCalculationOperator.h"

#include "../general/AtomInformations.h"
#include "../molecule/StdMolecule.h"
#include "../molecule/StdAtom.h"
#include "StdResult.h"
#include "MathLib.h"
#include "StdMathLib.h"
#include "RandomGenerator.h"

#include <omp.h>

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

#include <boost/math/special_functions/pow.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



MultiThreadCalculationOperator::MultiThreadCalculationOperator(Molecule* mol,
                                   int maximalNumberThreads,
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
  : StdCalculationOperator(mol,
                           temperature,
                           potentialEnergyStart,
                           timeStepStart,
                           potentialEnergyCloseCollision,
                           timeStepCloseCollision,
                           numberCyclesTM,
                           numberPointsVelocity,
                           numberPointsMCIntegrationTM,
                           energyConservationThreshold,
                           numberPointsMCIntegrationEHSSPA),
    m_maximalNumberThreads(maximalNumberThreads)
{

}

MultiThreadCalculationOperator::~MultiThreadCalculationOperator()
{

}

/**
 * Calculate TM and put the results in
 * m_result attribute.
 */
void MultiThreadCalculationOperator::calculateTM()
{
  // Parametres de mobil2 :
  // t -> temperature (298)
  //    -> StdCalculationOperator::m_Temperature
  // itn -> number of complete cycles for average mobility calculation (10)
  //    -> MonoThreadCalculationOperator::m_NumberCyclesTM
  // inp -> number of points in velocity integration (20)
  //    -> MonoThreadCalculationOperator::m_NumberPointsVelocity
  // imp -> number of points in MC integrations of impact parameter and orientation (25)
  //    -> MonoThreadCalculationOperator::m_numberPointsMCIntegrationTM
  // mob -> TM's result (mobility)
  // cs -> TM's result (CCS)
  // sdevpc -> ??

  // im2 est juste une variable pour l'affichage, iu3 et iu2 et ip aussi


  // Un objet pour manipuler les fonctions mathÃ©matiques.
  MathLib* mathLib = new StdMathLib();

  // Masses.
  const double m1 = 4.0026;
  const double m2 = m_molMass;

  // Variables de travail.
  Vector3D iPos;
  double dMax = 0.0;
  const int NbCasesCosX = 500;
  std::array<double, NbCasesCosX + 1> cosx;
  std::vector<double> q1st(m_numberPointsVelocity);
  std::vector<double> q2st(m_numberPointsVelocity);
  std::vector<double> om11st(m_numberCyclesTM);
  std::vector<double> om12st(m_numberCyclesTM);
  std::vector<double> om13st(m_numberCyclesTM);
  std::vector<double> om22st(m_numberCyclesTM);

  // Atome le plus Ã©loignÃ© sur l'axe des x.
  double rMax = 0.0;
  double r;
  // Une variable retenant l'atome augmentant le plus rMax.
  int atomHold;
  for (unsigned int i = 0; i < m_molNbAtoms; ++i) {
    iPos = m_molInitPos[i];
    r = sqrt(iPos.x * iPos.x + iPos.y * iPos.y + iPos.z * iPos.z);
    if (r > rMax) {
      rMax = r;
      atomHold = i;
    }
  }


  // Rotation de l'atome le plus Ã©loignÃ© sur l'axe des X.
  iPos = m_molInitPos[atomHold];
  double angleX = 0.0;
  double angleY = 0.0;
  double angleZ = 0.0;
  double rzy = sqrt(iPos.y * iPos.y + iPos.z * iPos.z);

  // Rotation d'un angle Y.
  angleY = acos(iPos.z / rzy) + (M_PI / 2.0);
  if (iPos.y < 0.0) {
    angleY = 2.0 * M_PI - angleY;
  }
  angleY = 2.0 * M_PI - angleY;
  mathLib->rotate(m_molInitPos, m_molPos, angleX, angleY, angleZ);

  // Rotation d'un angle Z.
  double rxy = sqrt(iPos.x * iPos.x + iPos.y * iPos.y);
  angleZ = acos(iPos.x / rxy);
  if (iPos.y < 0.0) {
    angleZ = 2.0 * M_PI - angleZ;
  }
  angleZ = 2.0 * M_PI - angleZ;
  mathLib->rotate(m_molInitPos, m_molPos, angleX, angleY, angleZ);



  // DÃ©terminaison de rMax, eMax et r00 sur les directions x, y et z.
  const int irn = 1000;
  double ddd = (rMax + m_maxROLJ) / irn;

  Vector3D p;
  Vector3D r00;
  Vector3D rMaxVec;
  Vector3D dPot;
  Vector3D eMax;

  double pot;

  // Definition du nombre de threads : le max possible.
  omp_set_num_threads(m_maximalNumberThreads);
  #pragma opm parallel
  {
    #pragma omp for
    for (int i = 1; i <= irn; ++i) {
      p.x = rMax + m_maxROLJ - (i * ddd);
      pot = calculatePotentials(m_molPos, p, dPot, dMax);
      if (pot <= 0.0) {
        r00.x = p.x;
        if (pot < eMax.x) {
          rMaxVec.x = p.x;
          eMax.x = pot;
        }
      }
    }

    p.x = 0.0;
    #pragma omp for
    for (int i = 1; i <= irn; ++i) {
      p.y = rMax + m_maxROLJ - (i * ddd);
      pot = calculatePotentials(m_molPos, p, dPot, dMax);
      if (pot <= 0.0) {
        r00.y = p.y;
        if (pot < eMax.y) {
          rMaxVec.y = p.y;
          eMax.y = pot;
        }
      }
    }

    p.y = 0.0;
    #pragma omp for
    for (int i = 1; i <= irn; ++i) {
      p.z = rMax + m_maxROLJ - (i * ddd);
      pot = calculatePotentials(m_molPos, p, dPot, dMax);
      if (pot <= 0.0) {
        r00.z = p.z;
        if (pot < eMax.z) {
          rMaxVec.z = p.z;
          eMax.z = pot;
        }
      }
    }
  }


  // PrÃ©paration de l'intÃ©gration sur gst.
  const double tst = m_XkFromMobcal * m_temperature / m_EoFromMobcal;
  double tst3 = boost::math::pow<3>(tst);

  double dgst = 5.0 * pow(10, -7) * 6.0 * sqrt(tst);
  double gst = dgst;
  double sum = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;

  for (int i = 1; i <= m_numberPointsVelocity; ++i) {
    sum1 += sqrt(i);
  }

  double hold1;
  double hold2;
  std::vector<double> wgst(m_numberPointsVelocity + 1);
  std::vector<double> pgst(m_numberPointsVelocity + 1);
  double gstt;

  for (int i = 1; i <= m_numberPointsVelocity; ++i) {
    hold1 = sqrt(i);
    hold2 = sqrt(i - 1);
    sum2 += hold2;
    wgst[i] = hold1 / sum1;
    gstt = tst3 * (sum2 + (hold1 / 2.0)) / sum1;

    while (sum < gstt) {
      sum += exp(-gst * gst / tst) * boost::math::pow<5>(gst) * dgst;
      gst = gst + dgst;
      if (sum > gstt) {
        pgst[i] = gst - (dgst / 2.0);
      }
    }
  }


  // DÃ©termination de b2max.
  // Ligne 1456.
  double dbst2 = 1.0;
  double dbst22 = dbst2 / 10.0;
  double cmin = 0.0005;
  double gst2;
  double v;
  int ibst;
  std::vector<double> b2max(m_numberPointsVelocity + 1, 0.0);
  double bst2;
  double b;
  double ang;

  // Definition du nombre de threads : le max possible.
  if (m_maximalNumberThreads >= m_numberPointsVelocity) {
    omp_set_num_threads(m_numberPointsVelocity);
  } else {
    omp_set_num_threads(m_maximalNumberThreads);
  }
  #pragma omp parallel for private(v, gst2, ibst, bst2, b, ang)
  for (int i = m_numberPointsVelocity; i >= 1; --i) {
    gst2 = boost::math::pow<2>(pgst[i]);
    v = sqrt((gst2 * m_EoFromMobcal) / (0.5 * m_massConstant));
    ibst = (int) (rMaxVec.x / m_RoFromMobcal) - 6;
    if (i < m_numberPointsVelocity) {
      ibst = (int) (b2max[i + 1] / dbst2) - 6;
    }
    if (ibst < 0) {
      ibst = 0;
    }
    // do/while pour le goto 3000 ligne 1490
    do {
      bst2 = dbst2 * ibst;
      b = m_RoFromMobcal * sqrt(bst2);
      // gsang 1475
      // Pas besoin d'erat dans l'appel Ã  gsang ?
      // Pas besoin de d1 ?
      // istep inutile dans Mobcal ?
      ang = calculateTrajectory(m_molPos, v, b);
      cosx[ibst] = 1.0 - cos(ang);

      // C'est le bordel dans les goto
      // On a une boucle de la ligne 1473 Ã  la ligne 1490 Ã  cause du goto 3000
      // Le seul moyen de sortir c'est le goto 3020 ligne 1480-1482
      // On va le symboliser par un break, ici
      if (ibst >= 4 && cosx[ibst] < cmin
          && cosx[ibst - 1] < cmin
          && cosx[ibst - 2] < cmin
          && cosx[ibst - 3] < cmin
          && cosx[ibst - 4] < cmin) {
        break;
      }

      // Le goto 3010 ligne 1479 sera symbolisÃ© par un if ici.
      // Comme le goto 3000 est obligatoire, on va boucler.
      ibst += 1;
      if (ibst > NbCasesCosX) {
        // ERREUR
        throw std::string("Ibst superior to 500.");
      }
    } while(true);

    // Ligne 1491, on arrive ici aprÃ¨s le goto 3020 ligne 1482.
    b2max[i] = (ibst - 5) * dbst2;
    // Boucle do/while pour le goto 3040 ligne 1495
    do {
      b2max[i] += dbst22;
      b = m_RoFromMobcal * sqrt(b2max[i]);
      ang = calculateTrajectory(m_molPos, v, b);
    } while (1.0 - cos(ang) > cmin);
  }
  // Fin de la boucle for : continue ligne 1496


  // On calcule Omega(1, 1)*,  Omega(1, 2)*, Omega(1, 3) et Omega(2, 2)*
  // en intÃ©grant Q(1)* ou Q(2)* sur toutes les orientations et Ã  des
  // vÃ©locitÃ©s initiales relatives.
  for (int i = 0; i < m_numberPointsVelocity; ++i) {
    q1st[i] = 0.0;
    q2st[i] = 0.0;
  }

  for (int ic = 0; ic < m_numberCyclesTM; ++ic) {
    om11st[ic] = 0.0;
    om12st[ic] = 0.0;
    om13st[ic] = 0.0;
    om22st[ic] = 0.0;
  }

  omp_set_nested(1);

  // Calcul du nombre de threads max pour une boucle imbriquee.
  MultiThreadCalculationOperator::NumberThread nT = getOptimizedNumberThreads(m_numberCyclesTM);
  omp_set_num_threads(nT.externLoop);

  #pragma omp parallel for
  for (int ic = 0; ic < m_numberCyclesTM; ++ic) {
    double om11stSum = 0.0;
    double om12stSum = 0.0;
    double om13stSum = 0.0;
    double om22stSum = 0.0;

    omp_set_num_threads(nT.innerLoop);

    #pragma omp parallel for reduction(+:om11stSum,om12stSum,om13stSum,om22stSum)
    for (int ig = 0; ig < m_numberPointsVelocity; ++ig) {
      // Retirer etoile : std::vector<Vector3D>
      std::vector<Vector3D> molPos(m_molPos);
      double valpgst = pgst[ig + 1];
      double gst2 = valpgst * valpgst;
      double v = sqrt((gst2 * m_EoFromMobcal) / (0.5 * m_massConstant));
      double temp1 = 0.0;
      double temp2 = 0.0;

      double rnb;
      double bst2;
      double b;
      double ang;
      double hold1;
      double hold2;
      double valb2max = b2max[ig + 1];

      for (int im = 0; im < m_numberPointsMCIntegrationTM; ++im) {
        rnb = RandomGenerator::getInstance()->getRandomNumber();
        mathLib->randomRotation(m_molInitPos, molPos);
        bst2 = rnb * valb2max;
        b = m_RoFromMobcal * sqrt(bst2);
        ang = calculateTrajectory(molPos, v, b);
        hold1 = 1.0 - cos(ang);
        hold2 = sin(ang);
        hold2 *= hold2;
        temp1 += (hold1 * valb2max);
        temp2 += (1.5 * hold2 * valb2max);
      }

      temp1 /= m_numberPointsMCIntegrationTM;
      temp2 /= m_numberPointsMCIntegrationTM;

      double valwgst = wgst[ig + 1];

      // Variable tampon
      om11stSum += temp1 * valwgst;
      om12stSum += temp1 * valpgst * valpgst * valwgst * (1.0 / (3.0 * tst));
      om13stSum += temp1 * boost::math::pow<4>(valpgst) * valwgst * (1.0 / (12.0 * tst * tst));
      om22stSum += temp2 * valpgst * valpgst * valwgst * (1.0 / (3.0 * tst));
      q1st[ig] += temp1;
      q2st[ig] += temp2;
    }

    om11st[ic] = om11stSum;
    om12st[ic] = om12stSum;
    om13st[ic] = om13stSum;
    om22st[ic] = om22stSum;
  }


  // On calcul les moyennes.
  // Ligne 1582.
  hold1 = 0.0;
  hold2 = 0.0;
  double temp = 0.0;

  for (int icc = 0; icc < m_numberCyclesTM; ++icc) {
    temp = 1.0 / (m_mobilityConstant / (sqrt(m_temperature) * om11st[icc] * M_PI * m_RoFromMobcal * m_RoFromMobcal));
    hold1 += om11st[icc];
    hold2 += temp;
  }

  // Moyennes.
  double mom11st = 0.0;
  double mom12st = 0.0;
  double mom13st = 0.0;
  double mom22st = 0.0;
  for (int ic = 0; ic < m_numberCyclesTM; ++ic) {
    mom11st += om11st[ic];
    mom12st += om12st[ic];
    mom13st += om13st[ic];
    mom22st += om22st[ic];
  }
  mom11st /= m_numberCyclesTM;
  mom12st /= m_numberCyclesTM;
  mom13st /= m_numberCyclesTM;
  mom22st /= m_numberCyclesTM;

  // Deviation standard.
  double sdom11st = 0.0;
  double hold;
  for (int ic = 0; ic < m_numberCyclesTM; ++ic) {
      hold = mom11st - om11st[ic];
      sdom11st += hold * hold;
  }
  // Ne sert Ã  rien ? DÃ©viation standard
  sdom11st = sqrt(sdom11st / m_numberCyclesTM);
  double cs = mom11st * M_PI * m_RoFromMobcal * m_RoFromMobcal;
  // sdevpc est enregistree pour la deivation standard
  double sdevpc = 100.0 * sdom11st / mom11st;
  m_result->setStandardDeviation(sdevpc);


  // On utilise les omegas pour obtenir un ordre supÃ©rieur de facteur
  // de correction pour les mobilitÃ©s.
  // Ligne 1633.
  double ayst = mom22st / mom11st;
  double best = ((5.0 * mom12st) - (4.0 * mom13st)) / mom11st;
  double cest = mom12st / mom11st;
  double term = ((4.0 * ayst) / 15.0) + (0.5 * (pow((m2 - m1), 2.0) / (m1 * m2)));
  double u2 = term - (0.08333 * (2.4 * best + 1.0) * (m1 / m2));
  double w = m1 / m2;
  double delta = (pow(((6.0 * cest) - 5.0), 2.0) * w) / (60.0 * (1.0 + u2));
  double f = 1.0 / (1.0 - delta);
  double mob = (m_mobilityConstant * f) / (sqrt(m_temperature) * cs);

  // Average TM mobility ;
  double averageTMMobility = mob;
  // Inverse average TM mobility :
  double inverseAverageTMMobility = 1.0 / mob;

  // Average TM cross section.
  double TMCrossSection = cs * 1.0 * pow(10, 20);

  delete mathLib;

  m_result->setTM(TMCrossSection);
}

MultiThreadCalculationOperator::NumberThread MultiThreadCalculationOperator::getOptimizedNumberThreads(double maxThreadOnExternLoop) {
  int nbMaxThreads = m_maximalNumberThreads;
  // Si c'est un nombre premier, on diminue.
  while (boost::multiprecision::miller_rabin_test(nbMaxThreads, 25)) {
    nbMaxThreads--;
  }

  NumberThread nT;

  int maxThreads = 0;

  for (int i = 1; i <= maxThreadOnExternLoop; ++i) {
    int n = nbMaxThreads / i;
    if (i * n >= maxThreads) {
      maxThreads = i * n;
      nT.externLoop = i;
      nT.innerLoop = n;
    }
  }

  return nT;
}
