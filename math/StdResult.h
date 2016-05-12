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

#ifndef STDRESULT_H
#define STDRESULT_H

#include "Result.h"
#include "../molecule/Molecule.h"

#include "../writer/FileWriter.h"

class StdResult : public Result
{
  public:
    /**
     * StdResult's constructor.
     * \param mol the molecule relative to the results.
     */
    StdResult(Molecule* mol);

    virtual ~StdResult();

    /**
     * Returns the molecule saves with these results.
     * \return the molecule saves with these results.
     */
    Molecule* getAssociateMolecule() {return m_mol;}

    /**
     * Returns the result for EHSS.
     * \return the EHSS result, or 0 is !isEHSSSaved().
     */
    double getEHSS() {return m_ehssResult;}

    /**
     * Returns the result for PA.
     * \return the PA result, or 0 is !isPASaved().
     */
    double getPA() {return m_paResult;}

    /**
     * Returns the result for TM.
     * \return the TM result, or 0 is !isTMSaved().
     */
    double getTM() {return m_tmResult;}

    /**
     * Returns the structural asymmetry parameter.
     * \return the structural asymmetry parameter.
     */
    double getStructAsymParam() {return m_asymParam;}

    /**
     * Returns the standard deviation.
     * \return the standard deviation.
     */
    double getStandardDeviation() {return m_stdDeviation;}

    /**
     * Returns the number of failed trajectories.
     * \return the number of failed trajectories.
     */
    int getNumberOfFailedTrajectories() {return m_nbFailedTraject;}

    /**
     * \return true if EHSS was saved, false in the other case.
     */
    bool isEHSSSaved() {return m_ehssSaved;}

    /**
     * \return true if PA was saved, false in the other case.
     */
    bool isPASaved() {return m_paSaved;}

    /**
     * \return true if TM was saved, false in the other case.
     */
    bool isTMSaved() {return m_tmSaved;}

    /**
     * Sets the value of the EHSS result to ehss.
     * Sets isEHSSSaved() to true.
     * \param ehss the value of the EHSS result.
     */
    void setEHSS(double ehss)
    {
      m_ehssResult = ehss;
      m_ehssSaved = true;
    }

    /**
     * Sets the value of the PA result to pa.
     * Sets isPASaved() to true.
     * \param pa the value of the PA result.
     */
    void setPA(double pa)
    {
      m_paResult = pa;
      m_paSaved = true;
    }

    /**
     * Sets the value of the TM result to tm.
     * Sets isTMSaved() to true.
     * \param tm the value of the TM result.
     */
    void setTM(double tm) {
      m_tmResult = tm;
      m_tmSaved = true;
    }

    /**
     * Sets the value of the structural asymmetry parameter to asymParam.
     * \param asymParam the value of the structural asymmetry parameter.
     */
    void setStructAsymParam(double asymParam) {
      m_asymParam = asymParam;
    }

    /**
     * Returns the standard deviation.
     * \param stdDeviation the standard deviation.
     */
    void setStandardDeviation(double stdDeviation) {
      m_stdDeviation = stdDeviation;
    }

    /**
     * Returns the number of failed trajectories.
     * \param nbFailedTraject the number of failed trajectories.
     */
    void setNumberOfFailedTrajectories(int nbFailedTraject) {
      m_nbFailedTraject = nbFailedTraject;
    }

    /**
     * Indicates if EHSS needs to be printed.
     * \param true if EHSS needs to be printed, false otherwise.
     */
    void EHSSNeedsToBePrinted(bool b) {m_ehssPrinted = b;}

    /**
     * Indicates if PA needs to be printed.
     * \param true if PA needs to be printed, false otherwise.
     */
    void PANeedsToBePrinted(bool b) {m_paPrinted = b;}

    /**
     * Indicates if TM needs to be printed.
     * \param true if TM needs to be printed, false otherwise.
     */
    void TMNeedsToBePrinted(bool b) {m_tmPrinted = b;}

    /**
     * Indicates if EHSS needs to be printed.
     * \return true if EHSS needs to be printed, false otherwise.
     */
    bool isEHSSPrintable() {return m_ehssPrinted;}

    /**
     * Indicates if PA needs to be printed.
     * \return true if PA needs to be printed, false otherwise.
     */
    bool isPAPrintable() {return m_paPrinted;}

    /**
     * Indicates if TM needs to be printed.
     * \return true if TM needs to be printed, false otherwise.
     */
    bool isTMPrintable() {return m_tmPrinted;}

    /**
     * Write the result via the FileWriter.
     */
    void accept(FileWriter& fileWriter);

  private:
    /**
     * The molecule associates with the results.
     */
    Molecule* m_mol;

    /**
     * The EHSS result.
     */
    double m_ehssResult;

    /**
     * Indicates if a result was saved for EHSS.
     */
    bool m_ehssSaved;

    /**
     * Indicates if EHSS needs to be printed.
     */
    bool m_ehssPrinted;

    /**
     * The PA result.
     */
    double m_paResult;

    /**
     * Indicates if a result was saved for PA.
     */
    bool m_paSaved;

    /**
     * Indicates if PA needs to be printed.
     */
    bool m_paPrinted;

    /**
     * The TM result.
     */
    double m_tmResult;

    /**
     * Indicates if a result was saved for TM.
     */
    bool m_tmSaved;

    /**
     * Indicates if TM needs to be printed.
     */
    bool m_tmPrinted;

    /**
     * The structural asymmetry parameter.
     */
    double m_asymParam;

    /**
     * The standard deviation.
     */
    double m_stdDeviation;

    /**
     * The number of failed trajectories.
     */
    int m_nbFailedTraject;
};

#endif // STDRESULT_H
