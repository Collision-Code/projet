#ifndef CCFRAME_H
#define CCFRAME_H

#include <QtWidgets>

class CCFrame : public QMainWindow {
    Q_OBJECT

    public:
        CCFrame();
        virtual ~CCFrame();

    public slots:
        void openChemicalFile();
        void openChargeFile();
        void openAtomInfosFile();
        void saveResults();
        void about();
        void calculateTotalPoints(int value);
        void expandAllNodes(bool value);

    signals:
        void totalPoints(int value);

    private:
        void createView();
        void placeComponents();
        void createControllers();

        QAction *m_openChemicalFileAction;
        QAction *m_openChargeFileAction;
        QAction *m_openAtomInformationAction;
        QAction *m_saveAction;
        QAction *m_quitAction;
        QAction *m_aboutAction;
        QAction *m_helpAction;

        QString m_saveFile;

        QProgressBar *m_progressBar;
        QPushButton *m_startCalculation;

        QCheckBox *m_pa;
        QCheckBox *m_ehss;
        QCheckBox *m_tm;
        QRadioButton *m_He;
        QRadioButton *m_N2;

        QDoubleSpinBox *m_energy;
        QDoubleSpinBox *m_temperature;
        QSpinBox *m_completeCycles;
        QSpinBox *m_velocityPoints;
        QSpinBox *m_randomPoints;
        QSpinBox *m_totalPoints;
        QSpinBox *m_monteCarloTrajectories;
        QSpinBox *m_threads;
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

        QList<QStandardItem *> paResultList;
        QList<QStandardItem *> ehssResultList;
        QList<QStandardItem *> tmResultList;

        QTextEdit *m_text;
        QCheckBox *m_saveGeometriesInFile;
        QPushButton *m_saveResultsButton;

        QCheckBox *m_expandTree;
};

#endif // CCFRAME_H
