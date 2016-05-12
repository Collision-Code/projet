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
 * \file AtomInformations.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 11 mars 2016
 * \brief Class implementing a singleton to access data on atoms.
 * \details Data contains atomic number, mass, hard sphere radius, parameters of Lennard-Jones and color.
 */

#ifndef ATOMINFORMATIONS_H
#define ATOMINFORMATIONS_H

#include <string>
#include <map>
#include <vector>

class AtomInformations
{
  public:
    /**
     * \return an instance of AtomInformations to work with.
     */
    static AtomInformations* const getInstance()
    {
      return m_instance;
    }

    /**
     * Destructor.
     */
    virtual ~AtomInformations();

    /**
     * Load data from file.
     * \param fileName the name of the file to load.
     */
    void loadFile(std::string fileName);

    /**
     * Tests if a symbol exists.
     * \param symb the symbol to test.
     * \return true if symbol exists in data.
     */
    bool isExistingSymbol(std::string symb) const
    {
      return m_atomInfos.find(symb) != m_atomInfos.end();
    }

    /**
     * Search for the symbol having the integer part of its mass
     * equals to atomicMass.
     * \param atomicMass the mass to search for.
     * \return the symbol corresponding to atomicMass.
     */
    std::string getSymbol(int atomicMass);

    /**
     * \param symb the symbol of the atom to search atomic number for.
     * \return the atomic number of the atom of symbol symb.
     */
    int getAtomicNumber(std::string symb) const;

    /**
     * \param symb the symbol of the atom to search mass for.
     * \return the mass of the atom of symbol symb.
     */
    double getAtomicMass(std::string symb);

    /**
     * \param symb the symbol of the atom to search mass for.
     * \return EOLJ for Helium of the atom of symbol symb.
     */
    double getEOLJHe(std::string symb);

    /**
     * \param symb the symbol of the atom to search mass for.
     * \return ROLJ for Helium of the atom of symbol symb.
     */
    double getROLJHe(std::string symb);

    /**
     * \param symb the symbol of the atom to search hard sphere radius for.
     * \return the hard sphere radius of the atom of symbol symb in meter.
     */
    double getHSRadius(std::string symb);

  private:
    /**
     * Enumeration representing the different columns in the file,
     * in order.
     * COLUMN_NUMBER automatically contains the number of columns
     * of the file.
     */
    enum {
        ATOMIC_NUMBER = 0,
        ATOMIC_MASS,
        HARD_SPHERE_RADIUS,
        EOLJ_He,
        ROLJ_He,
        EOLJ_N2,
        ROLJ_N2,
        EOLJ_UFF,
        EOLJ_UFF_He,
        ROLJ_UFF_He,
        EOLJ_UFF_N2,
        ROLJ_UFF_N2,
        COLOR,

        COLUMN_NUMBER
    };

  private:
    /**
     * Constructor.
     */
    AtomInformations();

  private:
    /**
     * Static instance of AtomInformations to work with.
     */
    static AtomInformations* m_instance;

    /**
     * Map containing all the infos for atoms.
     * To a symbol is associating string values corresponding
     * to the columns of the file loaded by calling loadFile().
     */
    std::map<std::string, std::vector<std::string>> m_atomInfos;

    /**
     * Map containing atomic masses associated with symbols.
     */
    std::map<std::string, float> m_massesValues;

    /**
     * Map containing EOLJ for Helium associated with symbols.
     */
    std::map<std::string, float> m_EOLJHeValues;

    /**
     * Map containing ROLJ for Helium associated with symbols.
     */
    std::map<std::string, float> m_ROLJHeValues;

    /**
     * Map containing RHS values associated with symbols.
     */
    std::map<std::string, float> m_rhsValues;
};

#endif // ATOMINFORMATIONS_H
