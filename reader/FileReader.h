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
 * \file FileReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief An interface describing a way of loading geometries files.
 */

#ifndef __FILEREADER_H
#define __FILEREADER_H

#include "../molecule/Molecule.h"

#include <vector>
#include <string>

class FileReader {
  public:
    /**
     * Destructor.
     */
    virtual ~FileReader() {};

    /**
     * Returns name of file on load.
     * \return a string value giving the complete file name.
     */
    virtual std::string getFileName() const = 0;

    /**
     * Changes the actual file by a new one.
     */
    virtual void setFileName(std::string f) = 0;

    /**
     * Returns all molecule from the actual file.
     * \return a pointer to a molecule vector extract from file.
     */
    virtual std::vector<Molecule*>* loadResources() = 0;
};

#endif
