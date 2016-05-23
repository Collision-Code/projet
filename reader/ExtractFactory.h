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
 * \file ExtractFactory.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Interface describing a factory permitting to return the good FileReader to read a certain type of file.
 */

#ifndef EXTRACTFACTORY_H
#define EXTRACTFACTORY_H

#include <string>

#include "FileReader.h"

class ExtractFactory {
  public:
    /**
     * Releases allocated resources.
     */
    virtual ~ExtractFactory() {};

    /**
     * Returns the FileReader necessary to read the file.
     * \param fileName the file name.
     * \return a pointer to a FileReader which can read the file,
     * or null if the file can't be read.
     */
    virtual FileReader* getReader(std::string fileName) = 0;
};

#endif
