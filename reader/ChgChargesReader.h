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
 * \file ChgChargesReader.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a way to read and charge charges from a .chg file.
 */

#ifndef __CHGCHARGESREADER_H
#define __CHGCHARGESREADER_H

#include "ChargesReader.h"

class ChgChargesReader : public ChargesReader {
  public:
    /**
     * Constructor.
     * \param filename the name of file to work with.
     */
    ChgChargesReader(std::string filename);

    /**
     * Destructor.
     */
    virtual ~ChgChargesReader();

    /**
     * Return name of file onload.
     * \return a string value giving the complete file name.
     */
    std::string getFileName() const {return m_filename;}

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

#endif
