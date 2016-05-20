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
#include "../observer/Observable.h"

#include <map>

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
     * \return the list of input files.
     */
    std::vector<std::string> getInputFiles() const {
      return m_inputFiles;
    }

    /**
     * \return all loaded geometries.
     */
    std::vector<Molecule*> getLoadedGeometries() const {
      return m_geometries;
    }

    /**
     * \return true if EHSS should be calculated, else otherwise.
     */
    bool willEHSSBeCalculated() const {
      return m_calculator->willEHSSBeCalculated();
    }

    /**
     * \return true if PA should be calculated, else otherwise.
     */
    bool willPABeCalculated() const {
      return m_calculator->willPABeCalculated();
    }

    /**
     * \return true if TM should be calculated, else otherwise.
     */
    bool willTMBeCalculated() const {
      return m_calculator->willTMBeCalculated();
    }

    /**
     * Indicates if yes or no, EHSS should be calculated.
     * \param b true if EHSS should be calculated, else otherwise.
     */
    void shouldEHSSBeCalculated(bool b) {
      m_calculator->shouldEHSSBeCalculated(b);
    }

    /**
     * Indicates if yes or no, PA should be calculated.
     * \param b true if PA should be calculated, else otherwise.
     */
    void shouldPABeCalculated(bool b) {
      m_calculator->shouldPABeCalculated(b);
    }

    /**
     * Indicates if yes or no, TM should be calculated.
     * \param b true if TM should be calculated, else otherwise.
     */
    void shouldTMBeCalculated(bool b) {
      m_calculator->shouldTMBeCalculated(b);
    }

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
     * Loads all saved input files with charge file if present.
     * \return the number of geometries loaded.
     */
    int loadInputFiles();

    /**
     * \return the name of the charge file.
     */
    std::string getChargeFile() const {
      return m_chargeFile;
    }

    /**
     * Indicates the charge file name.
     * \param chargeFileName the name of the charge file.
     */
    void setChargeFile(std::string chargeFileName) {
      m_chargeFile = chargeFileName;
    }

    /**
     * \return a string representing the content of the calculations save.
     */
    std::string getResultFormat() const;

    /**
     * Sets the output file.
     * \param outputFileName the file name of the output file.
     */
    void setOutputFile(std::string outputFileName) {
      m_outputFile = outputFileName;
    }

    /**
     * \return the file name of the output file.
     */
    std::string getOutputFile() const {
      return m_outputFile;
    }

    /**
     * Save the results in the file named fileName.
     * Delete previous content of the file.
     * \param fileName the name of the output file.
     */
    void saveResults();

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
     * The charge file.
     */
    std::string m_chargeFile;

    /**
     * The output file.
     */
    std::string m_outputFile;

    /**
     * Geometries to work with.
     */
    std::vector<Molecule*> m_geometries;

    /**
     * A map associating geometries with the name of the file where they come from.
     */
    std::map<Molecule*, std::string> m_geometriesFromFile;
};

#endif
