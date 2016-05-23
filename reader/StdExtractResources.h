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
 * \file StdExtractResources.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way of read geometries from a file.
 */

#ifndef STDEXTRACTRESOURCES_H
#define STDEXTRACTRESOURCES_H

#include "ExtractResources.h"
#include "ExtractFactory.h"

class StdExtractResources : public ExtractResources
{
  public:
    /**
     * Creates a new StdExtractResources.
     */
    StdExtractResources();

    /**
     * Releases allocated resources.
     */
    virtual ~StdExtractResources();

    /**
     * Returns a vector of molecules loaded from the file.
     * \param fileName the name of the file in which are the molecules.
     * \return a pointer to a vector containing the loaded molecules,
     * or null if the file can't be loaded.
     */
    std::vector<Molecule*>* getGeometriesFromFile(std::string fileName);

  private:
    /**
     * The factory to recuperate the FileReader.
     */
    ExtractFactory* m_extractFactory;
};

#endif
