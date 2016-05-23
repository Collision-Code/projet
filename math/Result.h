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
 * \file Result.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief A interface describing how to save the results of cross-section calculations.
 */

#ifndef RESULT_H
#define RESULT_H

#include "../molecule/Molecule.h"

/**
 * Interface describing how to save results.
 */
class Result {
  public:
    virtual ~Result() {};

    /**
     * Returns the molecule saves with these results.
     * \return the molecule saves with these results.
     */
    virtual Molecule* getAssociateMolecule() = 0;

    /**
     * Returns the result for EHSS.
     * \return the EHSS result, or 0 is !isEHSSSaved().
     */
    virtual double getEHSS() = 0;

    /**
     * Returns the result for PA.
     * \return the PA result, or 0 is !isPASaved().
     */
    virtual double getPA() = 0;

    /**
     * Returns the result for TM.
     * \return the TM result, or 0 is !isTMSaved().
     */
    virtual double getTM() = 0;

    /**
     * Returns the structural asymmetry parameter.
     * \return the structural asymmetry parameter.
     */
    virtual double getStructAsymParam() = 0;

    /**
     * Returns the standard deviation.
     * \return the standard deviation.
     */
    virtual double getStandardDeviation() = 0;

    /**
     * Returns the number of failed trajectories.
     * \return the number of failed trajectories.
     */
    virtual int getNumberOfFailedTrajectories() = 0;

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
     * Sets the value of the EHSS result to ehss.
     * Sets isEHSSSaved() to true.
     * \param ehss the value of the EHSS result.
     */
    virtual void setEHSS(double ehss) = 0;

    /**
     * Sets the value of the PA result to pa.
     * Sets isPASaved() to true.
     * \param pa the value of the PA result.
     */
    virtual void setPA(double pa) = 0;

    /**
     * Sets the value of the TM result to tm.
     * Sets isTMSaved() to true.
     * \param tm the value of the TM result.
     */
    virtual void setTM(double tm) = 0;

    /**
     * Sets the value of the structural asymmetry parameter to asymParam.
     * \param asymParam the value of the structural asymmetry parameter.
     */
    virtual void setStructAsymParam(double asymParam) = 0;

    /**
     * Returns the standard deviation.
     * \param stdDeviation the standard deviation.
     */
    virtual void setStandardDeviation(double stdDeviation) = 0;

    /**
     * Returns the number of failed trajectories.
     * \param nbFailedTraject the number of failed trajectories.
     */
    virtual void setNumberOfFailedTrajectories(int nbFailedTraject) = 0;

    /**
     * Indicates if EHSS needs to be printed.
     * \param true if EHSS needs to be printed, false otherwise.
     */
    virtual void EHSSNeedsToBePrinted(bool b) = 0;

    /**
     * Indicates if PA needs to be printed.
     * \param true if PA needs to be printed, false otherwise.
     */
    virtual void PANeedsToBePrinted(bool b) = 0;

    /**
     * Indicates if TM needs to be printed.
     * \param true if TM needs to be printed, false otherwise.
     */
    virtual void TMNeedsToBePrinted(bool b) = 0;

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
     * Write the result via the FileWriter.
     */
    virtual void accept(class FileWriter& fileWriter) = 0;
};

#endif
