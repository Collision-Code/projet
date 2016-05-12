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

#ifndef PDBFILEREADER_H
#define PDBFILEREADER_H

#include "FileReader.h"

class PdbFileReader : public FileReader {
  public:
    /**
     * Constructs a new PdbFileReader.
     */
    PdbFileReader(std::string filename);

    /**
     * Frees all resources.
     */
    virtual ~PdbFileReader();

    /**
     * Returns name of file onload.
     * \return a string value giving the complete file name.
     */
    std::string getFileName() const {return m_filename;}

    /**
     * Changes the actual file by a new one.
     * \param filename the name of the file to load.
     */
    void setFileName(std::string filename);

    /**
     * Returns all molecule from the actual file.
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

#endif
