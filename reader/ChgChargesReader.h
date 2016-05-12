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

#ifndef __CHGCHARGESREADER_H
#define __CHGCHARGESREADER_H

#include "ChargesReader.h"

class ChgChargesReader : public ChargesReader {
  public:
    // BUILDERS
    /**
     * ChgFileReader's constructor.
     * \param filename the name of file to work with.
     */
    ChgChargesReader(std::string filename);
    // DESTRUCTORS
    virtual ~ChgChargesReader();
    // REQUESTS
    /**
     * Return name of file onload.
     * \return a string value giving the complete file name.
     */
    std::string getFileName() const {return m_filename;}
    // COMMANDS
    /**
     * Change the actual file by a new one.
     */
    void setFileName(std::string filename);
    /**
     * Return all molecule from the actual file.
     * \return a pointer to a molecule vector extract from file.
     */
    std::vector<Molecule*>* loadResources(std::vector<Molecule*>* molGeometries);
  private:
    /**
     * Name of file to work with.
     */
    std::string m_filename;
};

#endif // __CHGCHARGESREADER_H
