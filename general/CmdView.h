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
 * \file CmdView.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Interface describing the general model.
 * \details A facade which oversees method calls.
 */

#ifndef CMDVIEW_H
#define CMDVIEW_H

#include <string>

class CmdView {
  public:
    /**
     * Releases all allocated resources.
     */
    virtual ~CmdView() {};

    /**
     * Indicates if yes or no, EHSS should be calculated.
     * \param b true if EHSS should be calculated, else otherwise.
     */
    virtual void shouldEHSSBeCalculated(bool b) = 0;

    /**
     * Indicates if yes or no, PA should be calculated.
     * \param b true if PA should be calculated, else otherwise.
     */
    virtual void shouldPABeCalculated(bool b) = 0;

    /**
     * Indicates if yes or no, TM should be calculated.
     * \param b true if TM should be calculated, else otherwise.
     */
    virtual void shouldTMBeCalculated(bool b) = 0;

    /**
     * Indicates a new file to load.
     * \param fileName the name of the file to load.
     */
    virtual void addInputFile(std::string fileName) = 0;

    /**
     * Remove a file from the vector of the file to load.
     * \param fileName the name of the file to remove of the vector of
     * the file to load.
     */
    virtual void removeInputFile(std::string fileName) = 0;

    /**
     * Indicates the charge file name.
     * \param chargeFileName the name of the charge file.
     */
    virtual void setChargeFile(std::string chargeFileName) = 0;

    /**
     * Indicate the name of the file where results will be written.
     * Delete previous content of the file.
     * \param fileName the name of the output file.
     */
    virtual void setOutputFile(std::string fileName) = 0;

    /**
     * Launches all the calculations, on all input files.
     * Write the results in the output file.
     */
    virtual void launch() = 0;
};

#endif
