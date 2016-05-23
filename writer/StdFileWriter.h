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
 * \file StdFileWriter.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way of write results and means of results in a stream.
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
     * Writes a result in a stream.
     */
    void visitResult(Result* result);

    /**
     * Writes a mean of results in a file.
     */
    void visitMean(Mean* mean);

  private:
    /**
     * The stream in which the FileWriter can write.
     */
    std::ostream& m_stream;
};

#endif
