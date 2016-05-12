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

#ifndef __CHARGESREADER_H
#define __CHARGESREADER_H

#include "../molecule/Molecule.h"

#include <vector>
#include <string>

class ChargesReader {
  public:
    virtual ~ChargesReader() {};
    // REQUESTS
    /**
     * Return name of file onload.
     * \return a string value giving the complete file name.
     */
    virtual std::string getFileName() const = 0;
    // COMMANDS
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

#endif // __CHARGESREADER_H
