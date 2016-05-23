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
 * \file CCFrame.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements a graphical user interface to use the calculation model.
 */

#ifndef CCFRAME_H
#define CCFRAME_H

#include <QtWidgets>

#include <string>
#include <array>
#include <map>

#include "../general/GlobalParameters.h"
#include "../general/SystemParameters.h"
#include "../general/StdCmdView.h"
#include "../general/AtomInformations.h"
#include "../molecule/StdAtom.h"
#include "../molecule/StdMolecule.h"
#include "../observer/Observer.h"
#include "../observer/Event.h"
#include "../observer/state/CalculationState.h"

/**
 * A worker class to launch calculations on a different thread.
 */
class Worker : public QObject {
  Q_OBJECT

public slots:
  /**
   * Launches the calculations.
   */
  void doWork(StdCmdView *cmd) {
    cmd->launch();
    emit finished();
  }
signals:
  /**
   * Indicates that the calculations are finished.
   */
  void finished();
};

/**
 * A class describing the graphical user interface.
 */
class CCFrame : public QMainWindow, public Observer {
  Q_OBJECT

  public:
    /**
     * Constructor.
     */
    CCFrame();

    /**
     * Destructor.
     */
    virtual ~CCFrame();

    /**
     * Updates the observer.
     * \param cond the condition that triggered the notification.
     * \param obs the Observable which triggered the call. May be null.
     */
    void update(ObservableEvent cond, Observable* obs);

  public slots:
    /**
     * Opens a window to choose a geometries file.
     */
    void openChemicalFile();

    /**
     * Opens a window to choose a charge file.
     */
    void openChargeFile();

    /**
     * Opens a window to choose a modelling file.
     */
    void openAtomInfosFile();

    /**
     * Opens a window to choose a file to save results.
     */
    void saveResults();

    /**
     * Opens a window displaying the "about" text.
     */
    void about();

    /**
     * Indicates to the model if PA should be calculated.
     * \param value true if PA should be calculated, false otherwise.
     */
    void updateModelShouldPABeCalculated(bool value);

    /**
     * Indicates to the model if EHSS should be calculated.
     * \param value true if EHSS should be calculated, false otherwise.
     */
    void updateModelShouldEHSSBeCalculated(bool value);
    /**
     * Indicates to the model if TM should be calculated.
     * \param value true if TM should be calculated, false otherwise.
     */
    void updateModelShouldTMBeCalculated(bool value);

    /**
     * Indicates to the model to launch the calculations.
     */
    void updateModelLaunchCalculation();

    /**
     * Indicates to the model the maximum number of thread for calculations by TM method.
     * \param value the maximum number of threads for calculations by TM method.
     */
    void updateModelMaxNumberThreads(int value);

    /**
     * Indicates to the model the number of points in Monte-Carlo integrations for EHSS and PA methods.
     * \param value the number of points in Monte-Carlo integrations for EHSS and PA methods.
     */
    void updateModelNbPointsMCIntegrationEHSSPA(int value);

    /**
     * Indicates to the model the temperature for calculations.
     * \param value the temperature for calculations.
     */
    void updateModelTemperature(double value);

    /**
     * Indicates to the model the energy conservation threshold.
     * \param value the energy conservation threshold.
     */
    void updateModelEnergyConservationThreshold(double value);

    /**
     * Indicates to the model the number of complete cycles in TM method.
     * \param value the number of complete cycles in TM method.
     */
    void updateModelNbCompleteCycles(int value);

    /**
     * Indicates to the model the number of velocity points in TM method.
     * \param value the number of velocity points in TM method.
     */
    void updateModelNbVelocityPoints(int value);

    /**
     * Indicates to the model the number of points for Monte-Carlo integrations in TM method.
     * \param value the number of points for Monte-Carlo integrations in TM method.
     */
    void updateModelNbPointsMCIntegrationTM(int value);

    /**
     * Indicates to the model the potential energy at the start of a trajectory for TM method.
     * \param value the potential energy at the start of a trajectory for TM method.
     */
    void updateModelPotentialEnergyStart(double value);

    /**
     * Indicates to the model the potential energy when close to a collision for TM method.
     * \param value the potential energy when close to a collision for TM method.
     */
    void updateModelPotentialEnergyCloseCollision(double value);

    /**
     * Indicates to the model the time step between two points at the start of a trajectory for TM method.
     * \param value the time step between two points at the start of a trajectory for TM method.
     */
    void updateModelTimeStepStart(double value);

    /**
     * Indicates to the model the time step between two points when close to a collision for TM method.
     * \param value the time step between two points when close to a collision for TM method.
     */
    void updateModelTimeStepCloseCollision(double value);

    /**
     * Expands or collapses all nodes of the geometries list.
     * \param value true if we need to expand all nodes, false to collapse them.
     */
    void expandAllNodes(bool value);

    /**
     * Updates a result in the geometries list.
     * \param method the method between EHSS, PA and TM.
     * \param index the geometry index.
     * \param value the value of the result.
     */
    void updateResultList(QString method, int index, double value);

    /**
     * Prints results in the correct location.
     */
    void printResults(QString str);

    /**
     * Indicates to the graphical user interface that all calculations are finished.
     */
    void resultsAreReady();

    /**
     * Stop calculation thread and exit the application.
     */
    void killThreadAndExit();

  signals:
    void totalPoints(int value);
    void changeProgressBarValue(int value);
    void changeProgressBarVisibility(bool value);
    void disableWidgets(bool value);
    void changeResults(QString str);
    void resultHasChanged(QString method, int index, double value);
    void callWorkerThread(StdCmdView *cmd);

  private:
    static const int m_HeightItemAndScrollbar;
    static const QString m_DefaultAtomInfosFiles;
    static const std::array<QString, 5> m_ColumnNames;
    static const QString m_PaToStr;
    static const QString m_EhssToStr;
    static const QString m_TmToStr;
    static const std::map<QString, int> m_Methods;
    static const QString m_DefaultOutputFileName;
    static const QString m_NoFile;

  private:
    void createView();
    void placeComponents();
    void createControllers();
    void calculateTotalPoints();
    void writeGeometriesInTreeModel(std::vector<Molecule *> geometries);
    void initializeTreeModel();

  private:
    QThread *m_workerThread;
    Worker *m_worker;

    StdCmdView *m_model;
    double m_progressBarValue;
    int m_geometriesNb;
    double m_lastTmPercentage;

    QAction *m_openChemicalFileAction;
    QAction *m_openChargeFileAction;
    QAction *m_openAtomInformationAction;
    QAction *m_saveAction;
    QAction *m_quitAction;
    QAction *m_aboutAction;
    QAction *m_helpAction;
    QMenu *m_openMenu;

    QProgressBar *m_progressBar;
    QPushButton *m_startCalculation;

    QWidget *m_firstTab;
    QCheckBox *m_pa;
    QCheckBox *m_ehss;
    QCheckBox *m_tm;
    QRadioButton *m_He;
    QRadioButton *m_N2;

    QWidget *m_secondTab;
    QSpinBox *m_threads;
    QSpinBox *m_monteCarloTrajectories;
    QDoubleSpinBox *m_temperature;
    QDoubleSpinBox *m_energy;
    QSpinBox *m_completeCycles;
    QSpinBox *m_velocityPoints;
    QSpinBox *m_randomPoints;
    QSpinBox *m_totalPoints;
    QDoubleSpinBox *m_potentialStartEnergy;
    QDoubleSpinBox *m_potentialEnergyCollision;
    QDoubleSpinBox *m_timeStepAtStart;
    QDoubleSpinBox *m_timeStepCloseToCollision;

    QStringListModel *m_chemicalFilesListModel;
    QStringListModel *m_chargeFilesListModel;
    QStringListModel *m_atomInfosFilesListModel;
    QPushButton *m_chemicalFileOpenButton;
    QPushButton *m_chargeFileOpenButton;
    QPushButton *m_atomInfosFileOpenButton;

    QList<QStandardItem *> m_paResultList;
    QList<QStandardItem *> m_ehssResultList;
    QList<QStandardItem *> m_tmResultList;

    QTextEdit *m_text;
    QPushButton *m_saveResultsButton;

    QTreeView *m_tree;
    QStandardItemModel *m_treeModel;
    QCheckBox *m_expandTree;
};

#endif
