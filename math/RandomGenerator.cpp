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

#include "RandomGenerator.h"

#include <ctime>

RandomGenerator* RandomGenerator::m_instance = new RandomGenerator();

RandomGenerator::RandomGenerator()
{
  boost::mt19937 rng = boost::mt19937(time(nullptr));
  boost::uniform_01<> unif;
  m_generator = new boost::variate_generator<boost::mt19937, boost::uniform_01<>>(rng, unif);
}

RandomGenerator::~RandomGenerator()
{
  delete m_generator;
}
