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
 * \file ConsoleView.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Describes the console view and parses the arguments from the command line.
 */

#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "../observer/Observer.h"
#include "../general/CmdView.h"


class ConsoleView : public Observer
{
  public:
    /**
     * Constructor. Take the main command line in parameter.
     */
    ConsoleView(int argc, char* const argv[]);

    /**
     * Destructor.
     */
    virtual ~ConsoleView();

    /**
     * \return true if there is an error with the command line, false otherwise.
     */
    bool isThereAnError() const {
      return m_error;
    }

    /**
     * Updates the observer.
     * \param cond the condition that triggered the notification.
     * \param obs the Observable which triggered the call. May be null.
     */
    void update(ObservableEvent cond, Observable* obs);

    /**
     * Launches calculations.
     */
    void launch();

  private:
    /**
     * Parse the command line.
     * Take the command line in parameter.
     */
    void parse(int argc, char* const argv[]);

    /**
     * Prints the help.
     * \param progName the name of the program.
     */
    void printHelp(std::string progName);

    /**
     * Prints an error.
     * \param progName the program name.
     * \param error the error to print.
     */
    void printError(std::string progName, std::string error);

    /**
     * Converts a string to double.
     * \param s the string to convert.
     * \return the conversion of s into double.
     */
    double convertToDouble(const std::string& s);

    /**
     * Converts a string to an integer.
     * \param s the string to convert.
     * \return the conversion of s into integer.
     */
    int convertToInteger(const std::string& s);

  private:
    /**
     * The CmdView instance to work with.
     */
    CmdView* m_cmdView;

    /**
     * The date file.
     * Default value : "resources/atomInformations.csv".
     */
    std::string m_dataFile;

    /**
     * The output file.
     * Default value : "resCollision.ccout".
     */
    std::string m_outFile;

    /**
     * Indicates an error.
     */
    bool m_error;

    /**
     * Counts the number of geometries for whose the calculations are finished.
     */
    unsigned int m_geometriesFinished;

    /**
     * The total number of geometries to work with.
     */
    unsigned int m_totalNumberGeometries;

    /**
     * Indicates if we want text updates in console.
     */
    bool m_verbose;
};

#endif
