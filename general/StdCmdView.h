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
 * \file StdCmdView.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Class implementing the interface CmdView.h
 * \details A facade which oversees method calls.
 */

#ifndef STDCMDVIEW_H
#define STDCMDVIEW_H

#include "CmdView.h"

#include "GeometryCalculator.h"
#include "../reader/ExtractResources.h"

#include <vector>

class StdCmdView : public CmdView
{
  public:
    /**
     * Creates a new StdCmdView.
     * By default, EHSS, PA and TM will be calculated.
     */
    StdCmdView();

    /**
     * Releases all allocated resources.
     */
    virtual ~StdCmdView();

    /**
     * Indicates if yes or no, EHSS should be calculated.
     * \param b true if EHSS should be calculated, else otherwise.
     */
    void shouldEHSSBeCalculated(bool b) {m_calculator->shouldEHSSBeCalculated(b);}

    /**
     * Indicates if yes or no, PA should be calculated.
     * \param b true if PA should be calculated, else otherwise.
     */
    void shouldPABeCalculated(bool b) {m_calculator->shouldPABeCalculated(b);}

    /**
     * Indicates if yes or no, TM should be calculated.
     * \param b true if TM should be calculated, else otherwise.
     */
    void shouldTMBeCalculated(bool b) {m_calculator->shouldTMBeCalculated(b);}

    /**
     * Indicates a new file to load.
     * \param fileName the name of the file to load.
     */
    void addInputFile(std::string fileName);

    /**
     * Remove a file from the vector of the file to load.
     * \param fileName the name of the file to remove of the vector of
     * the file to load.
     */
    void removeInputFile(std::string fileName);

    /**
     * Indicates the charge file name.
     * \param chargeFileName the name of the charge file.
     */
    void setChargeFile(std::string chargeFileName) {m_chargeFile = chargeFileName;}

    /**
     * Indicate the name of the file where results will be written.
     * Delete previous content of the file.
     * \param fileName the name of the output file.
     */
    void setOutputFile(std::string fileName) {m_outputFile = fileName;}

    /**
     * Launches all the calculations, on all input files.
     * Write the results in the output file.
     */
    void launch();

  private:
    /**
     * A calculator for the CCS.
     */
    GeometryCalculator* m_calculator;

    /**
     * The vector containing all the files in input.
     */
    std::vector<std::string> m_inputFiles;

    /**
     * The output file.
     */
    std::string m_outputFile;

    /**
     * The charge file.
     */
    std::string m_chargeFile;
};

#endif
