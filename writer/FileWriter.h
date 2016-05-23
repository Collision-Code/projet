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
 * \file FileWriter.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Interface describing a way of write results and means of results in a stream.
 */

#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "../math/Result.h"
#include "../math/Mean.h"

class FileWriter {
  public:
    /**
     * Releases all allocated resources.
     */
    virtual ~FileWriter() {};

    /**
     * Writes a result in a file.
     */
    virtual void visitResult(Result* result) = 0;

    /**
     * Writes a mean of results in a file.
     */
    virtual void visitMean(Mean* mean) = 0;
};

#endif
