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

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include "../lib/boost/random.hpp"

class RandomGenerator
{
  public:
    /**
     * Returns an instance of RandomGenerator.
     * \return a pointer to a RandomGenerator.
     */
    static RandomGenerator* const getInstance()
    {
      return m_instance;
    }

    /**
     * Destructor.
     */
    virtual ~RandomGenerator();

    /**
     * Returns a random number between 0 and 1.
     * Random generation is uniform.
     */
    double getRandomNumber()
    {
      return (*m_generator)();
    }

  private:
    /**
     * Private constructor.
     */
    RandomGenerator();

  private:
    /**
     * Instance of RandomGenerator.
     */
    static RandomGenerator* m_instance;
    /**
     * The random number generator.
     */
    boost::variate_generator<boost::mt19937, boost::uniform_01<>>* m_generator;
};

#endif
