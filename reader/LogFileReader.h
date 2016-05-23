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
 * \file LogFileReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way of loading geometries files from .log/.out files.
 */

#ifndef __LOGFILEREADER_H
#define __LOGFILEREADER_H

#include "FileReader.h"

class LogFileReader : public FileReader {
  public:
    /**
     * MolFileReader's cosntructor.
     * \param filename the name of file to work with.
     */
    LogFileReader(std::string filename);

    /**
     * Destructor.
     */
    virtual ~LogFileReader();

    /**
     * Returns name of file on load.
     * \return a string value giving the complete file name.
     */
    std::string getFileName() const {return m_filename;}

    /**
     * Changes the actual file by a new one.
     */
    void setFileName(std::string filename);

    /**
     * Returns all molecule from the actual file.
     * \return a pointer to a molecule list extract from file.
     */
    std::vector<Molecule*>* loadResources();

  private:
    /**
     * Name of file to work with.
     */
    std::string m_filename;

  private:
    /**
     * Searches in input line string if number of atoms is specified.
     * \return an int representing the number of atoms.
     */
    int isThereAtomNumber(std::string line);

    /**
     * Searches name from pop option.
     * \return a string name of option.
     */
    std::string findPopOption(std::string commandeLine);

    /**
     * Loads at current file charges.
     */
    void chargeLoading(std::string isPopOption, std::ifstream &file,
        std::string commandLine, Molecule *newMol);
};

#endif
