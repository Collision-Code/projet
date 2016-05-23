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
 * \file ExtractResources.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Interface describing a way of read geometries from a file.
 */

#ifndef EXTRACTRESOURCES_H
#define EXTRACTRESOURCES_H

#include <vector>
#include <string>

#include "../molecule/Molecule.h"

class ExtractResources {
  public:
    /**
     * Releases allocated resources.
     */
    virtual ~ExtractResources() {};

    /**
     * Returns a vector of molecules loaded from the file.
     * \param fileName the name of the file in which are the molecules.
     * \return a pointer to a vector containing the loaded molecules,
     * or null if the file can't be loaded.
     */
    virtual std::vector<Molecule*>* getGeometriesFromFile(std::string fileName) = 0;
};

#endif
