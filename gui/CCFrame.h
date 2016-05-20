#ifndef CCFRAME_H
#define CCFRAME_H

#include <QtWidgets>
#include <QtConcurrent>

#include "../general/GlobalParameters.h"
#include "../general/SystemParameters.h"
#include "../general/StdCmdView.h"
#include "../general/AtomInformations.h"
#include "../molecule/StdAtom.h"
#include "../molecule/StdMolecule.h"
#include "../observer/Observer.h"
#include "../observer/Event.h"
#include "../observer/state/CalculationState.h"

class CCFrame : public QMainWindow, public Observer {
    Q_OBJECT

    public:
        CCFrame();
        virtual ~CCFrame();
        /**
         * Update the observer.
         * \param cond the condition that triggered the notification.
         * \param obs the Observable which triggered the call. May be null.
         */
        void update(ObservableEvent cond, Observable* obs);

    public slots:
        void openChemicalFile();
        void openChargeFile();
        void openAtomInfosFile();
        void saveResults();
        void about();

        void updateModelShouldPABeCalculated(bool value);
        void updateModelShouldEHSSBeCalculated(bool value);
        void updateModelShouldTMBeCalculated(bool value);
        void updateModelLaunchCalculation();

        void updateModelMaxNumberThreads(int value);
        void updateModelNbPointsMCIntegrationEHSSPA(int value);
        void updateModelTemperature(double value);
        void updateModelEnergyConservationThreshold(double value);
        void updateModelNbCompleteCycles(int value);
        void updateModelNbVelocityPoints(int value);
        void updateModelNbPointsMCIntegrationTM(int value);
        void updateModelPotentialEnergyStart(double value);
        void updateModelPotentialEnergyCloseCollision(double value);
        void updateModelTimeStepStart(double value);
        void updateModelTimeStepCloseCollision(double value);

        void expandAllNodes(bool value);
        void updateResultList(QString method, int index, double value);

        void printResults(QString str);
        void resultsAreReady();

    signals:
        void totalPoints(int value);
        void changeProgressBarValue(int value);
        void changeProgressBarVisibility(bool value);
        void changeResults(QString str);
        void resultHasChanged(QString method, int index, double value);

    private:
        void createView();
        void placeComponents();
        void createControllers();
        void calculateTotalPoints();
        void writeGeometriesInTreeModel(std::vector<Molecule *> geometries);

        QFuture<void> m_Future;
        QFutureWatcher<void> *m_FutureWatcher;

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

        QProgressBar *m_progressBar;
        QPushButton *m_startCalculation;

        QCheckBox *m_pa;
        QCheckBox *m_ehss;
        QCheckBox *m_tm;
        QRadioButton *m_He;
        QRadioButton *m_N2;

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

#endif // CCFRAME_H
