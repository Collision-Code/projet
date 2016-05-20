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
     * Write a result in a file.
     */
    virtual void visitResult(Result* result) = 0;

    /**
     * Write a mean of results in a file.
     */
    virtual void visitMean(Mean* mean) = 0;
};

#endif
