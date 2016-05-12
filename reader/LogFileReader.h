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

#ifndef __LOGFILEREADER_H
#define __LOGFILEREADER_H

#include "FileReader.h"

class LogFileReader : public FileReader {
  public:
    // BUILDERS
    /**
     * MolFileReader's cosntructor.
     * \param filename the name of file to work with.
     */
    LogFileReader(std::string filename);
    // DESTRUCTORS
    virtual ~LogFileReader();
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
     * \return a pointer to a molecule list extract from file.
     */
    std::vector<Molecule*>* loadResources();
  private:
    /**
     * Name of file to work with.
     */
    std::string m_filename;
  private:
    /**
     * Search in input line string if number of atoms is specified.
     * \return an int representing the number of atoms.
     */
    int isThereAtomNumber(std::string line);
    /**
     * Search name from pop option.
     * \return a string name of option.
     */
    std::string findPopOption(std::string commandeLine);
    /**
     * Load at current file charges.
     */
    void chargeLoading(std::string isPopOption, std::ifstream &file,
        std::string commandLine, Molecule *newMol);
};

#endif // __LOGFILEREADER_H
