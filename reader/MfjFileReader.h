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

#ifndef __MFJFILEREADER_H
#define __MFJFILEREADER_H

#include "FileReader.h"

class MfjFileReader : public FileReader {
  public:
    // BUILDERS
    /**
     * MfjFileReader's constructor.
     * \param filename the name of file to work with.
     */
    MfjFileReader(std::string filename);
    // DESTRUCTORS
    virtual ~MfjFileReader();
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

#endif // __MFJFILEREADER_H

