#include "CCFrame.h"

const int CCFrame::m_HeightItemAndScrollbar = 38;
const QString CCFrame::m_DefaultAtomInfosFiles = "resources/atomInformations.csv";
const std::array<QString, 5> CCFrame::m_ColumnNames = {"Symbol", "xCoord", "yCoord", "zCoord", "Charge"};
const QString CCFrame::m_PaToStr = "PA";
const QString CCFrame::m_EhssToStr = "EHSS";
const QString CCFrame::m_TmToStr = "TM";
const std::map<QString, int> CCFrame::m_Methods = {
    {CCFrame::m_PaToStr, 1}, {CCFrame::m_EhssToStr, 2}, {CCFrame::m_TmToStr, 3}
};
const QString CCFrame::m_DefaultOutputFileName = "resCollision.ccout";
const QString CCFrame::m_NoFile = "No file";

QString modifyResult(QString method, double d);
void initializeSpinBox(QSpinBox *sb, int min, int max, int step, int value);
void initializeDoubleSpinBox(QDoubleSpinBox *dsb, double min, double max,
                             double step, int decimals, double value);
QString obtainRelativePath(QString filePath);

void launch(StdCmdView *cmd);

// CONSTRUCTEUR
CCFrame::CCFrame() {
    AtomInformations::getInstance()->loadFile(m_DefaultAtomInfosFiles.toStdString());
    m_model = new StdCmdView();
    m_model->addObserver(this);
    createView();
    placeComponents();
    createControllers();
}

// DESTRUCTEUR
CCFrame::~CCFrame() {}

void CCFrame::update(ObservableEvent cond, Observable* obs) {
    CalculationState* cS;
    Molecule *m;
    int i;
    double total = 0.0;
    if (m_pa->isChecked()) total += 20.0;
    if (m_ehss->isChecked()) total += 20.0;
    if (m_tm->isChecked()) total += 60.0;
    if (total != 0) total = 100.0 / total;

    switch(cond) {
    case ObservableEvent::EHSS_STARTED:
        if (m_model->willEHSSBeCalculated()) {
            m_progressBarValue += (5.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::PA_STARTED:
        if (m_model->willPABeCalculated()) {
            m_progressBarValue += (5.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::TM_STARTED:
        if (m_model->willTMBeCalculated()) {
            m_lastTmPercentage = 0.0;
            m_progressBarValue += (5.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::EHSS_ENDED:
        if (m_model->willEHSSBeCalculated()) {
            cS = dynamic_cast<CalculationState*>(obs);
            m = cS->getMolecule();
            i = 0;
            while (m_model->getLoadedGeometries()[i] != m) {
                i++;
            }
            emit resultHasChanged(m_EhssToStr, i, cS->getEHSSResult());
            m_progressBarValue += (15.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::PA_ENDED:
        if (m_model->willPABeCalculated()) {
            cS = dynamic_cast<CalculationState*>(obs);
            m = cS->getMolecule();
            i = 0;
            while (m_model->getLoadedGeometries()[i] != m) {
                i++;
            }
            emit resultHasChanged(m_PaToStr, i, cS->getPAResult());
            m_progressBarValue += (15.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::TM_ENDED:
        if (m_model->willTMBeCalculated()) {
            cS = dynamic_cast<CalculationState*>(obs);
            m = cS->getMolecule();
            i = 0;
            while (m_model->getLoadedGeometries()[i] != m) {
                i++;
            }
            emit resultHasChanged(m_TmToStr, i, cS->getTMResult());
            m_progressBarValue += (5.0 / m_geometriesNb);
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::TRAJECTORY_NUMBER_UPDATE:
        if (m_model->willTMBeCalculated()) {
            cS = dynamic_cast<CalculationState*>(obs);
            m_progressBarValue += (((cS->getPercentageFinishedTrajectories() - m_lastTmPercentage) * 0.5)
                                   / m_geometriesNb);
            m_lastTmPercentage = cS->getPercentageFinishedTrajectories();
            emit changeProgressBarValue(total * m_progressBarValue);
        }
        break;

    case ObservableEvent::CALCULATIONS_FINISHED:
        m_progressBarValue = 0.0;
        emit changeProgressBarValue(100);
        break;

    default:
        break;
    }
}

/**
 * @brief CCFrame::createView
 * Permet d'initialiser les éléments graphiques importants liés au model.
 */
void CCFrame::createView() {
    // Configuration de la fenêtre principale
    setWindowTitle("Collision Code");
    setMinimumSize(600, 500);

    m_worker = new Worker;
    m_workerThread = new QThread;
    m_worker->moveToThread(m_workerThread);

    // Widgets des menus et leurs raccourcis
    m_openChemicalFileAction = new QAction("Chemical &File", this);
    m_openChargeFileAction = new QAction("&Charge File", this);
    m_openAtomInformationAction = new QAction("&Atom Informations File", this);

    m_saveAction = new QAction("&Save", this);
    m_saveAction->setShortcut(QKeySequence("Ctrl+S"));
    m_quitAction = new QAction("&Quit", this);
    m_quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    m_aboutAction = new QAction("&About Collision Code", this);
    m_helpAction = new QAction("&Help", this);

    // Widgets de la barre de status
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);

    m_startCalculation = new QPushButton("Start Calculation!");

    // Widgets de la partie centrale
    // Méthodes utilisées
    m_pa = new QCheckBox(m_PaToStr);
    m_ehss = new QCheckBox(m_EhssToStr);
    m_tm = new QCheckBox(m_TmToStr);
    m_He = new QRadioButton("He");
    m_N2 = new QRadioButton("N2");
    m_pa->setChecked(true);
    m_ehss->setChecked(true);
    m_tm->setChecked(true);
    m_He->setChecked(true);
    m_N2->setEnabled(false);

    // Seuil de conservation d'energie au cours de la méthode TM
    m_energy = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_energy, 90.00, 99.99, 0.01, 2,
            GlobalParameters::getInstance()->getEnergyConservationThreshold());

    m_temperature = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_temperature, 0.0, 1000.0, 0.1, 1,
            GlobalParameters::getInstance()->getTemperature());

    m_completeCycles = new QSpinBox;
    initializeSpinBox(m_completeCycles, 10, 100, 1,
            GlobalParameters::getInstance()->getNumberCompleteCycles());

    m_velocityPoints = new QSpinBox;
    initializeSpinBox(m_velocityPoints, 10, 100, 1,
            GlobalParameters::getInstance()->getNumberVelocityPoints());

    m_randomPoints = new QSpinBox;
    initializeSpinBox(m_randomPoints, 10, 5000, 10,
            GlobalParameters::getInstance()->getNbPointsMCIntegrationTM());

    m_totalPoints = new QSpinBox;
    initializeSpinBox(m_totalPoints,
        m_completeCycles->minimum() * m_velocityPoints->minimum() * m_randomPoints->minimum(),
        m_completeCycles->maximum() * m_velocityPoints->maximum() * m_randomPoints->maximum(),
        0,
        m_completeCycles->value() * m_velocityPoints->value() * m_randomPoints->value());
    m_totalPoints->setEnabled(false);
    m_totalPoints->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_totalPoints->setToolTip("This box is not directly editable.\n"
                              "Its value depends on the values of other fields.");

    m_monteCarloTrajectories = new QSpinBox;
    initializeSpinBox(m_monteCarloTrajectories, 10000, 5000000, 10000,
            GlobalParameters::getInstance()->getNbPointsMCIntegrationEHSSPA());

    m_threads = new QSpinBox;
    initializeSpinBox(m_threads, 1, 10000, 10,
            SystemParameters::getInstance()->getMaximalNumberThreads());

    m_potentialStartEnergy = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_potentialStartEnergy, 0.00001, 0.001, 0.00001, 5,
            GlobalParameters::getInstance()->getPotentialEnergyStart());

    m_potentialEnergyCollision = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_potentialEnergyCollision, 0.001, 0.01, 0.0001, 4,
            GlobalParameters::getInstance()->getPotentialEnergyCloseCollision());

    m_timeStepAtStart = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_timeStepAtStart, 0.1, 1.0, 0.01, 2,
            GlobalParameters::getInstance()->getTimeStepStart());

    m_timeStepCloseToCollision = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_timeStepCloseToCollision, 0.01, 0.1, 0.001, 3,
            GlobalParameters::getInstance()->getTimeStepCloseCollision());


    QStringList noFileList;
    noFileList << m_NoFile;
    QStringList defaultFileList;
    defaultFileList << m_DefaultAtomInfosFiles;
    m_chemicalFilesListModel = new QStringListModel(noFileList);
    m_chargeFilesListModel = new QStringListModel(noFileList);
    m_atomInfosFilesListModel = new QStringListModel(defaultFileList);
    m_chemicalFileOpenButton = new QPushButton("Open");
    m_chargeFileOpenButton = new QPushButton("Open");
    m_atomInfosFileOpenButton = new QPushButton("Open");

    m_text = new QTextEdit;
    m_text->setText(
        "Results will be posted here when calculations will be completed.\n"
        "Results of collision cross sections are available in real time during calculations in \"Geometries\" tab.");
    m_text->setReadOnly(true);
    m_saveResultsButton = new QPushButton("Save in text file");

    m_expandTree = new QCheckBox("Expand all tree nodes");
    m_treeModel = new QStandardItemModel;
    initializeTreeModel();
    m_tree = new QTreeView;
    m_tree->setModel(m_treeModel);
}

/**
 * @brief CCFrame::placeComponents
 * Permet de placer tous les composants graphiques sur l'application.
 */
void CCFrame::placeComponents() {
    // Barre des menus
    QMenu *fileMenu = menuBar()->addMenu("&File");
    m_openMenu = fileMenu->addMenu("&Open");
    QMenu *helpMenu = menuBar()->addMenu("&?");

    m_openMenu->addAction(m_openChemicalFileAction);
    m_openMenu->addAction(m_openChargeFileAction);
    m_openMenu->addAction(m_openAtomInformationAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_quitAction);

    helpMenu->addAction(m_helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(m_aboutAction);


    // Barre de status
    statusBar()->addWidget(m_startCalculation);
    statusBar()->addWidget(m_progressBar);


    // Partie centrale de l'application
    // Table d'onglets
    QTabWidget *tabs = new QTabWidget(this);

    // Onglet de configuration
    m_firstTab = new QWidget;

        QVBoxLayout *vbox = new QVBoxLayout;

            // Affiche le nom du fichier ouvert et propose l'ouverture de fichier.
            QGroupBox *groupbox = new QGroupBox("Opened Files");
                QVBoxLayout *subvbox = new QVBoxLayout;
                    QGroupBox *subgroupbox = new QGroupBox("Chemical File");
                        QHBoxLayout *hbox = new QHBoxLayout;
                            QListView *filesList = new QListView;
                            filesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
                            filesList->setMaximumHeight(m_HeightItemAndScrollbar);
                            filesList->setModel(m_chemicalFilesListModel);
                            hbox->addWidget(filesList);
                            hbox->addWidget(m_chemicalFileOpenButton);
                        subgroupbox->setLayout(hbox);
                    subvbox->addWidget(subgroupbox);
                    subgroupbox = new QGroupBox("Charge File");
                        hbox = new QHBoxLayout;
                            filesList = new QListView;
                            filesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
                            filesList->setMaximumHeight(m_HeightItemAndScrollbar);
                            filesList->setModel(m_chargeFilesListModel);
                            hbox->addWidget(filesList);
                            hbox->addWidget(m_chargeFileOpenButton);
                        subgroupbox->setLayout(hbox);
                    subvbox->addWidget(subgroupbox);
                    subgroupbox = new QGroupBox("Atom Informations File");
                        hbox = new QHBoxLayout;
                            filesList = new QListView;
                            filesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
                            filesList->setMaximumHeight(m_HeightItemAndScrollbar);
                            filesList->setModel(m_atomInfosFilesListModel);
                            hbox->addWidget(filesList);
                            hbox->addWidget(m_atomInfosFileOpenButton);
                        subgroupbox->setLayout(hbox);
                    subvbox->addWidget(subgroupbox);
                groupbox->setLayout(subvbox);
            vbox -> addWidget(groupbox);

            // Un groupe de boutons pour les méthodes à utiliser.
            groupbox = new QGroupBox;
                hbox = new QHBoxLayout;
                    subgroupbox = new QGroupBox("Using methods");
                        QHBoxLayout *subhbox = new QHBoxLayout;
                            subhbox->addWidget(m_pa);
                            subhbox->addWidget(m_ehss);
                            subhbox->addWidget(m_tm);
                        subgroupbox->setLayout(subhbox);
                    hbox->addWidget(subgroupbox);
                    subgroupbox = new QGroupBox("Projectile Molecule");
                        subhbox = new QHBoxLayout;
                            subhbox->addWidget(m_He);
                            subhbox->addWidget(m_N2);
                        subgroupbox->setLayout(subhbox);
                    hbox->addWidget(subgroupbox);
                groupbox->setLayout(hbox);
            vbox->addWidget(groupbox);
        m_firstTab->setLayout(vbox);
    tabs->addTab(m_firstTab, "Configuration");


    // Réglages des paramètres avancés
    m_secondTab = new QWidget;
        vbox = new QVBoxLayout;
            groupbox = new QGroupBox("Threads");
                QFormLayout *form = new QFormLayout;
                form->addRow("Number of threads to use:", m_threads);
                form->setHorizontalSpacing(125);
                groupbox->setLayout(form);
            vbox->addWidget(groupbox);
            groupbox = new QGroupBox("PA && EHSS methods");
                form = new QFormLayout;
                form->addRow("Number of Monte-Carlo trajectories:", m_monteCarloTrajectories);
                form->setHorizontalSpacing(78);
                groupbox->setLayout(form);
            vbox->addWidget(groupbox);
            groupbox = new QGroupBox("TM method");
                form = new QFormLayout;
                form->addRow("Temperature (K):", m_temperature);
                form->addRow("Energy conservation threshold (%):", m_energy);
                form->addRow("Number of complete cycles (itn):", m_completeCycles);
                form->addRow("Number of velocity points (inp):", m_velocityPoints);
                form->addRow("Number of points in Monte-Carlo integrations (imp):", m_randomPoints);
                form->addRow("Total number of points:", m_totalPoints);
                form->addRow("Potential Energy at Start (sw1):", m_potentialStartEnergy);
                form->addRow("Potential energy when close to a collision (sw2):", m_potentialEnergyCollision);
                form->addRow("Time step at start (dtsf1):", m_timeStepAtStart);
                form->addRow("Time step when close to a collision (dtsf2):", m_timeStepCloseToCollision);
                groupbox->setLayout(form);
            vbox->addWidget(groupbox);
        m_secondTab->setLayout(vbox);
    tabs->addTab(m_secondTab, "Advanced Parameters");


    // Onglet d'affichage des données du modèle
    QWidget *tab = new QWidget;
        vbox = new QVBoxLayout;
            vbox->addWidget(m_expandTree);
            vbox->addWidget(m_tree);
        tab->setLayout(vbox);
    tabs->addTab(tab, "Geometries");


    // Onglet d'affichage complets des Résultats
    tab = new QWidget;
        vbox = new QVBoxLayout;
            vbox->addWidget(m_text);
            vbox->addWidget(m_saveResultsButton);
        tab->setLayout(vbox);
    tabs->addTab(tab, "Complete Results");

    setCentralWidget(tabs);
}

void CCFrame::createControllers() {
    QObject::connect(m_openChemicalFileAction, SIGNAL(triggered()), this, SLOT(openChemicalFile()));
    QObject::connect(m_chemicalFileOpenButton, SIGNAL(pressed()), this, SLOT(openChemicalFile()));
    QObject::connect(m_openChargeFileAction, SIGNAL(triggered()), this, SLOT(openChargeFile()));
    QObject::connect(m_chargeFileOpenButton, SIGNAL(pressed()), this, SLOT(openChargeFile()));
    QObject::connect(m_openAtomInformationAction, SIGNAL(triggered()), this, SLOT(openAtomInfosFile()));
    QObject::connect(m_atomInfosFileOpenButton, SIGNAL(pressed()), this, SLOT(openAtomInfosFile()));
    QObject::connect(m_saveAction, SIGNAL(triggered()), this, SLOT(saveResults()));
    QObject::connect(m_saveResultsButton, SIGNAL(pressed()), this, SLOT(saveResults()));
    QObject::connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(killThreadAndExit()));

    QObject::connect(m_pa, SIGNAL(toggled(bool)), this, SLOT(updateModelShouldPABeCalculated(bool)));
    QObject::connect(m_ehss, SIGNAL(toggled(bool)), this, SLOT(updateModelShouldEHSSBeCalculated(bool)));
    QObject::connect(m_tm, SIGNAL(toggled(bool)), this, SLOT(updateModelShouldTMBeCalculated(bool)));
    QObject::connect(m_startCalculation, SIGNAL(pressed()), this, SLOT(updateModelLaunchCalculation()));
    QObject::connect(this, SIGNAL(changeProgressBarValue(int)), m_progressBar, SLOT(setValue(int)));
    QObject::connect(this, SIGNAL(changeProgressBarVisibility(bool)), m_progressBar, SLOT(setVisible(bool)));
    QObject::connect(this, SIGNAL(disableWidgets(bool)), m_startCalculation, SLOT(setVisible(bool)));
    QObject::connect(this, SIGNAL(disableWidgets(bool)), m_openMenu, SLOT(setEnabled(bool)));
    QObject::connect(this, SIGNAL(disableWidgets(bool)), m_firstTab, SLOT(setEnabled(bool)));
    QObject::connect(this, SIGNAL(disableWidgets(bool)), m_secondTab, SLOT(setEnabled(bool)));

    QObject::connect(m_threads, SIGNAL(valueChanged(int)), this, SLOT(updateModelMaxNumberThreads(int)));
    QObject::connect(m_monteCarloTrajectories, SIGNAL(valueChanged(int)), this, SLOT(updateModelNbPointsMCIntegrationEHSSPA(int)));
    QObject::connect(m_temperature, SIGNAL(valueChanged(double)), this, SLOT(updateModelTemperature(double)));
    QObject::connect(m_energy, SIGNAL(valueChanged(double)), this, SLOT(updateModelEnergyConservationThreshold(double)));
    QObject::connect(m_completeCycles, SIGNAL(valueChanged(int)), this, SLOT(updateModelNbCompleteCycles(int)));
    QObject::connect(m_velocityPoints, SIGNAL(valueChanged(int)), this, SLOT(updateModelNbVelocityPoints(int)));
    QObject::connect(m_randomPoints, SIGNAL(valueChanged(int)), this, SLOT(updateModelNbPointsMCIntegrationTM(int)));
    QObject::connect(this, SIGNAL(totalPoints(int)), m_totalPoints, SLOT(setValue(int)));
    QObject::connect(m_potentialStartEnergy, SIGNAL(valueChanged(double)), this, SLOT(updateModelPotentialEnergyStart(double)));
    QObject::connect(m_potentialEnergyCollision, SIGNAL(valueChanged(double)), this, SLOT(updateModelPotentialEnergyCloseCollision(double)));
    QObject::connect(m_timeStepAtStart, SIGNAL(valueChanged(double)), this, SLOT(updateModelTimeStepStart(double)));
    QObject::connect(m_timeStepCloseToCollision, SIGNAL(valueChanged(double)), this, SLOT(updateModelTimeStepCloseCollision(double)));

    QObject::connect(m_expandTree, SIGNAL(toggled(bool)), this, SLOT(expandAllNodes(bool)));
    QObject::connect(this, SIGNAL(resultHasChanged(QString,int,double)), this, SLOT(updateResultList(QString,int,double)));

    QObject::connect(this, SIGNAL(changeResults(QString)), this, SLOT(printResults(QString)));

    QObject::connect(m_workerThread, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(callWorkerThread(StdCmdView*)), m_worker, SLOT(doWork(StdCmdView*)));
    QObject::connect(m_worker, SIGNAL(finished()), this, SLOT(resultsAreReady()));
}

void CCFrame::openChemicalFile() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "Chemical files (*.xyz *.mol *.pdb *.log *.out *.mfj);;All files (*.*)"
    );

    if (!path.isEmpty()) {

        if (m_model->getInputFiles().size() > 0) {
            std::string oldFile = m_model->getInputFiles()[0];
            m_model->removeInputFile(oldFile);
        }

        QStringList chemicalList;
        try {
            m_model->setChargeFile("");
            QStringList chargeList;
            chargeList.append(m_NoFile);
            m_chargeFilesListModel->setStringList(chargeList);

            m_model->addInputFile(obtainRelativePath(path).toLatin1().toStdString());
            m_model->loadInputFiles();
            writeGeometriesInTreeModel(m_model->getLoadedGeometries());
            chemicalList.append(path);
        } catch(std::string caught) {
            QMessageBox::critical(this, "Error", QString::fromStdString(caught));
            chemicalList.append(m_NoFile);
            initializeTreeModel();
        }
        m_chemicalFilesListModel->setStringList(chemicalList);
    }
}

void CCFrame::openChargeFile() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "Charge files (*.chg);;All files (*.*)"
    );
    if (!path.isEmpty()) {
        QStringList list;
        try {
            m_model->setChargeFile(obtainRelativePath(path).toStdString());
            m_model->loadInputFiles();
            writeGeometriesInTreeModel(m_model->getLoadedGeometries());
            list.append(path);
        } catch(std::string caught) {
            m_model->setChargeFile("");
            writeGeometriesInTreeModel(m_model->getLoadedGeometries());
            list.append(m_NoFile);
            QMessageBox::critical(this, "Error", QString::fromStdString(caught));
        }
        m_chargeFilesListModel->setStringList(list);
    }
}

void CCFrame::openAtomInfosFile() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open",
        m_DefaultAtomInfosFiles,
        "Atom Informations files (*.csv);;All files (*.*)"
    );
    if (!path.isEmpty()) {
        try {
            AtomInformations::getInstance()->loadFile(obtainRelativePath(path).toStdString());
            QStringList list;
            list.append(path);
            m_atomInfosFilesListModel->setStringList(list);
        } catch(std::string caught) {
            QMessageBox::critical(this, "Error", QString::fromStdString(caught));
        }
    }
}

void CCFrame::saveResults() {
    QString path = QFileDialog::getSaveFileName(
        this,
        "Save",
        m_DefaultOutputFileName,
        "Collision Code Output Format (*.ccout);;All files (*.*)"
    );
    try {
        m_model->setOutputFile(obtainRelativePath(path).toStdString());
        m_model->saveResults();
    } catch(std::string caught) {
        QMessageBox::critical(this, "Error", QString::fromStdString(caught));
    }
}

void CCFrame::about() {
    QString message;
    QFile *file = new QFile("about.html");
    if (file->open(QIODevice::ReadOnly)) {
        QTextStream *flux = new QTextStream(file);
        flux->setCodec("UTF-8");
        message = flux->readAll();
        file->close();
    } else {
        message = "Impossible to open file: about.html";
    }
    QMessageBox::information(this, "About Collision Code", message);
}

void CCFrame::updateModelShouldPABeCalculated(bool value) {
    m_model->shouldPABeCalculated(value);
}

void CCFrame::updateModelShouldEHSSBeCalculated(bool value) {
    m_model->shouldEHSSBeCalculated(value);
}

void CCFrame::updateModelShouldTMBeCalculated(bool value) {
    m_model->shouldTMBeCalculated(value);
}

void CCFrame::updateModelLaunchCalculation() {
    m_geometriesNb = m_model->getLoadedGeometries().size();
    m_progressBarValue = 0;
    emit changeProgressBarValue(m_progressBarValue);
    emit disableWidgets(false);
    emit changeProgressBarVisibility(true);
    m_workerThread->start();
    emit callWorkerThread(m_model);
}

void CCFrame::updateModelMaxNumberThreads(int value) {
    SystemParameters::getInstance()->setMaximalNumberThreads(value);
}

void CCFrame::updateModelNbPointsMCIntegrationEHSSPA(int value) {
    GlobalParameters::getInstance()->setNbPointsMCIntegrationEHSSPA(value);
}

void CCFrame::updateModelTemperature(double value) {
    GlobalParameters::getInstance()->setTemperature(value);
}

void CCFrame::updateModelEnergyConservationThreshold(double value) {
    GlobalParameters::getInstance()->setEnergyConservationThreshold(value);
}

void CCFrame::updateModelNbCompleteCycles(int value) {
    GlobalParameters::getInstance()->setNumberCompleteCycles(value);
    calculateTotalPoints();
}

void CCFrame::updateModelNbVelocityPoints(int value) {
    GlobalParameters::getInstance()->setNumberVelocityPoints(value);
    calculateTotalPoints();
}

void CCFrame::updateModelNbPointsMCIntegrationTM(int value) {
    GlobalParameters::getInstance()->setNbPointsMCIntegrationTM(value);
    calculateTotalPoints();
}

void CCFrame::updateModelPotentialEnergyStart(double value) {
    GlobalParameters::getInstance()->setPotentialEnergyStart(value);
}

void CCFrame::updateModelPotentialEnergyCloseCollision(double value) {
    GlobalParameters::getInstance()->setPotentialEnergyCloseCollision(value);
}

void CCFrame::updateModelTimeStepStart(double value) {
    GlobalParameters::getInstance()->setTimeStepStart(value);
}

void CCFrame::updateModelTimeStepCloseCollision(double value) {
    GlobalParameters::getInstance()->setTimeStepCloseCollision(value);
}

void CCFrame::expandAllNodes(bool value) {
    if (value) {
        m_tree->expandAll();
    } else {
        m_tree->collapseAll();
    }
}

void CCFrame::printResults(QString str) {
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    m_text->setFont(font);

    const int tabStop = 4;  // 4 characters
    QFontMetrics metrics(font);
    m_text->setTabStopWidth(tabStop * metrics.width(' '));

    m_text->setText(str);
    m_text->setLineWrapMode(QTextEdit::NoWrap);
}

void CCFrame::updateResultList(QString method, int index, double value) {
    QBrush brush(Qt::GlobalColor::red);
    QStandardItem *item;
    if (method == m_PaToStr) {
        item = m_paResultList.at(index);
        item->setForeground(brush);
        item->setText(modifyResult(method, value));
        m_tree->resizeColumnToContents(m_Methods.at(m_PaToStr));

    } else if (method == m_EhssToStr) {
        item = m_ehssResultList.at(index);
        item->setForeground(brush);
        item->setText(modifyResult(method, value));
        m_tree->resizeColumnToContents(m_Methods.at(m_EhssToStr));
    } else {
        item = m_tmResultList.at(index);
        item->setForeground(brush);
        item->setText(modifyResult(method, value));
        m_tree->resizeColumnToContents(m_Methods.at(m_TmToStr));
    }
}

void CCFrame::resultsAreReady() {
    emit changeResults(QString::fromStdString(m_model->getResultFormat()));
    m_progressBar->setVisible(false);
    m_startCalculation->setVisible(true);
    m_openMenu->setEnabled(true);
    m_firstTab->setEnabled(true);
    m_secondTab->setEnabled(true);
    statusBar()->showMessage("Calculations finished", 4000);
}

void CCFrame::killThreadAndExit() {
    m_workerThread->terminate();
    m_workerThread->wait();
}

void CCFrame::calculateTotalPoints() {
    int total = m_completeCycles->value()
            * m_velocityPoints->value()
            * m_randomPoints->value();
    emit totalPoints(total);
}

void CCFrame::writeGeometriesInTreeModel(std::vector<Molecule *> geometries) {
    m_paResultList.clear();
    m_ehssResultList.clear();
    m_tmResultList.clear();
    for (unsigned int i = 0; i < geometries.size(); i++) {
        m_paResultList.append(new QStandardItem(modifyResult(m_PaToStr, -1)));
        m_ehssResultList.append(new QStandardItem(modifyResult(m_EhssToStr, -1)));
        m_tmResultList.append(new QStandardItem(modifyResult(m_TmToStr, -1)));
    }
    initializeTreeModel();

    QStandardItem *item;
    for (unsigned int i = 0; i < geometries.size(); i++) {
        Molecule *mol = geometries[i];
        QString molName = QString::fromStdString(mol->getName());
        item = new QStandardItem(molName.append(" : (").append(QString::number(i + 1)).append(")"));
        //item->setCheckable(true);
        //item->setCheckState(Qt::Checked);
        QList<QStandardItem *> itemsList;
        itemsList << item << m_paResultList.at(i) << m_ehssResultList.at(i)
                  << m_tmResultList.at(i) << new QStandardItem;
        for (QStandardItem *it : itemsList) {
            it->setEditable(false);
            it->setTextAlignment(Qt::AlignCenter);
        }
        m_treeModel->appendRow(itemsList);

        std::vector<Atom *> *atoms = mol->getAllAtoms();
        for (unsigned int j = 0; j < mol->getAtomNumber(); j++) {
            Atom *atom = (*atoms)[j];
            item->setChild(j, 0, new QStandardItem(QString::fromStdString(atom->getSymbol())));
            item->setChild(j, 1, new QStandardItem(QString::number(atom->getPosition()->x)));
            item->setChild(j, 2, new QStandardItem(QString::number(atom->getPosition()->y)));
            item->setChild(j, 3, new QStandardItem(QString::number(atom->getPosition()->z)));
            item->setChild(j, 4, new QStandardItem(QString::number(atom->getCharge())));
            for (unsigned int k = 0; k < m_ColumnNames.size(); k++) {
                item->child(j, k)->setEditable(false);
            }
        }
    }

    for (unsigned int i = 0; i < m_ColumnNames.size(); i++) {
        m_tree->resizeColumnToContents(i);
    }
}

void CCFrame::initializeTreeModel() {
    m_treeModel->clear();
    QStringList list;
    for (unsigned int i = 0; i < m_ColumnNames.size(); i++) {
        list.append(m_ColumnNames[i]);
    }
    m_treeModel->setHorizontalHeaderLabels(list);
}





QString modifyResult(QString method, double d) {
    QString res = method;
    if (d == -1) {
        res.append(" not calc.");
    } else {
        res.append(" = ").append(QString::number(d));
    }
    return res;
}

void initializeSpinBox(QSpinBox *sb, int min, int max, int step, int value) {
    sb->setRange(min, max);
    sb->setValue(value);
    sb->setSingleStep(step);
    sb->setAccelerated(true);
}
void initializeDoubleSpinBox(QDoubleSpinBox *dsb, double min, double max,
                             double step, int decimals, double value) {
    dsb->setRange(min, max);
    dsb->setDecimals(decimals);
    dsb->setValue(value);
    dsb->setSingleStep(step);
    dsb->setAccelerated(true);
    dsb->setLocale(QLocale::English);
}

QString obtainRelativePath(QString filePath) {
    QString relativePath;
    QStringList fromFilePath = filePath.split('/');
    QStringList fromCurrentDir = QDir::currentPath().split('/');
    while (!fromFilePath.isEmpty() && !fromCurrentDir.isEmpty() && fromFilePath.first() == fromCurrentDir.first()) {
        fromFilePath.removeFirst();
        fromCurrentDir.removeFirst();
    }
    while (!fromCurrentDir.isEmpty()) {
        relativePath.append("../");
        fromCurrentDir.removeFirst();
    }
    while (!fromFilePath.isEmpty()) {
        relativePath.append(fromFilePath.first());
        if (fromFilePath.size() > 1) {
            relativePath.append("/");
        }
        fromFilePath.removeFirst();
    }
    return relativePath;
}
