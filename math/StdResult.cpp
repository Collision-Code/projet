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

#include "StdResult.h"

StdResult::StdResult(Molecule* mol)
  : m_mol(mol), m_ehssResult(0.0), m_ehssSaved(false),
    m_ehssPrinted(true), m_paResult(0.0),
    m_paSaved(false), m_paPrinted(true), m_tmResult(0.0),
    m_tmSaved(false), m_tmPrinted(true), m_asymParam(0.0),
    m_stdDeviation(0.0), m_nbFailedTraject(0)
{

}

StdResult::~StdResult()
{

}

void StdResult::accept(FileWriter& fileWriter)
{
  fileWriter.visitResult(this);
}
