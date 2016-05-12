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

#ifndef STDFILEWRITER_H
#define STDFILEWRITER_H

#include "FileWriter.h"

#include <fstream>

class StdFileWriter : public FileWriter {
  public:
    /**
     * Creates a new StdFileWriter.
     */
    StdFileWriter(std::ofstream& file);

    /**
     * Releases all allocated resources.
     */
    virtual ~StdFileWriter();

    /**
     * Write a result in a file.
     */
    void visitResult(Result* result);

  private:
    /**
     * The stream in which the FileWriter can write.
     */
    std::ofstream& m_file;
};

#endif
