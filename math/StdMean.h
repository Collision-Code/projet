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

#ifndef STDMEAN_H
#define STDMEAN_H

#include "Mean.h"

#include "../writer/FileWriter.h"

#include <vector>


class StdMean : public Mean
{
  public:
    /**
     * Constructor.
     */
    StdMean();

    /**
     * Destructor.
     */
    virtual ~StdMean();

    /**
     * Returns the mean of EHSS results.
     * \return the mean of EHSS results, or 0 is !isEHSSSaved().
     */
    double getMeanEHSS();

    /**
     * Returns the mean of PA results.
     * \return the mean of PA results, or 0 is !isPASaved().
     */
    double getMeanPA();

    /**
     * Returns the mean of TM results.
     * \return the mean of TM results, or 0 is !isTMSaved().
     */
    double getMeanTM();

    /**
     * Returns the mean of the structural asymmetry parameters.
     * \return the mean of the structural asymmetry parameters.
     */
    double getMeanStructAsymParam();

    /**
     * Returns the mean of the standard deviations.
     * \return the mean of the standard deviation.
     */
    double getMeanStandardDeviation();

    /**
     * Returns the mean of the numbers of failed trajectories.
     * \return the mean of the numbers of failed trajectories.
     */
    int getMeanNumberOfFailedTrajectories();

    /**
     * \return true if EHSS was saved, false in the other case.
     */
    bool isEHSSSaved();

    /**
     * \return true if PA was saved, false in the other case.
     */
    bool isPASaved();

    /**
     * \return true if TM was saved, false in the other case.
     */
    bool isTMSaved();

    /**
     * Indicates if EHSS needs to be printed.
     * \return true if EHSS needs to be printed, false otherwise.
     */
    bool isEHSSPrintable();

    /**
     * Indicates if PA needs to be printed.
     * \return true if PA needs to be printed, false otherwise.
     */
    bool isPAPrintable();

    /**
     * Indicates if TM needs to be printed.
     * \return true if TM needs to be printed, false otherwise.
     */
    bool isTMPrintable();

    /**
     * Add a result to the results used to calculate the means.
     * \param r the result to add to the list.
     */
    void addResult(Result* r) {
      m_listResults.push_back(r);
    }

    /**
     * Write the mean object via the FileWriter.
     */
    void accept(class FileWriter& fileWriter);

  private:
    /**
     * The list of results.
     */
    std::vector<Result*> m_listResults;
};

#endif
