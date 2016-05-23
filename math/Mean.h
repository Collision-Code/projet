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
 * \file Mean.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Interface describing a way of save mean of calculations results.
 */

#ifndef MEAN_H
#define MEAN_H

#include "Result.h"

class Mean
{
  public:
    /**
     * Destructor.
     */
    virtual ~Mean() {};

    /**
     * Returns the mean of EHSS results.
     * \return the mean of EHSS results, or 0 is !isEHSSSaved().
     */
    virtual double getMeanEHSS() = 0;

    /**
     * Returns the mean of PA results.
     * \return the mean of PA results, or 0 is !isPASaved().
     */
    virtual double getMeanPA() = 0;

    /**
     * Returns the mean of TM results.
     * \return the mean of TM results, or 0 is !isTMSaved().
     */
    virtual double getMeanTM() = 0;

    /**
     * Returns the mean of the structural asymmetry parameters.
     * \return the mean of the structural asymmetry parameters.
     */
    virtual double getMeanStructAsymParam() = 0;

    /**
     * Returns the mean of the standard deviations.
     * \return the mean of the standard deviation.
     */
    virtual double getMeanStandardDeviation() = 0;

    /**
     * Returns the mean of the numbers of failed trajectories.
     * \return the mean of the numbers of failed trajectories.
     */
    virtual int getMeanNumberOfFailedTrajectories() = 0;

    /**
     * \return true if EHSS was saved, false in the other case.
     */
    virtual bool isEHSSSaved() = 0;

    /**
     * \return true if PA was saved, false in the other case.
     */
    virtual bool isPASaved() = 0;

    /**
     * \return true if TM was saved, false in the other case.
     */
    virtual bool isTMSaved() = 0;

    /**
     * Indicates if EHSS needs to be printed.
     * \return true if EHSS needs to be printed, false otherwise.
     */
    virtual bool isEHSSPrintable() = 0;

    /**
     * Indicates if PA needs to be printed.
     * \return true if PA needs to be printed, false otherwise.
     */
    virtual bool isPAPrintable() = 0;

    /**
     * Indicates if TM needs to be printed.
     * \return true if TM needs to be printed, false otherwise.
     */
    virtual bool isTMPrintable() = 0;

    /**
     * Add a result to the results used to calculate the means.
     * \param r the result to add to the list.
     */
    virtual void addResult(Result* r) = 0;

    /**
     * Write the mean object via the FileWriter.
     */
    virtual void accept(class FileWriter& fileWriter) = 0;
};

#endif
