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
