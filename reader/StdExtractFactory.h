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

#ifndef STDEXTRACTFACTORY_H
#define STDEXTRACTFACTORY_H

#include "ExtractFactory.h"

class StdExtractFactory : public ExtractFactory
{
  public:
    /**
     * Creates a new StdExtractFactory.
     */
    StdExtractFactory();

    /**
     * Releases allocated resources.
     */
    virtual ~StdExtractFactory();

    /**
     * Returns the FileReader necessary to read the file.
     * \param fileName the file name.
     * \return a pointer to a FileReader which can read the file,
     * or null if the file can't be read.
     */
    FileReader* getReader(std::string fileName);
};

#endif
