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

#include "../observer/Observer.h"
#include "../molecule/Molecule.h"

#include <string>
#include <vector>

class CmdView : public Observable {
  public:
    /**
     * Releases all allocated resources.
     */
    virtual ~CmdView() {};

    /**
     * \return the list of input files.
     */
    virtual std::vector<std::string> getInputFiles() const = 0;

    /**
     * \return all loaded geometries.
     */
    virtual std::vector<Molecule*> getLoadedGeometries() const = 0;

    /**
     * \return true if EHSS should be calculated, else otherwise.
     */
    virtual bool willEHSSBeCalculated() const = 0;

    /**
     * \return true if PA should be calculated, else otherwise.
     */
    virtual bool willPABeCalculated() const = 0;

    /**
     * \return true if TM should be calculated, else otherwise.
     */
    virtual bool willTMBeCalculated() const = 0;

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
     * Loads all saved input files with charge file if present.
     * \return the number of geometries loaded.
     */
    virtual int loadInputFiles() = 0;

    /**
     * \return the name of the charge file.
     */
    virtual std::string getChargeFile() const = 0;

    /**
     * Indicates the charge file name.
     * \param chargeFileName the name of the charge file.
     */
    virtual void setChargeFile(std::string chargeFileName) = 0;

    /**
     * \return a string representing the content of the calculations save.
     */
    virtual std::string getResultFormat() const = 0;

    /**
     * Sets the output file.
     * \param outputFileName the file name of the output file.
     */
    virtual void setOutputFile(std::string outputFileName) = 0;

    /**
     * \return the file name of the output file.
     */
    virtual std::string getOutputFile() const = 0;

    /**
     * Save the results in the output file.
     * Delete previous content of the file.
     */
    virtual void saveResults() = 0;

    /**
     * Launches all the calculations, on all input files.
     * Write the results in the output file.
     */
    virtual void launch() = 0;
};

#endif
