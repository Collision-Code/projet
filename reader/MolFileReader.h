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

#ifndef __MOLFILEREADER_H
#define __MOLFILEREADER_H

#include "FileReader.h"

class MolFileReader : public FileReader {
  public:
    // BUILDERS
    /**
     * MolFileReader's constructor.
     * \param filename the name of file to work with.
     */
    MolFileReader(std::string filename);
    // DESTRUCTORS
    virtual ~MolFileReader();
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
    std::vector<Molecule*>* loadResources();

  private:
    /**
     * Convert a string in double.
     * Throw an exception if not possible.
     */
    double convertToDouble(const std::string& s);

  private:
    /**
     * Name of file to work with.
     */
    std::string m_filename;
};

#endif // __MOLFILEREADER_H
