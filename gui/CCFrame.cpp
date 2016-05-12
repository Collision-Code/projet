#include "CCFrame.h"

#define _HEIGHT_ITEM_AND_SCROLLBAR 38

QString modifyResult(QString method, double d);
void initializeSpinBox(QSpinBox *sb, int min, int max, int step, int value);
void initializeDoubleSpinBox(QDoubleSpinBox *dsb, double min, double max,
                             double step, int decimals, double value);
QTreeView *tree;
int geometries = 5;

// CONSTRUCTEUR
CCFrame::CCFrame() {
    createView();
    placeComponents();
    createControllers();
}

// DESTRUCTEUR
CCFrame::~CCFrame() {}


/**
 * @brief CCFrame::createView
 * Permet d'initialiser les éléments graphiques importants liés au model.
 */
void CCFrame::createView() {
    // Configuration de la fenêtre principale
    setWindowTitle("Collision Code");
    setMinimumSize(600, 500);

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
    m_progressBar->setValue(15);
    m_progressBar->setEnabled(false);

    m_startCalculation = new QPushButton("Start Calculation!");

    // Widgets de la partie centrale
    // Méthodes utilisées
    m_pa = new QCheckBox("PA");
    m_ehss = new QCheckBox("EHSS");
    m_tm = new QCheckBox("TM");
    m_He = new QRadioButton("He");
    m_N2 = new QRadioButton("N2");
    m_pa->setChecked(true);
    m_ehss->setChecked(true);
    m_tm->setChecked(true);
    m_He->setChecked(true);
    m_N2->setEnabled(false);

    // Seuil de conservation d'energie au cours de la méthode TM
    m_energy = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_energy, 90.00, 99.99, 0.01, 2, 99.00);

    m_temperature = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_temperature, 0.0, 1000.0, 0.1, 1, 298.0);

    m_completeCycles = new QSpinBox;
    initializeSpinBox(m_completeCycles, 10, 100, 1, 10);

    m_velocityPoints = new QSpinBox;
    initializeSpinBox(m_velocityPoints, 10, 100, 1, 40);

    m_randomPoints = new QSpinBox;
    initializeSpinBox(m_randomPoints, 10, 5000, 10, 25);

    m_totalPoints = new QSpinBox;
    initializeSpinBox(m_totalPoints, 1000, 50000000, 1000, 10000);
    m_totalPoints->setEnabled(false);
    m_totalPoints->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_totalPoints->setToolTip("This box is not directly editable.\n"
                              "Its value depends on the values of other fields.");

    m_monteCarloTrajectories = new QSpinBox;
    initializeSpinBox(m_monteCarloTrajectories, 10000, 5000000, 10000, 250000);

    m_threads = new QSpinBox;
    initializeSpinBox(m_threads, 1, 10000, 10, 20);

    m_potentialStartEnergy = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_potentialStartEnergy, 0.00001, 0.001, 0.00001, 5, 0.00005);

    m_potentialEnergyCollision = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_potentialEnergyCollision, 0.001, 0.01, 0.0001, 4, 0.0025);

    m_timeStepAtStart = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_timeStepAtStart, 0.1, 1.0, 0.01, 2, 0.5);

    m_timeStepCloseToCollision = new QDoubleSpinBox;
    initializeDoubleSpinBox(m_timeStepCloseToCollision, 0.01, 0.1, 0.001, 3, 0.05);


    QStringList noFileList;
    noFileList << "No File";
    QStringList defaultFileList;
    defaultFileList << "Default File";
    m_chemicalFilesListModel = new QStringListModel(noFileList);
    m_chargeFilesListModel = new QStringListModel(noFileList);
    m_atomInfosFilesListModel = new QStringListModel(defaultFileList);
    m_chemicalFileOpenButton = new QPushButton("Open");
    m_chargeFileOpenButton = new QPushButton("Open");
    m_atomInfosFileOpenButton = new QPushButton("Open");

    m_text = new QTextEdit;
    m_text->setPlainText(
        "Results will be posted here when calculations are completed.\n"
        "Results of collision cross sections are available in real time during calculations in \"Geometries\" tab.");
    m_text->setReadOnly(true);
    m_saveGeometriesInFile = new QCheckBox("Also save in file the geometries used in calculations");
    m_saveGeometriesInFile->setChecked(true);
    m_saveResultsButton = new QPushButton("Save in text file");

    m_expandTree = new QCheckBox("Expand all tree nodes");


    for (int i = 0; i < geometries; i++) {
        paResultList.append(new QStandardItem(modifyResult("PA", -1)));
        ehssResultList.append(new QStandardItem(modifyResult("EHSS", -1)));
        tmResultList.append(new QStandardItem(modifyResult("TM", -1)));
    }
}

/**
 * @brief CCFrame::placeComponents
 * Permet de placer tous les composants graphiques sur l'application.
 */
void CCFrame::placeComponents() {
    // Barre des menus
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *openMenu = fileMenu->addMenu("&Open");
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *viewMenu = menuBar()->addMenu("&View");
    QMenu *helpMenu = menuBar()->addMenu("&?");

    openMenu->addAction(m_openChemicalFileAction);
    openMenu->addAction(m_openChargeFileAction);
    openMenu->addAction(m_openAtomInformationAction);
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
    QWidget *tab = new QWidget;

        QVBoxLayout *vbox = new QVBoxLayout;

            // Affiche le nom du fichier ouvert et propose l'ouverture de fichier.
            QGroupBox *groupbox = new QGroupBox("Opened Files");
                QVBoxLayout *subvbox = new QVBoxLayout;
                    QGroupBox *subgroupbox = new QGroupBox("Chemical File");
                        QHBoxLayout *hbox = new QHBoxLayout;
                            QListView *filesList = new QListView;
                            filesList->setMaximumHeight(_HEIGHT_ITEM_AND_SCROLLBAR);
                            filesList->setModel(m_chemicalFilesListModel);
                            hbox->addWidget(filesList);
                            hbox->addWidget(m_chemicalFileOpenButton);
                        subgroupbox->setLayout(hbox);
                    subvbox->addWidget(subgroupbox);
                    subgroupbox = new QGroupBox("Charge File");
                        hbox = new QHBoxLayout;
                            filesList = new QListView;
                            filesList->setMaximumHeight(_HEIGHT_ITEM_AND_SCROLLBAR);
                            filesList->setModel(m_chargeFilesListModel);
                            hbox->addWidget(filesList);
                            hbox->addWidget(m_chargeFileOpenButton);
                        subgroupbox->setLayout(hbox);
                    subvbox->addWidget(subgroupbox);
                    subgroupbox = new QGroupBox("Atom Informations File");
                        hbox = new QHBoxLayout;
                            filesList = new QListView;
                            filesList->setMaximumHeight(_HEIGHT_ITEM_AND_SCROLLBAR);
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
        tab->setLayout(vbox);
    tabs->addTab(tab, "Configuration");


    // Réglages des paramètres avancés
    tab = new QWidget;
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
        tab->setLayout(vbox);
    tabs->addTab(tab, "Advanced Parameters");


    // Onglet d'affichage des données du modèle
    tab = new QWidget;
        vbox = new QVBoxLayout;
            vbox->addWidget(m_expandTree);

            QStandardItemModel *model = new QStandardItemModel;
            QStringList list;
            list << "Symbol" << "xCoord" << "yCoord" << "zCoord" << "Charge";
            model->setHorizontalHeaderLabels(list);

            int atoms = 15;
            double x = 1.1, y = 2.2, z = 3.3, charge = 0.125;
            QStandardItem *item;
            for (int i = 0; i < geometries; i++) {
                QString str = "NOM MOL";
                item = new QStandardItem(str.append(" : (").append(QString::number(i + 1)).append(")"));
                item->setCheckable(true);
                item->setCheckState(Qt::Checked);
                QList<QStandardItem *> itemsList;
                itemsList << item << paResultList.at(i) << ehssResultList.at(i)
                          << tmResultList.at(i) << new QStandardItem;
                for (QStandardItem *it : itemsList) {
                    it->setEditable(false);
                    it->setTextAlignment(Qt::AlignCenter);
                }
                model->appendRow(itemsList);

                for (int j = 0; j < atoms; j++) {
                    item->setChild(j, 0, new QStandardItem("H"));
                    item->setChild(j, 1, new QStandardItem(QString::number(x)));
                    item->setChild(j, 2, new QStandardItem(QString::number(y)));
                    item->setChild(j, 3, new QStandardItem(QString::number(z)));
                    item->setChild(j, 4, new QStandardItem(QString::number(charge)));
                    for (int k = 0; k < list.size(); k++) {
                        item->child(j, k)->setEditable(false);
                    }
                }
            }

            tree = new QTreeView;
            tree->setModel(model);
            vbox->addWidget(tree);
        tab->setLayout(vbox);
    tabs->addTab(tab, "Geometries");


    // Onglet d'affichage complets des Résultats
    tab = new QWidget;
        vbox = new QVBoxLayout;
            vbox->addWidget(m_text);
            vbox->addWidget(m_saveGeometriesInFile);
            vbox->addWidget(m_saveResultsButton);
        tab->setLayout(vbox);
    tabs->addTab(tab, "Complete Results");

    setCentralWidget(tabs);
}

void CCFrame::createControllers() {
    QObject::connect(m_startCalculation, SIGNAL(clicked(bool)), m_progressBar, SLOT(setDisabled(bool)));

    QObject::connect(m_openChemicalFileAction, SIGNAL(triggered()), this, SLOT(openChemicalFile()));
    QObject::connect(m_openChargeFileAction, SIGNAL(triggered()), this, SLOT(openChargeFile()));
    QObject::connect(m_openAtomInformationAction, SIGNAL(triggered()), this, SLOT(openAtomInfosFile()));
    QObject::connect(m_saveAction, SIGNAL(triggered()), this, SLOT(saveResults()));
    QObject::connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    QObject::connect(m_chemicalFileOpenButton, SIGNAL(pressed()), this, SLOT(openChemicalFile()));
    QObject::connect(m_chargeFileOpenButton, SIGNAL(pressed()), this, SLOT(openChargeFile()));
    QObject::connect(m_atomInfosFileOpenButton, SIGNAL(pressed()), this, SLOT(openAtomInfosFile()));
    QObject::connect(m_saveResultsButton, SIGNAL(pressed()), this, SLOT(saveResults()));

    QObject::connect(m_completeCycles, SIGNAL(valueChanged(int)), this, SLOT(calculateTotalPoints(int)));
    QObject::connect(m_velocityPoints, SIGNAL(valueChanged(int)), this, SLOT(calculateTotalPoints(int)));
    QObject::connect(m_randomPoints, SIGNAL(valueChanged(int)), this, SLOT(calculateTotalPoints(int)));
    QObject::connect(this, SIGNAL(totalPoints(int)), m_totalPoints, SLOT(setValue(int)));

    QObject::connect(m_expandTree, SIGNAL(clicked(bool)), this, SLOT(expandAllNodes(bool)));
}

void CCFrame::openChemicalFile() {
    QString tmp = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "Chemical files (*.xyz *.mol *.pdb *.log *.out *.mfj);;All files (*.*)"
    );
    if (!tmp.isEmpty()) {
        QStringList list;
        list.append(tmp);
        m_chemicalFilesListModel->setStringList(list);
    }
}

void CCFrame::openChargeFile() {
    QString tmp = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "Charge files (*.chg);;All files (*.*)"
    );
    if (!tmp.isEmpty()) {
        QStringList list;
        list.append(tmp);
        m_chargeFilesListModel->setStringList(list);
    }
}

void CCFrame::openAtomInfosFile() {
    QString tmp = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "Atom Informations files (*.csv);;All files (*.*)"
    );
    if (!tmp.isEmpty()) {
        QStringList list;
        list.append(tmp);
        m_atomInfosFilesListModel->setStringList(list);
    }
}

void CCFrame::saveResults() {
    m_saveFile = QFileDialog::getSaveFileName(
        this,
        "Save",
        QString(),
        "Collision Code Output Format (*.ccout);;All files (*.*)"
    );
}

void CCFrame::about() {
    QString message = "<h3>Collision Code</h3>"
        "<p>This software was developped at the "
        "<a href=\"http://sciences-techniques.univ-rouen.fr/\">"
        "University of Rouen Normandie</a> to calculate collision cross sections. "
        "According to the methods in the MOBCAL software "
        "(Shvartsburg and Jarrold, 1996-1999) available from "
        "<a href=\"http://www.indiana.edu/~nano/software/\">"
        "M. F. Jarrold's group at Indiana University</a>.</p>"
        "<p>At the request of:<ul>"
        "<li>Hélène LAVANANT</li><li>Vincent TOGNETTI<li></ul></p>"
        "<p>Developers Team:<ul>"
        "<li>Anthony BREANT</li><li>Clément POINSOT<li><li>Jérémie PANTIN</li>"
        "<li>Mohamed TAKHTOUKH</li><li>Thomas CAPET</li></ul></p>"
        "<p>Contact us at <a href=\"mailto:collisioncode@gmail.com\">"
        "collisioncode@gmail.com</a>";

    QMessageBox::information(this, "About Collision Code", message);
}

void CCFrame::calculateTotalPoints(int value) {
    int total = m_completeCycles->value()
            * m_velocityPoints->value()
            * m_randomPoints->value();
    emit totalPoints(total);
}

void CCFrame::expandAllNodes(bool value) {
    if (value) {
        tree->expandAll();
    } else {
        tree->collapseAll();
    }
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
    //dsb = new QDoubleSpinBox;
    dsb->setRange(min, max);
    dsb->setDecimals(decimals);
    dsb->setValue(value);
    dsb->setSingleStep(step);
    dsb->setAccelerated(true);
    dsb->setLocale(QLocale::English);
}
