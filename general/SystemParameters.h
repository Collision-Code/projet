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

/**
 * \file SystemParameters.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 08 may 2016
 * \brief Class implementing a singleton to access system parameters.
 */

#ifndef SYSTEMPARAMETERS_H
#define SYSTEMPARAMETERS_H


class SystemParameters
{
  public:
    /**
     * \return an instance of SystemParameters to work with.
     */
    static SystemParameters* getInstance() {
      return m_instance;
    }

    /**
     * Destructor.
     */
    virtual ~SystemParameters();

    /**
     * Returns the maximal number of threads.
     * \return the maximal number of threads.
     */
    unsigned int getMaximalNumberThreads() const {
      return m_maxNumberThreads;
    }

    /**
     * Sets the maximal number of threads to n.
     * \param n the new maximal number of threads.
     */
    void setMaximalNumberThreads(int n) {
      m_maxNumberThreads = n;
    }

  private:
    /**
     * Constructor.
     */
    SystemParameters();

  private:
    /**
     * Static instance of SystemParameters to work with.
     */
    static SystemParameters* m_instance;

  private:
    /**
     * Maximal number of threads.
     * Default value : 20.
     */
    unsigned int m_maxNumberThreads;
};

#endif
