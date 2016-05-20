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

#include <iostream>

class StdFileWriter : public FileWriter {
  public:
    /**
     * Creates a new StdFileWriter.
     */
    StdFileWriter(std::ostream& stream);

    /**
     * Releases all allocated resources.
     */
    virtual ~StdFileWriter();

    /**
     * Write a result in a stream.
     */
    void visitResult(Result* result);

    /**
     * Write a mean of results in a file.
     */
    void visitMean(Mean* mean);

  private:
    /**
     * The stream in which the FileWriter can write.
     */
    std::ostream& m_stream;
};

#endif
