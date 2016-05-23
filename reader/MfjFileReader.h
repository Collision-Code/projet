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
 * \file MfjFileReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way of loading geometries files from .mfj files.
 */

#ifndef __MFJFILEREADER_H
#define __MFJFILEREADER_H

#include "FileReader.h"

class MfjFileReader : public FileReader {
  public:
    /**
     * MfjFileReader's constructor.
     * \param filename the name of file to work with.
     */
    MfjFileReader(std::string filename);

    /**
     * Destructor.
     */
    virtual ~MfjFileReader();

    /**
     * Returns name of file on load.
     * \return a string value giving the complete file name.
     */
    std::string getFileName() const {return m_filename;}

    /**
     * Change the actual file by a new one.
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

