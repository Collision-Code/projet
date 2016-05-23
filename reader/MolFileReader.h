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
 * \file MolFileReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way of loading geometries files from .mol files.
 */

#ifndef __MOLFILEREADER_H
#define __MOLFILEREADER_H

#include "FileReader.h"

class MolFileReader : public FileReader {
  public:
    /**
     * MolFileReader's constructor.
     * \param filename the name of file to work with.
     */
    MolFileReader(std::string filename);

    /**
     * Destructor.
     */
    virtual ~MolFileReader();

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
     * \return a pointer to a molecule vector extract from file.
     */
    std::vector<Molecule*>* loadResources();

  private:
    /**
     * Converts a string in double.
     * Throws an exception if not possible.
     */
    double convertToDouble(const std::string& s);

  private:
    /**
     * Name of file to work with.
     */
    std::string m_filename;
};

#endif
