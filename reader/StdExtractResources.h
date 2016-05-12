/*
 * Collision-Code
 * Free software to calculate collision cross-section with Helium.
 * Universit� de Rouen
 * 2016
 *
 * Anthony BREANT
 * Clement POINSOT
 * Jeremie PANTIN
 * Mohamed TAKHTOUKH
 * Thomas CAPET
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