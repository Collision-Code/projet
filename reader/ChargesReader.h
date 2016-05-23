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
 * \file ChargesReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Interface describing a way to read and charge charges from a .chg file.
 */

#ifndef __CHARGESREADER_H
#define __CHARGESREADER_H

#include "../molecule/Molecule.h"

#include <vector>
#include <string>

class ChargesReader {
  public:
    /**
     * Destructor.
     */
    virtual ~ChargesReader() {};

    /**
     * Return name of file onload.
     * \return a string value giving the complete file name.
     */
    virtual std::string getFileName() const = 0;

    /**
     * Change the actual file by a new one.
     */
    virtual void setFileName(std::string f) = 0;

    /**
     * Return all molecule from the actual file.
     * \return a pointer to a molecule vector extract from file.
     */
    virtual std::vector<Molecule*>* loadResources(std::vector<Molecule*>* molGeometries) = 0;
};

#endif
