#include <QtGui>
#include <QtWidgets>
#include <QPainter>
#include <QTextDocument>
#include <QGuiApplication>
#include <QDebug>
#include <QImage>
#include <QTextDocumentWriter>
#include "mainwindow.h"
#include "centralwidget.h"
#include "ui_dockcontrols.h"
#include "dockcontrols.h"
#include "laserpropertiesdialog.h"
#include "ui_laserpropertiesdialog.h"
#include "reflectorpropertiesdialog.h"
#include "ui_reflectorpropertiesdialog.h"
#include "binocularpropertiesdialog.h"
#include "ui_binocularpropertiesdialog.h"
#include "labroom.h"
#include "labeditdialog.h"
#include "ui_labeditdialog.h"
#include "footprintdialog.h"
#include "ui_footprintdialog.h"
#include "beaminspectordialog.h"
#include "ui_beaminspectordialog.h"
#include "gotopointdialog.h"
#include "ui_gotopointdialog.h"
#include "description.h"
#include "ui_description.h"
#include "atmosphericeffectsdialog.h"
#include "ui_atmosphericeffectsdialog.h"
#include "undo_commands/addreflectorcommand.h"
#include "undo_commands/addbinocularcommand.h"
#include "undo_commands/addbeaminspectorcommand.h"
#include "undo_commands/addmeteocommand.h"
#include "undo_commands/addfootprintcommand.h"
#include "undo_commands/addfootprintpropertycommand.h"
#include "undo_commands/addlabpropertycommand.h"
#include "undo_commands/addlaserpropertycommand.h"
#include "undo_commands/addbinocularpropertycommand.h"
#include "undo_commands/addreflectorpropertycommand.h"
#include "undo_commands/deletereflectorcommand.h"
#include "undo_commands/deletebinocularcommand.h"
#include "undo_commands/deletebeaminspectorcommand.h"
#include "undo_commands/deletefootprintcommand.h"
#include "undo_commands/movecommand.h"

#include <QtWidgets>
#include <iterator>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#include <fstream>
#endif
#endif
#endif

const QSize MainWindow::mySceneImageSize(600, 400);

MainWindow::MainWindow()
      : laserWindow(new CentralWidget)
{
    QFont font;
    font.setPointSize(8);
    setFont(font);

    setThemeOnStart();

    undoStack = new QUndoStack(this);
    laserWindow->setUndoStack(undoStack);
    setCentralWidget(laserWindow);

    setWindowTitle(tr("Laser Studio"));
    QIcon iconWin;
    iconWin.addFile(QStringLiteral(":/images/ico.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(iconWin);

    scales << tr("6.25%") << tr("12.5%") << tr("25%") << tr("50%") << tr("100%") << tr("200%")
           << tr("400%") << tr("800%") << tr("1000%") << tr("1200%") << tr("1600%") << tr("2500%")
           << tr("3200%") << tr("4000%") << tr("6000%") << tr("8000%") << tr("10000%")<< tr("15000%")
           << tr("20000%") << tr("25000%");

    scale=1;
    footprintsCount=0;
    dragModeState=false;

    createUndoView();
    createActions();
    createStatusBar();
    createToolBars();
    createContextMenu();
    updateActions();

    setCurrentFile("");

    setMouseTracking(true);
    qApp->installEventFilter(this);

    setLaserPoint();
    createRoom();

    previewRect=laserWindow->graphicsView->viewport()->rect();
    qDebug()<<"Rettangolo anteprima di stampa" << previewRect;

    state=false;
    reflectorsModel= new ReflectorsListModel(myReflectors, this);
    binocularsModel= new BinocularsListModel(myBinoculars, this);
    inspectorsModel= new InspectorsListModel(myBeamInspectors, this);
    environmentModel= new EnvironmentListModel(labroomList, state, this);
    QRectF fakeRect= QRectF(0.0, 0.0, 0.0, 0.0);
    myFakeRoom=new LabRoom(fakeRect);
    environmentModel->addDescriptor(*myFakeRoom);

    laserWindow->myDockReflectorsList->ui->listView->setWordWrap(true);
    laserWindow->myDockReflectorsList->ui->listView->setModel(reflectorsModel);
    laserWindow->myDockReflectorsList->ui->binocularListView->setModel(binocularsModel);
    laserWindow->myDockReflectorsList->ui->inspectorListView->setModel(inspectorsModel);
    laserWindow->myDockReflectorsList->ui->environmentListView->setModel(environmentModel);

    reflectorsSelectionModel=laserWindow->myDockReflectorsList->ui->listView->selectionModel();
    binocularsSelectionModel=laserWindow->myDockReflectorsList->ui->binocularListView->selectionModel();
    inspectorsSelectionModel=laserWindow->myDockReflectorsList->ui->inspectorListView->selectionModel();
    environmentSelectionModel=laserWindow->myDockReflectorsList->ui->environmentListView->selectionModel();

    laserPointList.clear();
    BeamInspector::computeRayleighDistance(laserWindow->myDockControls->getWavelength(),
                                           laserWindow->myDockControls->getBeamDiameter(),
                                           laserWindow->myDockControls->getDivergence());

    laserpoint->setRayleighDistance(BeamInspector::getRayleighDistance());
    laserpoint->setQualityFactor(BeamInspector::getQualityFactor());
    laserModel= new LaserListModel(laserPointList, this);
    laserModel->addDescriptor(*laserpoint);

    laserWindow->myDockReflectorsList->ui->laserListView->setWordWrap(true);
    laserWindow->myDockReflectorsList->ui->laserListView->setModel(laserModel); 
    laserSelectionModel=laserWindow->myDockReflectorsList->ui->laserListView->selectionModel();
    setupLaserProspective();

    externalFile=false;
    externalFile=(QCoreApplication::arguments().size()>1);

    QString fileName;
    if (externalFile==true)
    {
        fileName = QApplication::arguments().at(1);
        timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->start(100);
        loadFile(fileName);
        connect(timer, SIGNAL(timeout()), this, SLOT(makeSceneOfSavedItems()));
    }

    readSettings();

    connect(laserWindow->myDockControls, SIGNAL(NOHD_Changed()), this, SLOT(setDNRO_ForLaserpoint()));
    connect(laserWindow->myDockControls, SIGNAL(NOHD_Changed()), this, SLOT(setDNRO_ForReflector()));
    connect(laserWindow->myDockControls, SIGNAL(NOHD_Changed()), this, SLOT(setDNRO_ForBinocular()));
    connect(laserWindow->myDockControls, SIGNAL(NOHD_Changed()), this, SLOT(setDNRO_ForFootprint()));
    connect(laserWindow->myDockControls, SIGNAL(lambertianMaxChanged()), this, SLOT(setLambertianMaxForReflector()));
    connect(laserWindow->myDockControls, SIGNAL(NSHD_Changed()), this, SLOT(setDNRC_ForLaserpoint()));
    connect(laserWindow->myDockControls, SIGNAL(divergenceChanged()), this, SLOT(setDivergenceForReflector()));
    connect(laserWindow->myDockControls, SIGNAL(modified()), this, SLOT(laserModified()));
    connect(laserWindow->myDockControls, SIGNAL(beamDiameterChanged()), this, SLOT(setReflectorBeamDiameterForDiffusion()));
    connect(laserWindow->myDockControls, SIGNAL(beamDiameterChanged()), this, SLOT(setBeamDiameterForBinocular()));
    connect(laserWindow->myDockControls, SIGNAL(beamDiameterChanged()), this, SLOT(updateForBeamInspection()));
    connect(laserWindow->myDockControls, SIGNAL(wavelengthChanged()), this, SLOT(updateForBeamInspection()));
    connect(laserWindow->myDockControls, SIGNAL(divergenceChanged()), this, SLOT(updateForBeamInspection()));
    connect(laserWindow->myDockControls, SIGNAL(powerErgChanged()), this, SLOT(setReflectorPowerErgForDiffusion()));
    connect(laserWindow->myDockControls, SIGNAL(EMP_Changed()), this, SLOT(setReflectorEMP_ForDiffusion()));
    connect(laserWindow->myDockControls, SIGNAL(wavelengthChanged()), this, SLOT(setWavelengthForBinocular()));
    connect(this, SIGNAL(reflectorListChanged()), this, SLOT(addElementList()));
    connect(this, SIGNAL(myLabRoomListChanged()), this, SLOT(addLabList()));
    connect(laserWindow->myDockReflectorsList->ui->laserListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(propertyFromList()));
    connect(laserWindow->myDockReflectorsList->ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectFromList()));
    connect(laserWindow->myDockReflectorsList->ui->environmentListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(environmentFromList()));
    connect(laserWindow->myDockReflectorsList->ui->binocularListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectBinocularFromList()));    
    connect(laserWindow->myDockReflectorsList->ui->inspectorListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectInspectorFromList()));
    connect(laserWindow->myDockReflectorsList->ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(goToSelectedReflector()));
    connect(laserWindow->myDockReflectorsList->ui->binocularListView, SIGNAL(clicked(QModelIndex)), this, SLOT(goToSelectedBinocular()));
    connect(laserWindow->myDockReflectorsList->ui->laserListView, SIGNAL(clicked(QModelIndex)), this, SLOT(gotToLaserpoint()));
    connect(laserWindow->myDockReflectorsList->ui->environmentListView, SIGNAL(clicked(QModelIndex)), this, SLOT(goToLab()));
    connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
    connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(laserModified()));
    connect(laserWindow->graphicsView->scene, SIGNAL(laserSelected()), this, SLOT(laserpointSelectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(reflectorSelected()), this, SLOT(listSelectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(inspectorSelected()), this, SLOT(inspectorListSelectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(binocularSelected()), this, SLOT(binocularListSelectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(labroomSelected()), this, SLOT(labroomSelectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(deselected()), this, SLOT(listDeselectionFromGraphics()));
    connect(laserWindow->graphicsView->scene, SIGNAL(footprintRelease()), this, SLOT(shadowZoneForLaser()));
    connect(laserWindow->graphicsView, SIGNAL(mouseRelease()), this, SLOT(listMultipleSelectionFromGraphics()));
    connect(laserWindow->myDockControls, SIGNAL(noFeasibleInput()), this, SLOT(noFeasibleInputFunction()));
    connect(laserWindow->myDockControls, SIGNAL(operationChanged()), this, SLOT(deletedViewCenter()));
    connect(this, SIGNAL(binocularListChanged()), this, SLOT(addBinocularList()));
    connect(laserWindow->graphicsView->scene, SIGNAL(changed(const QList<QRectF> &)),this, SLOT(setViewportRect()));
    connect(laserWindow->graphicsView, SIGNAL(viewportChanged()),this, SLOT(setViewportRect()));
    connect(laserWindow->graphicsView->scene, &GraphicsScene::graphicItemMoved, this, &MainWindow::graphicItemMoveToStack);
    connect(undoStack, &QUndoStack::indexChanged, this, &MainWindow::updateUndoStackList);
    connect(this, SIGNAL(beamInspectorListChanged()), this, SLOT(addInspectorList()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::setLaserPoint()
{
    minZ = 0;
    maxZ = 0;
    seqNumber = 0;    
    binSeqNumber = 0;
    footprintSeqNumber = 0;
    inspectorSeqNumber = 0;

    footprint=nullptr;
    reflector=nullptr;
    binocular=nullptr;
    beamInspector=nullptr;
    myLabRoom=nullptr;
    gridlines=nullptr;

    laserWindow->setGridState(false);

    laserpoint = new LaserPoint();

    laserpoint->setPos(QPoint(0, 0));
    laserpoint->setUndoStack(undoStack);
    laserpoint->setSelected(true);
    laserpoint->setStringPosition();

    laserWindow->graphicsView->scene->addItem(laserpoint);
    laserWindow->graphicsView->scene->clearSelection();

    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setUpdatedPosition()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setUpdatedPosition()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForReflector()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForReflector()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForInspector()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForInspector()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateList()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateList()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateLaserList()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateLaserList()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForBinocular()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForBinocular()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForFootprint()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForFootprint()));
    connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setShadowZone()));
    connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setShadowZone()));
}

bool MainWindow::okToContinue()
{
    if (isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("Progetto"), tr("Il progetto è stato modificato.\n"
                               "Vuoi salvare le modifiche?"), QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);

        if(r==QMessageBox::Yes)
            return save();
        else if(r==QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::newFile()
{
    if (okToContinue())
    {

        /***********************************************************************************
         * Pulisco la scena (cancellando gli oggetti ad uno ad uno con la funzione membro  *
         * clearScene() non essedo possibile con clean()) ed imposto una nuova scena       *
         * grafica.                                                                        *
         ***********************************************************************************/

        clearScene();
        laserWindow->graphicsView->scene->clear();
        laserWindow->setNewScene();

        myReflectors.clear();
        reflectorsModel->deleteList();
        reflectorsSelectionModel->clear();

        myBinoculars.clear();
        binocularsModel->deleteList();
        binocularsSelectionModel->clear();

        myBeamInspectors.clear();
        inspectorsModel->deleteList();
        inspectorsSelectionModel->clear();

        labroomList.clear();
        environmentModel->deleteList();
        environmentSelectionModel->clear();
        environmentModel->addDescriptor(*myFakeRoom);

        laserPointList.clear();
        laserModel->deleteList();
        laserSelectionModel->clear();

        myFootprints.clear();

        laserWindow->clearInstallationDesription();
        setLaserPoint();
        showGridAction->setChecked(false);

        onlyReflectorGoggleAction->setChecked(true);
        setGoggleMaterial(LaserGoggle::ONLY_REFLECTOR);

        menuSceneScaleChanged("100%", 4);

        laserModel->addDescriptor(*laserpoint);
        laserSelectionModel->select(laserModel->index(0, 0), QItemSelectionModel::Select);
        setControls();
        laserWindow->myDockControls->setVIS();

        setCurrentFile("");

        laserWindow->setMeteoRange(CentralWidget::STANDARD_VISIBILITY_DISTANCE);
        meteoWidgets(true, false, false);
        laserpoint->setSelected(true);
        laserWindow->graphicsView->centerOn(laserpoint->pos());

        QFont font;
        font.setPointSize(8);
        setFont(font);

        environmentModel->setState(false);
        environmentModel->setMeteoVisibility(laserWindow->getMeteoRange());
        environmentModel->myDataHasChanged();

        setPolygon();
        setPolygonAct->setChecked(true);

        setupLaserProspective();
        laserSettingsAction->setChecked(true);

        undoStack->clear();

        connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
        connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(laserModified()));
        connect(laserWindow->graphicsView->scene, SIGNAL(laserSelected()), this, SLOT(laserpointSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(reflectorSelected()), this, SLOT(listSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(binocularSelected()), this, SLOT(binocularListSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(inspectorSelected()), this, SLOT(inspectorListSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(labroomSelected()), this, SLOT(labroomSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(deselected()), this, SLOT(listDeselectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(footprintRelease()), this, SLOT(shadowZoneForLaser()));
        connect(laserWindow->graphicsView->scene, SIGNAL(changed(const QList<QRectF> &)),this, SLOT(setViewportRect()));
        connect(laserWindow->graphicsView->scene, &GraphicsScene::graphicItemMoved, this, &MainWindow::graphicItemMoveToStack);
    }
}

void MainWindow::setControls()
{
    laserWindow->myDockControls->ui->comboBoxBands->setCurrentIndex(1);
    laserWindow->myDockControls->ui->operationCombo->setCurrentIndex(0);
    laserWindow->myDockControls->setDialControls();
}

void MainWindow::setCurrentDirectory()
{
    QFileInfo filenameInfo;
    if(!recentFiles.empty())
    {
        filenameInfo=recentFiles.first();
        curDirectoryFile=filenameInfo.absolutePath();
    }
}
void MainWindow::open()
{
    if (okToContinue())
    {
        setCurrentDirectory();
        QString fileName = QFileDialog::getOpenFileName(this,
                                       tr("Apri progetto"), curDirectoryFile,
                                       tr("File Laser Studio (*.srk)"));
        if (!fileName.isEmpty())
        {
            loadFile(fileName);
            makeSceneOfSavedItems();

            setupLaserProspective();
            laserSettingsAction->setChecked(true);
            undoStack->clear();
            laserWindow->setUndoStack(undoStack);

            connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
            connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(laserModified()));
            connect(laserWindow->graphicsView->scene, SIGNAL(laserSelected()), this, SLOT(laserpointSelectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(reflectorSelected()), this, SLOT(listSelectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(binocularSelected()), this, SLOT(binocularListSelectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(inspectorSelected()), this, SLOT(inspectorListSelectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(labroomSelected()), this, SLOT(labroomSelectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(deselected()), this, SLOT(listDeselectionFromGraphics()));
            connect(laserWindow->graphicsView->scene, SIGNAL(footprintRelease()), this, SLOT(shadowZoneForLaser()));
            connect(laserWindow->graphicsView->scene, SIGNAL(changed(const QList<QRectF> &)),this, SLOT(setViewportRect()));

            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setUpdatedPosition()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setUpdatedPosition()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForReflector()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForReflector()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateList()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateList()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateLaserList()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateLaserList()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForBinocular()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForBinocular()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForFootprint()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForFootprint()));
            connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setShadowZone()));
            connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setShadowZone()));
            connect(laserWindow->graphicsView->scene, &GraphicsScene::graphicItemMoved, this, &MainWindow::graphicItemMoveToStack);

            laserpoint->setPos(laserpoint->pos()+QPointF(-1,-1));
            laserpoint->setPos(laserpoint->pos()+QPointF(1,1));
            laserWindow->graphicsView->centerOn(laserpoint->pos());

            laserWindow->graphicsView->update();
        }
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);    
}

bool MainWindow::saveAs()
{
  setCurrentDirectory();
  QString saveFilename;

  if(curFile.isEmpty())
      saveFilename=curDirectoryFile;
  else
      saveFilename=curFile;


  QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Salva progetto"), saveFilename,
                               tr("File Laser Studio (*.srk)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::openRecentFile()
{
    if (okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());

        makeSceneOfSavedItems();
        setupLaserProspective();
        laserSettingsAction->setChecked(true);

        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setUpdatedPosition()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setUpdatedPosition()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForReflector()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForReflector()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateList()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateList()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(updateLaserList()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(updateLaserList()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForBinocular()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForBinocular()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setDistanceForFootprint()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setDistanceForFootprint()));
        connect(laserpoint, SIGNAL(xChanged()), this, SLOT(setShadowZone()));
        connect(laserpoint, SIGNAL(yChanged()), this, SLOT(setShadowZone()));

        connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
        connect(laserWindow->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(laserModified()));
        connect(laserWindow->graphicsView->scene, SIGNAL(laserSelected()), this, SLOT(laserpointSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(reflectorSelected()), this, SLOT(listSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(binocularSelected()), this, SLOT(binocularListSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(inspectorSelected()), this, SLOT(inspectorListSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(labroomSelected()), this, SLOT(labroomSelectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(deselected()), this, SLOT(listDeselectionFromGraphics()));
        connect(laserWindow->graphicsView->scene, SIGNAL(footprintRelease()), this, SLOT(shadowZoneForLaser()));
        connect(laserWindow->graphicsView->scene, SIGNAL(changed(const QList<QRectF> &)),this, SLOT(setViewportRect()));
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Informazioni su Laser Studio"),
            tr("<h2>Laser Studio</h2>"
               "<p>Applicazione per la valutazione della sicurezza laser in accordo con la direttiva SMD-W-001.</p>"
               "<p>Autore: Ing. Carmine Giordano</p>"
               "<p>La disponibilità del software è resa nei termini specificati dalla GNU GPL consultabile dal sito <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">gnu.org.</a></p>"));
}

void MainWindow::onLineHelp()
{
    //qHtmlHelp("Laser Safety.chm","generalita.html");
    //qDebug() << QUrl("file:Laser Safety.chm").isRelative();
    QUrl helpFile;
    helpFile.setPath("LaserStudio.chm", QUrl::TolerantMode);
    QDesktopServices::openUrl(helpFile);
}

void MainWindow::selectFromList()
{
    if(reflector==0)
        return;

    QModelIndex index=laserWindow->myDockReflectorsList->getModelIndex();
    int listRow=index.row();

    qDebug() << "valore di listrow: " <<listRow;
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();

    while (myIterator != myReflectors.end())
    {
        if(myIterator->second==listRow)
            reflector=myIterator->first;

        ++myIterator;
    }

    laserWindow->graphicsView->scene->clearSelection();
    reflector->setSelected(true);

    ReflectorPropertiesDialog dialog(reflector, this);
    dialog.exec();
}

void MainWindow::selectBinocularFromList()
{
    if(binocular==0)
        return;

    QModelIndex index=laserWindow->myDockReflectorsList->getModelBinocularIndex();
    int listRow=index.row();
    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();

    while (myIterator != myBinoculars.end() )
    {
        if(myIterator->second==listRow)
            binocular=myIterator->first;

        ++myIterator;
    }

    laserWindow->graphicsView->scene->clearSelection();
    binocular->setSelected(true);

    BinocularPropertiesDialog dialog(binocular, laserWindow->myDockControls->getWavelength(), this);
    dialog.exec();

    if(dialog.result()==QDialog::Accepted)
    {
        double exendedOpticalDiameter=binocular->getExendedOpticalDiameter();
        bool binocularInZone=laserpoint->shapeEnhacedPathContainsPoint(laserpoint->mapFromScene(binocular->pos()), exendedOpticalDiameter);
        binocular->setInZone(binocularInZone);
        binocular->laserParametersChanged();
        setMaxEhnacedOpticalDiameter();

        if(footprint!=nullptr)
        {
            QList<FootprintObject*>::iterator myIterator; // iterator
            myIterator = myFootprints.begin();

            while (myIterator != myFootprints.end() )
            {
                footprint=*myIterator;
                footprint->laserParameterChanged();
                ++myIterator;
            }
        }
        setShadowZone();
    }
}

void MainWindow::selectInspectorFromList()
{
    if(beamInspector==0)
        return;

    QModelIndex index=laserWindow->myDockReflectorsList->getModelBeamInspectorIndex();
    int listRow=index.row();
    QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
    myIterator = myBeamInspectors.begin();

    while (myIterator != myBeamInspectors.end() )
    {
        if(myIterator->second==listRow)
            beamInspector=myIterator->first;

        ++myIterator;
    }

    laserWindow->graphicsView->scene->clearSelection();
    beamInspector->setSelected(true);

    BeamInspectorDialog dialog(beamInspector, this);
    QPointF position=beamInspector->pos();
    dialog.exec();

    if(dialog.result()==QDialog::Rejected)
        beamInspector->setPos(position);
    else
    {
        beamInspector->setDescription(dialog.ui->descriptionTextEdit->toPlainText());
        qDebug()<< "Descrizione segnaposto: "<<dialog.ui->descriptionTextEdit->toPlainText();
    }
}

void MainWindow::propertyFromList()
{
    laserWindow->graphicsView->scene->clearSelection();
    laserpoint->setSelected(true);

    LaserPropertiesDialog dialog(laserpoint, this);
    dialog.exec();

    if(dialog.result()==QDialog::Accepted)
        updateForCondMeteo();
}

void MainWindow::environmentFromList()
{
    if(environmentModel->getState())
    {
        laserWindow->graphicsView->scene->clearSelection();
        myLabRoom->setSelected(true);

        LabEditDialog dialog(myLabRoom, this);
        dialog.exec();
    }
}

void MainWindow::setCondMeteo()
{
    double wavelength=laserWindow->myDockControls->getWavelength();
    AtmosphericEffectsDialog dialog(this, laserWindow, wavelength);
    dialog.exec();
    if(dialog.result()==QDialog::Accepted)       
    {
        double A = dialog.getA();
        double V = dialog.getV();
        double atmAttCoeff = dialog.getAtmAttCoeff();
        addMeteoCommand = new AddMeteoCommand(laserWindow, wavelength, A,
                                        V, atmAttCoeff, environmentModel);
        undoStack->push(addMeteoCommand);

        updateForCondMeteo();
    }
}

void MainWindow::updateForCondMeteo()
{
    setLambertianMaxForReflector();
    setDNRO_ForLaserpoint();
    setDNRC_ForLaserpoint();
    setDNRO_ForReflector();
    setDNRO_ForBinocular();
    setDNRO_ForFootprint();
}

void MainWindow::atmosphericEffects()
{
    double wavelength=laserWindow->myDockControls->getWavelength();
    bool atmEffects= addAtmosphericEffectsAct->isChecked();
    atmosphericEffectsOn(atmEffects);

    if(atmEffects)
    {
        if((wavelength>=400)&&(wavelength<=2000))
            statusBar()->showMessage(tr("Attenuazione atmosferica impostata"), 2000);
        else
            statusBar()->showMessage(tr("Attenuazione atmosferica non applicabile"), 2000);
    }
    else
    {
        if((wavelength>=400)&&(wavelength<=2000))
            statusBar()->showMessage(tr("Attenuazione atmosferica disabilitata"), 2000);
    }

    updateForCondMeteo();

    if(footprint!=nullptr)
    {
        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();

        while (myIterator != myFootprints.end() )
        {
        footprint=*myIterator;
        footprint->laserParameterChanged();
        ++myIterator;
        }
    }
    setShadowZone();
    setWindowModified(true);
}

void MainWindow::scintillation()
{
    bool scintillation= addScintillationAct->isChecked();
    scintillationOn(scintillation);
    updateForCondMeteo();

    if(footprint!=nullptr)
    {
        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();

        while (myIterator != myFootprints.end() )
        {
            footprint=*myIterator;
            footprint->laserParameterChanged();
            ++myIterator;
        }
    }
    setShadowZone();
    setWindowModified(true);
}

void MainWindow::properties()
{
    LaserPoint *laserpoint = selectedLaserPoint();
    Reflector *reflector = selectedReflector();
    Binocular *binocular = selectedBinocular();
    BeamInspector *beamInspector = selectedBeamInspector();
    LabRoom *myLabRoom = selectedLab();
    FootprintObject *footprint= selectedFootprint();

    if (laserpoint)
    {
        LaserPropertiesDialog dialog(laserpoint, this);
        dialog.exec();
        if(dialog.result()==QDialog::Accepted)
        {
            QPointF position=QPointF(dialog.ui->xSpinBox->value(), dialog.ui->ySpinBox->value());
            int installationIndex=dialog.ui->installationComboBox->currentIndex();
            double apertureAngle=dialog.ui->areaSpinBox->value();
            bool filterOn=dialog.ui->filterCheckBox->checkState();
            double transmittance=dialog.ui->transmittanceSpinBox->value();

            QUndoCommand* addLaserPropertyCommand= new AddLaserPropertyCommand(laserpoint, position, installationIndex,
                                     apertureAngle, filterOn, transmittance);

            undoStack->push(addLaserPropertyCommand);

            updateForCondMeteo();
        }
    }
    else if(reflector)
    {
        ReflectorPropertiesDialog dialog(reflector, this);
        dialog.exec();
        if(dialog.result()==QDialog::Accepted)
        {
            QPointF position=QPointF(dialog.ui->xSpinBox->value(), dialog.ui->ySpinBox->value());
            double reflection=dialog.ui->reflectionSpinBox->value();
            int positioning =dialog.ui->positioningLabel->text().toInt();
            QString descriptionTextEdit=dialog.ui->descriptionTextEdit->toPlainText();

            QUndoCommand* addReflectorPropertyCommand = new AddReflectorPropertyCommand(reflector, position, reflection, positioning, descriptionTextEdit);

            undoStack->push(addReflectorPropertyCommand);
        }

    }
    else if(binocular)
    {
        BinocularPropertiesDialog dialog(binocular, laserWindow->myDockControls->getWavelength(), this);
        dialog.exec();
        if(dialog.result()==QDialog::Accepted)
        {
            QPointF position = QPointF(dialog.ui->xSpinBox->value(), dialog.ui->ySpinBox->value());
            double amplification=dialog.ui->ampSpinBox->value();
            double transmittance=dialog.ui->transmittanceSpinBox->value();
            QString descriptionText=dialog.ui->descriptionTextEdit->toPlainText();
            double D0Value=dialog.ui->D0_SpinBox->value();
            double wavelength=laserWindow->myDockControls->getWavelength();

            QUndoCommand *addBinocularPropertyCommand=new AddBinocularPropertyCommand(binocular, wavelength, position,
                                        amplification, transmittance, descriptionText,
                                        D0Value);

            undoStack->push(addBinocularPropertyCommand);

            double exendedOpticalDiameter=binocular->getExendedOpticalDiameter();
            bool binocularInZone=this->laserpoint->shapeEnhacedPathContainsPoint(this->laserpoint->mapFromScene(binocular->pos()), exendedOpticalDiameter);
            binocular->setInZone(binocularInZone);
            binocular->laserParametersChanged();

            setMaxEhnacedOpticalDiameter();

            if(footprint!=nullptr)
            {
                QList<FootprintObject*>::iterator myIterator; // iterator
                myIterator = myFootprints.begin();

                while (myIterator != myFootprints.end() )
                {
                    footprint=*myIterator;
                    footprint->laserParameterChanged();
                    ++myIterator;
                }
            }
            setShadowZone();
        }
    }
    else if(myLabRoom)
    {
        LabEditDialog dialog(myLabRoom, this);
        dialog.exec();

        if(dialog.result()==QDialog::Accepted)
        {
            double lab_x=dialog.ui->xSpinBox->value();
            double lab_y=dialog.ui->ySpinBox->value();
            double rectWidth=dialog.ui->widthSpinBox->value();
            double rectHeight=dialog.ui->heightSpinBox->value();

            QUndoCommand * addLabPropertyCommand=new AddLabPropertyCommand(myLabRoom,
                        lab_x, lab_y, rectWidth, rectHeight);
            undoStack->push(addLabPropertyCommand);
        }

    }
    else if(footprint)
    {
        FootprintDialog dialog(footprint, this);
        dialog.exec();

        if(dialog.result()==QDialog::Accepted)
        {
            double rectWidth=dialog.ui->widthSpinBox->value();
            double rectHeight=dialog.ui->heightSpinBox->value();
            double x=dialog.ui->xSpinBox->value();
            double y=dialog.ui->ySpinBox->value();
            QString description=dialog.ui->descriptionTextEdit->toPlainText();

            QUndoCommand* addFootprintPropertyCommand= new AddFootprintPropertyCommand(footprint,
                               x, y, rectWidth, rectHeight, description);
            undoStack->push(addFootprintPropertyCommand);
        }
            shadowZoneForLaser();
    }
    else if(beamInspector)
    {
        BeamInspectorDialog dialog(beamInspector, this);
        QPointF position=beamInspector->pos();
        dialog.exec();

        if(dialog.result()==QDialog::Rejected)
            beamInspector->setPos(position);
        else
        {
            beamInspector->setDescription(dialog.ui->descriptionTextEdit->toPlainText());
            qDebug()<< "Descrizione segnaposto: "<<dialog.ui->descriptionTextEdit->toPlainText();
        }
    }
    else
        installationDescription();

}

void MainWindow::installationDescription()
{
    Description dialog(this);

    dialog.ui->FA_ComboBox->setCurrentText(laserWindow->getForce());
    dialog.ui->customerLineEdit->setText(laserWindow->getCustomer());
    dialog.ui->UASL_LineEdit->setText(laserWindow->get_UASL());
    dialog.ui->UASL_AssistantTextEdit->setText(laserWindow->get_UASL_Assistant());
    dialog.ui->laserTextEdit->setText(laserWindow->getLaserDescription());
    dialog.ui->descriptionTextEdit->setText(laserWindow->getPlaceDescription());

    dialog.exec();
    if(dialog.result()==QDialog::Accepted)
    {
        laserWindow->setForce(dialog.ui->FA_ComboBox->currentText());
        laserWindow->setCustomer(dialog.ui->customerLineEdit->text());
        laserWindow->set_UASL(dialog.ui->UASL_LineEdit->text());
        laserWindow->set_UASL_Assistant(dialog.ui->UASL_AssistantTextEdit->toPlainText());
        laserWindow->setLaserDescription(dialog.ui->laserTextEdit->toPlainText());
        laserWindow->setPlaceDescription(dialog.ui->descriptionTextEdit->toPlainText());
        setWindowModified(true);
      }
}
void MainWindow::createActions()
{
    QFont font;
    font.setPointSize(8);

    menuBar()->setFont(font);
    //Crea il menu file
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setFont(font);

    newAct = new QAction(tr("&Nuovo"), this);
    newAct->setIcon(QIcon(":/images/new.png"));
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Crea un nuovo progetto"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    fileMenu->addAction(newAct);
    separatorAction = fileMenu->addSeparator();

    const QIcon openIcon = QIcon::fromTheme("open project", QIcon(":/images/open.png"));
    openAct = new QAction(openIcon, tr("&Apri..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Apre il progetto"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    saveAct = new QAction(saveIcon, tr("&Salva..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Salva il progetto"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);

    saveAsAct = new QAction(tr("Salva con nome..."), this);
    saveAsAct->setStatusTip(tr("Salva con nome il progetto"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);

    separatorAction = fileMenu->addSeparator();

    exportImageAct= new QAction(tr("Esporta la selezione della scena ..."), this);
    exportImageAct->setStatusTip(tr("Esporta la selezione della scena "));
    exportImageAct->setIcon(QIcon(":/images/image.png"));
    exportImageAct->setCheckable(true);
    exportImageAct->setChecked(false);
    connect(exportImageAct, &QAction::triggered, this, &MainWindow::setImagePreview);
    fileMenu->addAction(exportImageAct);

    setPreviewAct= new QAction(tr("Imposta area di stampa ..."), this);
    setPreviewAct->setStatusTip(tr("Imposta area di stampa "));
    setPreviewAct->setIcon(QIcon(":/images/preview.png"));
    setPreviewAct->setCheckable(true);
    setPreviewAct->setChecked(false);
    connect(setPreviewAct, &QAction::triggered, this, &MainWindow::setPrintPreview);
    fileMenu->addAction(setPreviewAct);

    printPreviewAct= new QAction(tr("Anteprima di stampa ..."), this);
    printPreviewAct->setStatusTip(tr("Anteprima di stampa"));
    connect(printPreviewAct, &QAction::triggered, this, &MainWindow::on_printPreviewAction_triggered);
    fileMenu->addAction(printPreviewAct);

    exportReportAct= new QAction(tr("Esporta report ..."), this);
    exportReportAct->setStatusTip(tr("Esporta il report in formato odt "));
    connect(exportReportAct, &QAction::triggered, this, &MainWindow::exportReport);
    fileMenu->addAction(exportReportAct);

    const QIcon printIcon = QIcon::fromTheme("Stampa", QIcon(":/images/print.png"));
    printAct = new QAction(printIcon, tr("Stampa..."), this);
    printAct->setStatusTip(tr("Stampa"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);
    fileMenu->addAction(printAct);

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);

    fileMenu->addSeparator();

    descriptionAct= new QAction(tr("Descrizione installazione ..."), this);
    descriptionAct->setStatusTip(tr("Consente di inserire informazioni sull'installazione "));
    descriptionAct->setShortcut(tr("Ctrl+I"));
    connect(descriptionAct, &QAction::triggered, this, &MainWindow::installationDescription);
    fileMenu->addAction(descriptionAct);

    fileMenu->addSeparator();

    quitAct = fileMenu->addAction(tr("&Esci"), this, &QWidget::close);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Esce dall-applicativo"));

    viewMenu = menuBar()->addMenu(tr("&Visualizza"));

    viewMenu ->setFont(font);

    zoomMenu = viewMenu->addMenu(tr("&Zoom"));
    zoomMenu ->setFont(font);

    zoomInAction=new QAction(tr("Ingrandisci"), this);
    zoomInAction->setIcon(QIcon(":/images/zoomin.png"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(sceneScaleUp()));
    zoomInAction->setStatusTip(tr("Ingrandisce la vista"));
    zoomMenu->addAction(zoomInAction);

    zoomOutAction=new QAction(tr("Diminuisci"), this);
    zoomOutAction->setIcon(QIcon(":/images/zoomout.png"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(sceneScaleDown()));
    zoomOutAction->setStatusTip(tr("Diminuisce la vista"));
    zoomMenu->addAction(zoomOutAction);

    zoomMenu->addSeparator();

    QActionGroup *zoomGroup = new QActionGroup(this);

    for (int i = 0; i < nScales; ++i)
    {
        QString zoomScale=scales.at(i);
        zoomActions[i] = new QAction(zoomScale, this);
        zoomActions[i]  ->setCheckable(true);

        connect(zoomActions[i], &QAction::triggered, this, [zoomScale, i, this]() {menuSceneScaleChanged(zoomScale, i);});

        zoomMenu ->addAction(zoomActions[i]);
        zoomGroup->addAction(zoomActions[i]);

        if(zoomScale=="100%")
           zoomActions[i]->setChecked(true);
    }

    displayMenu = viewMenu->addMenu(tr("Display"));
    displayMenu ->setFont(font);

    QActionGroup *displayGroup = new QActionGroup(this);

    const QIcon dragIcon = QIcon::fromTheme("Scrolla", QIcon(":/images/drag.png"));
    dragAct = new QAction(dragIcon, tr("Scrolla..."), this);
    dragAct->setStatusTip(tr("Scrolla la zona"));
    connect(dragAct, &QAction::triggered, this, &MainWindow::dragMode);
    dragAct->setCheckable(true);
    //view->addAction(dragAct);
    displayMenu->addAction(dragAct);

    const QIcon selectionIcon = QIcon::fromTheme("Seleziona", QIcon(":/images/selection.png"));
    selectAct = new QAction(selectionIcon, tr("Seleziona gli elementi grafici..."), this);
    selectAct->setStatusTip(tr("Seleziona gli elementi grafici"));
    connect(selectAct, &QAction::triggered, this, &MainWindow::selectionMode);
    selectAct->setCheckable(true);
    selectAct->setChecked(true);
    displayMenu->addAction(selectAct);

    displayGroup->addAction(dragAct);
    displayGroup->addAction(selectAct);

    viewMenu->addSeparator();

    addDockWidget(Qt::LeftDockWidgetArea, laserWindow->myDockControls);
    showDockWidgetControls = new QAction(tr("Visualizza i controlli"), this);
    showDockWidgetControls = laserWindow->myDockControls->toggleViewAction();
    showDockWidgetControls->setIcon(QIcon(":/images/controls.png"));
    showDockWidgetControls->setStatusTip(tr("Visualizza i controlli"));
    viewMenu->addAction(showDockWidgetControls);

    addDockWidget(Qt::BottomDockWidgetArea, laserWindow->myDockResults);
    showDockWidgetResults = new QAction(tr("Visualizza risultati D.Lgs 81/2008"), this);
    showDockWidgetResults = laserWindow->myDockResults->toggleViewAction();
    showDockWidgetResults->setIcon(QIcon(":/images/eye.png"));
    showDockWidgetResults->setStatusTip(tr("Visualizza risultati D.Lgs 81/2008 relativi agli effetti sull'occhio"));
    viewMenu->addAction(showDockWidgetResults);

    addDockWidget(Qt::BottomDockWidgetArea, laserWindow->myDockSkin);
    showDockWidgetSkin = new QAction(tr("Visualizza risultati D.Lgs 81/2008 relativi agli effetti sulla cute"), this);
    showDockWidgetSkin = laserWindow->myDockSkin->toggleViewAction();
    showDockWidgetSkin->setIcon(QIcon(":/images/skin.png"));
    showDockWidgetSkin->setStatusTip(tr("Visualizza risultati D.Lgs 81/2008 relativi agli effetti sulla cute"));
    viewMenu->addAction(showDockWidgetSkin);

    addDockWidget(Qt::BottomDockWidgetArea, laserWindow->myDockEffects);
    showDockWidgetEffects = new QAction(tr("Visualizza parametri ed effetti"), this);
    showDockWidgetEffects = laserWindow->myDockEffects->toggleViewAction();
    showDockWidgetEffects->setIcon(QIcon(":/images/results.png"));
    showDockWidgetEffects->setStatusTip(tr("Visualizza parametri ed effetti"));
    viewMenu->addAction(showDockWidgetEffects);

    addDockWidget(Qt::BottomDockWidgetArea, laserWindow->myDockLea);
    showDockLea = new QAction(tr("Visualizza risultati i risultati della classificazione CEI EN 60825-1"), this);
    showDockLea = laserWindow->myDockLea->toggleViewAction();
    showDockLea->setIcon(QIcon(":/images/classifier.png"));
    showDockLea->setStatusTip(tr("Visualizza risultati i risultati della classificazione CEI EN 60825-1"));
    viewMenu->addAction(showDockLea);

    tabifyDockWidget(laserWindow->myDockEffects,laserWindow->myDockSkin);
    tabifyDockWidget(laserWindow->myDockSkin,laserWindow->myDockResults);
    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);

    addDockWidget(Qt::RightDockWidgetArea, laserWindow->myDockGoggle);
    showDockWidgetGoggle = new QAction(tr("Visualizza calcolo marcatura DPI"), this);
    showDockWidgetGoggle = laserWindow->myDockGoggle->toggleViewAction();
    showDockWidgetGoggle->setIcon(QIcon(":/images/glasses.png"));
    showDockWidgetGoggle->setStatusTip(tr("Visualizza calcolo marcatura occhiali protettori"));
    viewMenu->addAction(showDockWidgetGoggle);

    addDockWidget(Qt::RightDockWidgetArea, laserWindow->myDockReflectorsList);
    showReflectorsList = new QAction(tr("Visualizza la lista dei riflettori"), this);
    showReflectorsList = laserWindow->myDockReflectorsList->toggleViewAction();
    showReflectorsList->setIcon(QIcon(":/images/list.png"));
    showReflectorsList->setStatusTip(tr("Visualizza la lista dei riflettori"));
    viewMenu->addAction(showReflectorsList);

    addDockWidget(Qt::RightDockWidgetArea, myDockHistory);
    showDockHistory = new QAction(tr("Visualizza la cronologia dei comandi"), this);
    showDockHistory = myDockHistory->toggleViewAction();
    showDockHistory->setIcon(QIcon(":/images/history.png"));
    showDockHistory->setStatusTip(tr("Visualizza la cronologia dei comandi"));
    viewMenu->addAction(showDockHistory);

    tabifyDockWidget(laserWindow->myDockReflectorsList, laserWindow->myDockGoggle);  
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);

    viewMenu->addSeparator();
    showGridAction=new QAction(tr("Griglia"), this);
    connect(showGridAction, SIGNAL(triggered()), this, SLOT(backgroundGrid()));  
    showGridAction->setStatusTip(tr("Visualizza la griglia"));
    showGridAction->setIcon(QIcon(":/images/grid.png"));
    showGridAction->setCheckable(true);
    showGridAction->setChecked(false);
    viewMenu->addAction(showGridAction);

    centerOnViewAction=new QAction(tr("Centra la vista"), this);
    centerOnViewAction->setIcon(QIcon(":/images/point.png"));

    connect(centerOnViewAction, SIGNAL(triggered()), this, SLOT(goToPoint()));

    centerOnViewAction->setStatusTip(tr("Centra la vista nel punto specificato"));
    viewMenu->addAction(centerOnViewAction);

    darkThemeAct = new QAction(tr("Dark thema"), this);
    darkThemeAct->setStatusTip(tr("Imposta un tema con tonalità scure"));
    connect(darkThemeAct, &QAction::triggered, this, &MainWindow::changeGuiTheme);
    darkThemeAct->setCheckable(true);
    darkThemeAct->setChecked(theme);
    viewMenu->addAction(darkThemeAct);

    reflectorsEditMenu = menuBar()->addMenu(tr("&Dettagli scena"));
    reflectorsEditMenu ->setFont(font);

    undoAction = undoStack->createUndoAction(this, tr("&Annulla"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    reflectorsEditMenu->addAction(undoAction);

    redoAction = undoStack->createRedoAction(this, tr("&Ripristina"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    reflectorsEditMenu->addAction(redoAction);

    changeMeteoAct= new QAction(tr("Specifica meteo"), this);
    changeMeteoAct->setIcon(QIcon(":/images/meteo.png"));
    changeMeteoAct->setStatusTip(tr("Consente di specificare le condizioni meteo"));

    connect(changeMeteoAct, SIGNAL(triggered()), this, SLOT(setCondMeteo()));
    reflectorsEditMenu->addAction(changeMeteoAct);

    bringToFrontAction = new QAction(tr("Metti in primo piano"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    bringToFrontAction->setStatusTip(tr("Porta l'elemento grafico in primo piano"));

    connect(bringToFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
    reflectorsEditMenu->addAction(bringToFrontAction);

    sendToBackAction = new QAction(tr("&Metti in secondo piano"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    sendToBackAction->setStatusTip(tr("Porta l'elemento grafico in secondo piano"));

    connect(sendToBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));
    reflectorsEditMenu->addAction(sendToBackAction);

    deleteAction = new QAction(tr("&Cancella"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setStatusTip(tr("Elimina l'elemento grafico"));
    deleteAction->setShortcut(tr("Del"));

    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(shadowZoneForLaser()));
    reflectorsEditMenu->addAction(deleteAction);

    wetTargetAction = new QAction(tr("Dettagli riflettore bagnato..."), this);

    connect(wetTargetAction, SIGNAL(triggered()), this, SLOT(wetTarget()));
    wetTargetAction->setStatusTip(tr("Dettagli del riflettore bagnato"));
    reflectorsEditMenu->addAction(wetTargetAction);

    fresnelTargetAction = new QAction(tr("Dettagli riflettore di vetro..."), this);

    connect(fresnelTargetAction, SIGNAL(triggered()), this, SLOT(fresnelTarget()));
    fresnelTargetAction->setStatusTip(tr("Dettagli del riflettore di vetro"));
    reflectorsEditMenu->addAction(fresnelTargetAction);

    lambertianTargetAction = new QAction(tr("Detagli riflettore lambertiano..."), this);
    connect(lambertianTargetAction, SIGNAL(triggered()),
            this, SLOT(lambertianTarget()));
    lambertianTargetAction->setStatusTip(tr("Dettagli del riflettore di lambertiano"));
    reflectorsEditMenu->addAction(lambertianTargetAction);

    reflectorsEditMenu->addSeparator();
    propertiesAction = new QAction(tr("Proprietà..."), this);

    connect(propertiesAction, SIGNAL(triggered()), this, SLOT(properties()));
    propertiesAction->setStatusTip(tr("Proprietà dell'elemento selezionato"));
    reflectorsEditMenu->addAction(propertiesAction );

    environmentMenu= menuBar()->addMenu(tr("&Ambiente"));
    environmentMenu ->setFont(font);

    placeMenu= environmentMenu ->addMenu(tr("Tipo"));
    placeMenu ->setFont(font);

    setPolygonAct = new QAction(tr("Poligono di tiro militare"), this);
    setPolygonAct->setIcon(QIcon(":/images/polygon.png"));
    setPolygonAct ->setCheckable(true);
    setPolygonAct->setStatusTip(tr("Visualizza un ambiente esterno come ad esempio un poligono di tiro"));

    connect(setPolygonAct, SIGNAL(triggered()), this, SLOT(setPolygon()));
    placeMenu->addAction(setPolygonAct);

    addLabAct = new QAction(tr("Laboratorio"), this);
    addLabAct->setIcon(QIcon(":/images/room.png"));
    addLabAct->setStatusTip(tr("Visualizza un ambiente chiuso ad esempio un laboratorio"));
    addLabAct->setCheckable(true);
    connect(addLabAct, SIGNAL(triggered()), this, SLOT(addRoom()));
    placeMenu->addAction(addLabAct);

    QActionGroup *environmentGroup = new QActionGroup(this);

    environmentGroup->addAction(setPolygonAct);
    environmentGroup->addAction(addLabAct);
    setPolygonAct->setChecked(true);

    goggleMenu= environmentMenu ->addMenu(tr("Protettori ottici"));
    goggleMenu ->setFont(font);

    onlyReflectorGoggleAction=new QAction(tr("Riflettori semplici"), this);
    onlyReflectorGoggleAction->setStatusTip(tr("Riflettori semplici"));
    onlyReflectorGoggleAction->setCheckable(true);
    LaserGoggle::material myGoggleMaterial= LaserGoggle::ONLY_REFLECTOR;
    connect(onlyReflectorGoggleAction, &QAction::triggered, this, [myGoggleMaterial, this]() {setGoggleMaterial(myGoggleMaterial);});

    goggleMenu->addAction(onlyReflectorGoggleAction);

    glassGoggleAction=new QAction(tr("Vetro"), this);
    glassGoggleAction->setStatusTip(tr("Vetro"));
    glassGoggleAction->setCheckable(true);
    myGoggleMaterial= LaserGoggle::GLASS;
    connect(glassGoggleAction, &QAction::triggered, this, [myGoggleMaterial, this]() {setGoggleMaterial(myGoggleMaterial);});

    goggleMenu->addAction(glassGoggleAction);

    plasticGoggleAction=new QAction(tr("Plastica"), this);
    plasticGoggleAction->setStatusTip(tr("Plastica"));
    plasticGoggleAction->setCheckable(true);
    myGoggleMaterial= LaserGoggle::PLASTIC;
    connect(plasticGoggleAction, &QAction::triggered, this, [myGoggleMaterial, this]() {setGoggleMaterial(myGoggleMaterial);});

    goggleMenu->addAction(plasticGoggleAction);

    QActionGroup *goggleGroup = new QActionGroup(this);

    goggleGroup->addAction(onlyReflectorGoggleAction);
    goggleGroup->addAction(glassGoggleAction);
    goggleGroup->addAction(plasticGoggleAction);
    onlyReflectorGoggleAction->setChecked(true);

    reflectorsMenu= environmentMenu ->addMenu(tr("Aggiungi riflettore"));
    reflectorsMenu->setFont(font);

    addPinInspectorAction = new QAction(tr("Segnaposto di ispezione"), this);
    addPinInspectorAction->setIcon(QIcon(":/images/inspector.png"));
    connect(addPinInspectorAction, SIGNAL(triggered()), this, SLOT(addBeamInspector()));
    addPinInspectorAction->setStatusTip(tr("Aggiunge segnaposto di ispezione"));
    environmentMenu->addAction(addPinInspectorAction);

    addWetReflectorAction = new QAction(tr("Riflettore bagnato"), this);
    addWetReflectorAction->setIcon(QIcon(":/images/wet.png"));
    addWetReflectorAction->setStatusTip(tr("Aggiunge un riflettore bagnato alla scena"));
    target myTarget= WET_TARGET;

    /*connessione con un'espressione lambda (una espressione lambda è una funzione senza nome).
     * myTarget e this sono i parametri da catturare () senza parametri perchè non vengono passati parametri
     * alla funzione*/
    connect(addWetReflectorAction, &QAction::triggered, this, [myTarget, this]() {addReflector(myTarget);});

    reflectorsMenu->addAction(addWetReflectorAction);

    addGlassReflectorAction = new QAction(tr("Riflettore di vetro"), this);
    addGlassReflectorAction->setIcon(QIcon(":/images/window.png")); 
    addGlassReflectorAction->setStatusTip(tr("Aggiunge un riflettore di vetro alla scena"));
    myTarget= GLASS_TARGET;

    /*connessione con un'espressione lambda (una espressione lambda è una funzione senza nome).
     * myTarget e this sono i parametri da catturare () senza parametri perchè non vengono passati parametri
     * alla funzione*/
    connect(addGlassReflectorAction, &QAction::triggered, this, [myTarget, this]() {addReflector(myTarget);});

    reflectorsMenu->addAction(addGlassReflectorAction);

    addLambertianReflectorAction = new QAction(tr("Riflettore lambertiano"), this);
    addLambertianReflectorAction->setIcon(QIcon(":/images/lambertian.png"));
    addLambertianReflectorAction->setStatusTip(tr("Aggiunge un riflettore di lambertiano alla scena"));
    myTarget=LAMBERTIAN_TARGET;

    /*connessione con un'espressione lambda (una espressione lambda è una funzione senza nome).
     * myTarget e this sono i parametri da catturare () senza parametri perchè non vengono passati parametri
     * alla funzione*/
    connect(addLambertianReflectorAction, &QAction::triggered, this, [myTarget, this]() {addReflector(myTarget);});

    reflectorsMenu->addAction(addLambertianReflectorAction);


    addMirrorReflectorAction = new QAction(tr("Specchio"), this);
    addMirrorReflectorAction->setIcon(QIcon(":/images/mirror.png"));    
    addMirrorReflectorAction->setStatusTip(tr("Aggiunge uno specchio alla scena"));
    myTarget= MIRROR_TARGET;

    /*connessione con un'espressione lambda (una espressione lambda è una funzione senza nome).
     * myTarget e this sono i parametri da catturare () senza parametri perchè non vengono passati parametri
     * alla funzione*/
    connect(addMirrorReflectorAction, &QAction::triggered, this, [myTarget, this]() {addReflector(myTarget);});

    reflectorsMenu->addAction(addMirrorReflectorAction);

    addBinocularAct = new QAction(tr("Dispositivo ottico"), this);
    addBinocularAct->setIcon(QIcon(":/images/binocular.png"));
    addBinocularAct->setStatusTip(tr("Aggiunge un dispositivo ottico"));
    connect(addBinocularAct, SIGNAL(triggered()), this, SLOT(addBinocular()));

    environmentMenu->addAction(addBinocularAct);

    addFootprintAct = new QAction(tr("Ingombro"), this);
    addFootprintAct->setIcon(QIcon(":/images/package.png"));
    addFootprintAct->setStatusTip(tr("Aggiunge un ingombro"));
    connect(addFootprintAct, SIGNAL(triggered()), this, SLOT(addFootprint()));

    environmentMenu->addAction(addFootprintAct);

    addAtmosphericEffectsAct = new QAction(tr("Attenuazione atmosferica"), this);
    addAtmosphericEffectsAct->setIcon(QIcon(":/images/atmeffects.png"));
    addAtmosphericEffectsAct->setStatusTip(tr("Valuta gil effetti di attenuazione atmosferica"));
    addAtmosphericEffectsAct->setCheckable(true);
    addAtmosphericEffectsAct->setChecked(false);
    connect(addAtmosphericEffectsAct, SIGNAL(triggered()), this, SLOT(atmosphericEffects()));
    environmentMenu->addAction(addAtmosphericEffectsAct);

    addScintillationAct = new QAction(tr("Scintillazione"), this);
    addScintillationAct->setIcon(QIcon(":/images/scintillation.png"));
    addScintillationAct->setStatusTip(tr("Valuta effetti di scintillazione"));
    addScintillationAct->setCheckable(true);
    addScintillationAct->setChecked(false);
    connect(addScintillationAct, SIGNAL(triggered()), this, SLOT(scintillation()));

    environmentMenu->addAction(addScintillationAct);

    prospectiveMenu =menuBar()->addMenu(tr("&Prospettive"));
    prospectiveMenu->setFont(font);

    laserSettingsAction = new QAction(tr("Valutazione esposizione"), this);
    connect(laserSettingsAction, SIGNAL(triggered()), this, SLOT(setupLaserProspective()));   
    laserSettingsAction->setStatusTip(tr("Valutazione dei valori di esposizione massima permessa"));
    laserSettingsAction ->setCheckable(true);

    fieldSettingsAction = new QAction(tr("Studio dell'installazione"), this);
    connect(fieldSettingsAction, SIGNAL(triggered()), this, SLOT(setupFieldProspective()));
    fieldSettingsAction->setStatusTip(tr("Studio e verifica dell'installazione laser"));
    fieldSettingsAction ->setCheckable(true);

    classifierSettingsAction = new QAction(tr("Classifica semplificata"), this);
    connect(classifierSettingsAction, SIGNAL(triggered()), this, SLOT(setupClassifierProspective()));
    classifierSettingsAction->setStatusTip(tr("Risultati classificazione apparecchiatura laser"));
    classifierSettingsAction ->setCheckable(true);

    goggleSettingsAction = new QAction(tr("Scelta dispositivi di protezione"), this);
    connect(goggleSettingsAction, SIGNAL(triggered()), this, SLOT(setupGoggleProspective()));
    goggleSettingsAction->setStatusTip(tr("Scelta dispositivi di protezione con il calcolo della marcatura"));
    goggleSettingsAction ->setCheckable(true);

    prospectiveMenu->addAction(laserSettingsAction);
    prospectiveMenu->addAction(goggleSettingsAction);
    prospectiveMenu->addAction(fieldSettingsAction);
    prospectiveMenu->addAction(classifierSettingsAction);

    QActionGroup *prospectiveGroup = new QActionGroup(this);

    prospectiveGroup->addAction(laserSettingsAction);
    prospectiveGroup->addAction(fieldSettingsAction);
    prospectiveGroup->addAction(goggleSettingsAction);
    prospectiveGroup->addAction(classifierSettingsAction);
    laserSettingsAction->setChecked(true);

    helpMenu = menuBar()->addMenu(tr("&Aiuto"));
    helpMenu->setFont(font);

    onLineHelpAct=helpMenu->addAction(tr("&Guida in linea"), this, &MainWindow::onLineHelp);
    onLineHelpAct->setStatusTip(tr("Guida in linea"));
    onLineHelpAct->setShortcut(QKeySequence::HelpContents);

    aboutAct = helpMenu->addAction(tr("&Informazioni"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Fornisce informazioni sull'applicazione"));

    aboutQtAct = helpMenu->addAction(tr("Informazioni su &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Ambiente di sviluppo per applicazioni C++ con licenza GPL"));
}

void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    myDockHistory=new DockHistory();
    myDockHistory->ui->scrollArea->setWidget(undoView);
    undoView->setWindowTitle(tr("Command List"));
    undoView->setCleanIcon(QIcon(":/images/ok.png"));
    undoView->setEmptyLabel("Inizio cronologia comandi");
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::backgroundGrid()
{
    if(showGridAction->isChecked())
    {
        QRectF viewportRect= laserWindow->graphicsView->viewport()->rect();
        QPointF center=viewportRect.center();
        viewportRect.translate(-center);

        gridlines = new GridLines();
        gridlines->setPos(QPointF(0.0, 0.0));
        gridlines->setSceneRect(viewportRect);
        laserWindow->graphicsView->scene->addItem(gridlines);
        --minZ;
        gridlines->setZValue(minZ);

        backgroundGridPixmap();
    }
    else
    {
        if(gridlines!=nullptr)
        {
            laserWindow->graphicsView->scene->removeItem(gridlines);
            delete gridlines;
            gridlines=nullptr;
        }
    }
        laserWindow->graphicsView->scene->update();
        laserWindow->graphicsView->update();
        laserWindow->setGridState(showGridAction->isChecked());
}

void MainWindow::backgroundGridPixmap()
{
    scaleNumber=sceneScaleCombo->currentIndex();

    switch(scaleNumber)
    {
        case(0):
        gridUnit=640;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 640");
        break;

        case(1):
        gridUnit=320;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 320");
        break;

        case(2):
        gridUnit=160;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 160");
        break;

        case(3):
        gridUnit=80;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 80");
        break;

        case(4):
        gridUnit=40;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 40");
        break;

        case(5):
        gridUnit=20;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 20");
        break;

        case(6):
        gridUnit=10;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 10");
            break;

        case(7):
        gridUnit=5;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 5");
        break;

        case(8):
        gridUnit=4;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 4");
        break;

        case(9):
        gridUnit=3;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 3");
        break;

        case(10):
        gridUnit=2;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 2");
        break;

        case(11):
        gridUnit=2;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 2");
        break;

        case(12):
        gridUnit=2;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 2");
        break;

        case(13):
        gridUnit=1;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 1,0");
        break;

        default:
        gridUnit=1;
        gridlines->setGridLines(gridUnit);
        gridlines->setTextLabel("Passo griglia [m]= 1,0");
        break;
    }
}

void MainWindow::setPrintPreview()
{
    bool previewChecked=setPreviewAct->isChecked();
    if(previewChecked)
    {
        laserpoint->setEnabled(false);
        laserWindow->graphicsView->setBackgroundBrush(QColor(220, 220, 220));
        enableControlsAndItems(false);
        connect(laserWindow->graphicsView, SIGNAL(mouseRelease()), this, SLOT(setSelectionRect()));
    }
}

void MainWindow::setSelectionRect()
{
    previewRect=laserWindow->graphicsView->getSelectionRect();
    qDebug() << "Preview rect: " << previewRect;
    statusBar()->showMessage(tr("Area di stampa impostata"), 2000);
    disconnect(laserWindow->graphicsView, SIGNAL(mouseRelease()), this, SLOT(setSelectionRect()));
    laserWindow->graphicsView->setBackgroundBrush(Qt::NoBrush);
    enableControlsAndItems(true);
    setPreviewAct->setChecked(false);
}

void MainWindow::setImagePreview()
{
    bool previewChecked=exportImageAct->isChecked();
    if(previewChecked)
    {
        laserpoint->setEnabled(false);
        //laserWindow->graphicsView->setBackgroundBrush(QColor(220, 220, 220));
        enableControlsAndItems(false);
        connect(laserWindow->graphicsView, SIGNAL(mouseRelease()), this, SLOT(setImageRect()));
    }
}

void MainWindow::setImageRect()
{
    QPainter myPainter;
    previewImage=laserWindow->graphicsView->getSelectionRect();
    qDebug() << "Image rect: " << previewImage;
    QSize mySceneImageSize= QSize(previewImage.width(), previewImage.height());
    QImage mySceneImage = QImage(mySceneImageSize, QImage::Format::Format_RGB32);
    QRect imageRect=mySceneImage.rect();
    mySceneImage.fill(0);
    mySceneImage.invertPixels();

    myPainter.begin(&mySceneImage);

    laserWindow->graphicsView->render(&myPainter, imageRect, previewImage);

    QString imageName = QFileDialog::getSaveFileName(this, tr("Esporta selezione scena"),
                               "../senza nome.png",
                               tr("Immagini (*.png *.xpm *.jpg)"));
    if (!imageName.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        mySceneImage.save(imageName);      
        statusBar()->showMessage(tr("Esportazione selezione scena terminata"), 2000);
    }

    disconnect(laserWindow->graphicsView, SIGNAL(mouseRelease()), this, SLOT(setImageRect()));
    //laserWindow->graphicsView->setBackgroundBrush(Qt::NoBrush);
    enableControlsAndItems(true);
    exportImageAct->setChecked(false);
    myPainter.end();
    QApplication::restoreOverrideCursor();
}

void MainWindow::setupLaserProspective()
{
    showDockWidgetEffects->setChecked(true);
    laserWindow->myDockEffects->show();

    showDockWidgetResults->setChecked(true);
    laserWindow->myDockResults->show();

    showDockWidgetSkin->setChecked(true);
    laserWindow->myDockSkin->show();

    showDockWidgetControls->setChecked(true);
    laserWindow->myDockControls->show();

    showDockWidgetGoggle->setChecked(false);
    laserWindow->myDockGoggle->hide();

    showReflectorsList->setChecked(false);
    laserWindow->myDockReflectorsList->hide();

    showDockLea->setChecked(false);
    laserWindow->myDockLea->hide();
}

void MainWindow::setupFieldProspective()
{
    showDockWidgetControls->setChecked(false);
    laserWindow->myDockControls->hide();

    showDockWidgetResults->setChecked(false);
    laserWindow->myDockResults->hide();

    showDockWidgetEffects->setChecked(false);
    laserWindow->myDockEffects->hide();

    showDockWidgetSkin->setChecked(false);
    laserWindow->myDockSkin->hide();

    showDockWidgetGoggle->setChecked(false);
    laserWindow->myDockGoggle->hide();

    showReflectorsList->setChecked(true);
    laserWindow->myDockReflectorsList->show();

    showDockLea->setChecked(false);
    laserWindow->myDockLea->hide();
}

void MainWindow::setupGoggleProspective()
{
    showDockWidgetControls->setChecked(true);
    laserWindow->myDockControls->show();

    showDockWidgetResults->setChecked(false);
    laserWindow->myDockResults->hide();

    showDockWidgetEffects->setChecked(false);
    laserWindow->myDockEffects->hide();

    showDockWidgetSkin->setChecked(false);
    laserWindow->myDockSkin->hide();

    showDockWidgetGoggle->setChecked(true);
    laserWindow->myDockGoggle->show();

    showReflectorsList->setChecked(false);
    laserWindow->myDockReflectorsList->hide();

    showDockLea->setChecked(false);
    laserWindow->myDockLea->hide();
}

void MainWindow::setupClassifierProspective()
{
    showDockWidgetControls->setChecked(true);
    laserWindow->myDockControls->show();

    showDockWidgetResults->setChecked(false);
    laserWindow->myDockResults->hide();

    showDockWidgetEffects->setChecked(false);
    laserWindow->myDockEffects->hide();

    showDockWidgetSkin->setChecked(false);
    laserWindow->myDockSkin->hide();

    showDockWidgetGoggle->setChecked(false);
    laserWindow->myDockGoggle->hide();

    showReflectorsList->setChecked(false);
    laserWindow->myDockReflectorsList->hide();

    showDockLea->setChecked(true);
    laserWindow->myDockLea->show();
}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel("Pronto");
    QFont font;
    font.setPointSize(8);
    statusLabel->setFont(font);
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusBar()->addWidget(statusLabel);
}

void MainWindow::createContextMenu()
{
    laserWindow->setContextMenuPolicy(Qt::ActionsContextMenu);
    laserWindow->addAction(deleteAction);
    laserWindow->addAction(propertiesAction);
    laserWindow->addAction(changeMeteoAct);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    setWindowModified(false);

    QString shownName = tr("Senza nome");

    if (!curFile.isEmpty())
    {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Laser Studio")));
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!laserWindow->writeFile(fileName))
    {
        statusBar()->showMessage(tr("Salvataggio annullato"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File salvato"), 2000);
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    if (!laserWindow->readFile(fileName))
    {
        statusBar()->showMessage(tr("Caricamento annullato"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

void MainWindow::readSettings()
{
    QSettings settings("Carmine Giordano", "Laser Studio");

    settings.beginGroup("mainWindow");
    resize(settings.value("size").toSize());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    /*****************************************************************
     * Application output segnala che non funziona quindi l'ho tolto *
     * restoreState(settings.value("state").toByteArray());          *
     *****************************************************************/

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("Carmine Giordano", "Laser Studio");

    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("theme", theme);

    /*****************************************************************
     * Application output segnala che non funziona quindi l'ho tolto *
     * settings.setValue("state", saveState());                      *
     *****************************************************************/

    settings.setValue("recentFiles", recentFiles);
    settings.endGroup();
}

QString MainWindow::strippedName(const QString &fullFileName)
{
        return QFileInfo(fullFileName).fileName();
}

void MainWindow::laserModified()
{
    setWindowModified(true);
    //updateStatusBar();
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count())
        {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
            recentFileActions[j]->setVisible(false);
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(5, 5, 5, 5));
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    printReport(&printer);
}

void MainWindow::on_printPreviewAction_triggered()
{
    #if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printpreviewdialog)

        QPrinter printer(QPrinter::HighResolution);
        printer.setPageMargins(QMarginsF(5, 5, 5, 5));

        QPrintPreviewDialog preview(&printer, this);
        preview.setWindowTitle(tr("Anteprima di stampa"));
        preview.setMinimumSize(400, 800);
        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printReport);
        preview.exec();

    #endif
}

void MainWindow::exportReport()
{

    myLaserReport=new LaserReport(laserWindow, laserpoint, LaserReport::ODF);
    myLaserReport->setReflectorsList(myReflectors);
    myLaserReport->setFootprintsList(myFootprints);
    myLaserReport->setBinocularsList(myBinoculars);
    myLaserReport->setIndoor(environmentModel->getState());

    saveReportImages();

    myLaserReport->setReflectorsFilenameList(reflectorsFilenameList);
    myLaserReport->setReflectorsGraphImageList(reflectorsGraphImageList);

    QTextDocument *textDocument=myLaserReport->buildReportDocument();

    QString exportName = QFileDialog::getSaveFileName(this, tr("Esporta in formato odt"),
                               "../senza nome.odt",
                               tr("Documento (*.odt)"));
    if (!exportName.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QTextDocumentWriter myodf(exportName);
        myodf.setFormat("ODF");
        myodf.setFileName(exportName);
        myodf.write(textDocument);
        statusBar()->showMessage(tr("Esportazione documento terminata"), 2000);
        QApplication::restoreOverrideCursor();
    }

    delete textDocument;
    delete myLaserReport;
}

void MainWindow::printReport(QPrinter *printer)
{
    #if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)

        myLaserReport=new LaserReport(laserWindow, laserpoint, LaserReport::PDF);
        myLaserReport->setReflectorsList(myReflectors);
        myLaserReport->setFootprintsList(myFootprints);
        myLaserReport->setBinocularsList(myBinoculars);
        myLaserReport->setIndoor(environmentModel->getState());

        saveReportImages();

        myLaserReport->setReflectorsFilenameList(reflectorsFilenameList);
        myLaserReport->setReflectorsGraphImageList(reflectorsGraphImageList);

        QTextDocument *textDocument=myLaserReport->buildReportDocument();
        textDocument->adjustSize();
        /***************************************************************************
        * La larghezza del testo di textDocument la imposto ad 800 px.             *
        * Per l'oggetto istanziato da QTextDocument l'unità di misura dipende      *
        * dal dispositivo paint impiegato. Quando il dispositivo su cui si disegna *
        * è lo schermo, come in questo caso, l'unità di misura è il pixel logico,  *
        * quando il dispositivo è una stampante come avverrà in fase di stampa,    *
        * l'unità di misura è il punto (1 punto corrisponde ad  1/72 di pollice.   *
        * In questo caso perciò non si considera l'alta definizione della          *
        * stampante ma la risoluzione logica dello schermo.                        *
        * *************************************************************************/

        /**************************************************************************
        * Stampo                                                                  *
        * *************************************************************************/

        textDocument->print(printer);


        delete textDocument;
        delete myLaserReport;

    #endif
}

void MainWindow::saveReportImages()
{
    QImage mySceneImage = QImage(mySceneImageSize, QImage::Format::Format_RGB32);

    mySceneImage.fill(0);
    mySceneImage.invertPixels();

    qDebug()<< "Dot per metri dell'immagine nella direzione X" << mySceneImage.dotsPerMeterX();

    QPainter myPainter;

    qDebug()<<"Larghezza dell'immagine da immagine: " <<mySceneImage.width();
    myPainter.begin(&mySceneImage);
    myPainter.setRenderHint(QPainter::Antialiasing);

    QRect rectImage=mySceneImage.rect();

    qDebug()<<"View Rectangle: "<< previewRect;
    laserWindow->graphicsView->render(&myPainter, rectImage, previewRect);


    myPainter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
    myPainter.drawRect(rectImage);

    /**************************************************************************
    * salvo l'immagine (png) e ne verifico il salvataggio (in fase di debug)  *
    * *************************************************************************/

    if(mySceneImage.save("sceneImg.png", "PNG"))
        qDebug()<<"Imagine salvata";


    /**************************************************************************
    * Effettuo il flush di painter anche se non strettamente necessario per   *
    * rendere il codice più leggibile                                         *
    ***************************************************************************/

    myPainter.end();

    /**************************************************************************
     * L'immagine impiegata per le tabelle non puù essere ridimensionata      *
     * pertanto ne creo un'altra delle dimensioni dei due grafici da          *
     * utilizzare per gli occhiali protettori.                                *
     **************************************************************************/

    QSize myChartGoggleImageSize(laserWindow->myDockControls->getChartView()->width(),
                      laserWindow->myDockControls->getChartView()->height());

    QImage myChartGoggleImage = QImage(myChartGoggleImageSize, QImage::Format::Format_RGB32);

    /**************************************************************************
     * Per impiegare una nuova immagine devo caricarla come dispostivo di     *
     * output per painter.                                                    *
     **************************************************************************/

    myPainter.begin(&myChartGoggleImage);
    myChartGoggleImage.fill(0);
    myChartGoggleImage.invertPixels();

    laserWindow->myDockControls->getChartView()->scene()->render(&myPainter);
    if(myChartGoggleImage.save("chartViewImg.png", "PNG"))
        qDebug()<<"Imagine salvata";

    laserWindow->myDockControls->getdChartView()->scene()->render(&myPainter);
    if(myChartGoggleImage.save("dChartViewImg.png", "PNG"))
        qDebug()<<"Imagine salvata";

    /**************************************************************************
    * Effettuo il flush di painter anche se non strettamente necessario per   *
    * rendere il codice più leggibile                                         *
    ***************************************************************************/

    myPainter.end();

    if(!myReflectors.empty())
    {
        QString reflectorFilename;
        QImage imageChart;

        reflectorsFilenameList.clear();
        reflectorsGraphImageList.clear();

        QList<pair<Reflector*, int>>::iterator myIterator; // iterator
        myIterator = myReflectors.begin();
        int i =1;
        while (myIterator != myReflectors.end() )
        {
        reflector=myIterator->first;

            if(reflector->getReflectorKind()==WET_TARGET)
            {
                if(reflector->getOpticalDiameter()!=0)
                {
                WetChartView *polarChartView = new WetChartView(0, reflector->getZsVect(),
                                                         reflector->getMaxElement(), reflector->getCorrectPositioning());
                polarChartView->setRenderHint(QPainter::Antialiasing);

                QSize imageChartSize=polarChartView->size();
                imageChart=QImage(imageChartSize, QImage::Format::Format_RGB32);
                myPainter.begin(&imageChart);
                imageChart.fill(0);
                imageChart.invertPixels();

                polarChartView->setRadialAxis();
                polarChartView->setAngularAxis();
                polarChartView->scene()->render(&myPainter);

                reflectorFilename="reflectorChartImg"+QString::number(i)+".png";

                if(imageChart.save(reflectorFilename, "PNG"))
                    qDebug()<<"Imagine salvata";

                         myPainter.end();
                }
            }
            else
            if(reflector->getReflectorKind()==GLASS_TARGET)
                {
                if(reflector->getOpticalDiameter()!=0)
                {
                    FresnelChartView *polarChartView = new FresnelChartView(0, reflector->getZsVect(),
                                                           reflector->getMaxElement(), reflector->getCorrectPositioning());
                    polarChartView->setRenderHint(QPainter::Antialiasing);
                    polarChartView->setRadialAxis();
                    polarChartView->setAngularAxis();
                    QSize imageChartSize=polarChartView->size();
                    imageChart=QImage(imageChartSize, QImage::Format::Format_RGB32);
                    myPainter.begin(&imageChart);
                    imageChart.fill(0);
                    imageChart.invertPixels();
                    polarChartView->scene()->render(&myPainter);

                    reflectorFilename="reflectorChartImg"+QString::number(i)+".png";

                    if(imageChart.save(reflectorFilename, "PNG"))
                        qDebug()<<"Imagine salvata";

                    myPainter.end();
                }
            }
            else if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
            {
            if(reflector->getOpticalDiameter()!=0)
            {
                QSize imageChartSize;
                if(!reflector->isExendedDiffusion())
                {
                    LambertianChartDialog dialog(reflector, this);
                    dialog.getLambertianPointChart();

                    imageChartSize=dialog.getLambertianPointChart()->size();
                    imageChart=QImage(imageChartSize, QImage::Format::Format_RGB32);
                    myPainter.begin(&imageChart);
                    imageChart.fill(0);
                    imageChart.invertPixels();

                    dialog.getLambertianPointChart()->scene()->render(&myPainter);
                    reflectorFilename="reflectorChartImg"+QString::number(i)+".png";

                    if(imageChart.save(reflectorFilename, "PNG"))
                        qDebug()<<"Imagine salvata";

                    myPainter.end();
                }
                else
                {
                    LambertianChartDialog dialog(reflector, this);

                    imageChartSize=dialog.getLambertianExendedChart()->size();

                    imageChart=QImage(imageChartSize, QImage::Format::Format_RGB32);
                    myPainter.begin(&imageChart);
                    imageChart.fill(0);
                    imageChart.invertPixels();
                    dialog.getLambertianExendedChart()->scene()->render(&myPainter);
                    reflectorFilename="reflectorChartImg"+QString::number(i)+".png";

                    if(imageChart.save(reflectorFilename, "PNG"))
                        qDebug()<<"Imagine salvata";
                    myPainter.end();
                 }
                }
            }
            else if(reflector->getReflectorKind()==MIRROR_TARGET)
                imageChart=QImage();

            reflectorsGraphImageList.append(imageChart);
            reflectorsFilenameList.append(reflectorFilename);
            ++myIterator;
            ++i;
        }
    }
}

void MainWindow::setSceneArea(QRect sceneArea) const
{
    laserWindow->graphicsView->scene->setSceneRect(sceneArea);
}

void MainWindow::menuSceneScaleChanged(const QString &scale, const int &index)
{
    sceneScaleChanged(scale);
    sceneScaleCombo->setCurrentIndex(index);
}

void MainWindow::barSceneScaleChanged(const QString &scale)
{ 
    int index=sceneScaleCombo->currentIndex();
    sceneScaleChanged(scale);
    zoomActions[index]->setChecked(true);
}

void MainWindow::updateScale()
{
    laserpoint->setPixScale(scale);

    if(myLabRoom)
    myLabRoom->setPixScale(scale);

    if(reflector)
    {
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
        while (myIterator != myReflectors.end() )
        {
            reflector=myIterator->first;
            reflector->setPixScale(scale);
            ++myIterator;
        }
    }

    if(binocular)
    {
    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
        while (myIterator != myBinoculars.end() )
        {
            binocular=myIterator->first;
            binocular->setPixScale(scale);
            ++myIterator;
        }
    }

    if(beamInspector)
    {
        QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
        myIterator = myBeamInspectors.begin();
        while (myIterator != myBeamInspectors.end() )
        {
            beamInspector=myIterator->first;
            beamInspector->setPixScale(scale);
            ++myIterator;
        }
    }


    if(footprint)
    {
        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();
        while (myIterator != myFootprints.end() )
        {
            footprint=*myIterator;
            footprint->setItemScale(scale);
            footprint->setTransform(myTransform);
            ++myIterator;
        }
    }
}

void MainWindow::sceneScaleChanged(const QString &_scale)
{
    scale = _scale.left(_scale.indexOf(tr("%"))).toDouble() / 100.0;
    QGraphicsView *view = new QGraphicsView();
    view=laserWindow->graphicsView;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    laserWindow->setScale(scale);
    int index=sceneScaleCombo->currentIndex();
    laserWindow->setScaleIndex(index);
    myTransform = laserWindow->graphicsView->transform();

    updateScale();

    if(showGridAction->isChecked())
    {
        setViewportRect();
        backgroundGridPixmap();
    }
}

void MainWindow::sceneScaleUp()
{
    int index=sceneScaleCombo->currentIndex();

    QString indexScale;
    ++index;

    if(index>=nScales )
        return;


    QGraphicsView *view = new QGraphicsView();
    view=laserWindow->graphicsView;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());

    indexScale=scales.at(index);
    scale=indexScale.left(indexScale.indexOf(tr("%"))).toDouble() / 100.0;

    view->scale(scale, scale);

    zoomActions[index]->setChecked(true);
    sceneScaleCombo->setCurrentIndex(index);

    updateScale();

    if(gridlines!=nullptr)
    {
        backgroundGridPixmap();
        setViewportRect();
    }

    if(showGridAction->isChecked())
    {        
        backgroundGridPixmap();
        setViewportRect();
    }
}

void MainWindow::sceneScaleDown()
{
    int index=sceneScaleCombo->currentIndex();

    QString indexScale;
    --index;

    if(index<0)
        return;

    QGraphicsView *view = new QGraphicsView();
    view=laserWindow->graphicsView;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());

    indexScale=scales.at(index);
    scale=indexScale.left(indexScale.indexOf(tr("%"))).toDouble() / 100.0;

    view->scale(scale, scale);
    zoomActions[index]->setChecked(true);
    sceneScaleCombo->setCurrentIndex(index);

    updateScale();

    if(gridlines!=nullptr)
        setViewportRect();

    if(showGridAction->isChecked())
        backgroundGridPixmap();
}

void MainWindow::dragMode()
{
    dragModeState=true;
    laserWindow->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    laserpoint->setFlag(QGraphicsItem::ItemIsMovable, false);
    laserpoint->setFlag(QGraphicsItem::ItemIsSelectable, false);
    laserpoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    if(myLabRoom!=0)
    {
        myLabRoom->setFlag(QGraphicsItem::ItemIsMovable, false);
        myLabRoom->setFlag(QGraphicsItem::ItemIsSelectable, false);
        myLabRoom->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    addWetReflectorAction->setEnabled(false);
    addGlassReflectorAction->setEnabled(false);
    addMirrorReflectorAction->setEnabled(false);
    addLambertianReflectorAction->setEnabled(false);
    addLabAct->setEnabled(false);
    addBinocularAct->setEnabled(false);

    if(reflector==0)
        return;

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();

    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        reflector->setFlag(QGraphicsItem::ItemIsMovable, false);
        reflector->setFlag(QGraphicsItem::ItemIsSelectable, false);
        reflector->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        ++myIterator;
    }

    if(binocular==0)
        return;

    QList<pair<Binocular*, int>>::iterator myBinocularIterator; // iterator
    myBinocularIterator = myBinoculars.begin();

    while(myBinocularIterator != myBinoculars.end())
    {
        binocular=myBinocularIterator->first;
        binocular->setFlag(QGraphicsItem::ItemIsMovable, false);
        binocular->setFlag(QGraphicsItem::ItemIsSelectable, false);
        binocular->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        ++myBinocularIterator;
    }
}

void MainWindow::selectionMode()
{
        dragModeState=false;
        laserWindow->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        laserpoint->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

        if(myLabRoom!=0)
           myLabRoom->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

        addWetReflectorAction->setEnabled(true);
        addGlassReflectorAction->setEnabled(true);
        addMirrorReflectorAction->setEnabled(true);
        addLambertianReflectorAction->setEnabled(true);
        addLabAct->setEnabled(true);
        addBinocularAct->setEnabled(true);

        if(reflector==0)
            return;

        QList<pair<Reflector*, int>>::iterator myIterator; // iterator
        myIterator = myReflectors.begin();

        while (myIterator != myReflectors.end() )
        {
            reflector=myIterator->first;
            reflector->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
            ++myIterator;
        }

        if(binocular==0)
            return;

        QList<pair<Binocular*, int>>::iterator myBinocularIterator; // iterator
        myBinocularIterator = myBinoculars.begin();

        while(myBinocularIterator != myBinoculars.end())
        {
            binocular=myBinocularIterator->first;
                binocular->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
                ++myBinocularIterator;
        }
}

Reflector *MainWindow::selectedReflector() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<Reflector *>(items.first());
    else
        return 0;
}

Binocular *MainWindow::selectedBinocular() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<Binocular *>(items.first());
    else
        return 0;
}

LabRoom *MainWindow::selectedLab() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<LabRoom *>(items.first());
    else
        return 0;
}

FootprintObject *MainWindow::selectedFootprint() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<FootprintObject *>(items.first());
    else
        return 0;
}

LaserPoint *MainWindow::selectedLaserPoint() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<LaserPoint *>(items.first());
    else
        return 0;
}

BeamInspector *MainWindow::selectedBeamInspector() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<BeamInspector *>(items.first());
    else
        return 0;
}

Link *MainWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<Link *>(items.first());
    else
        return 0;
}

MainWindow::BinocularNodePair MainWindow::selectedBinocularNodePair() const
{
    return BinocularNodePair(laserpoint, binocular);
}

MainWindow::ObjectNodePair MainWindow::selectedObjectNodePair() const
{
    return ObjectNodePair(laserpoint, footprint);
}

void MainWindow::addBeamInspectorLink()
{
    InspectorNodePair inspectorNodes=selectedInspectorNodePair();
    if (inspectorNodes == InspectorNodePair())
        return;

    LaserPoint* laser= inspectorNodes.first;
    BeamInspector* inspector= inspectorNodes.second;

    InspectorLink *inspectorlink = new InspectorLink(laser, inspector);
    laserWindow->graphicsView->scene->addItem(inspectorlink);
}

MainWindow::InspectorNodePair MainWindow::selectedInspectorNodePair() const
{
    return InspectorNodePair(laserpoint, beamInspector);
}

void MainWindow::addBinocular()
{
    QPointF binocularPos=QPointF(laserpoint->pos().x()+ (150.0/scale * ((binSeqNumber+1) % 5)),
                        laserpoint->pos().y()+(50.0/scale * (((binSeqNumber+1) / 5) % 11)));

    double binocularPosX=binocularPos.x();
    double binocularPosY=binocularPos.y();

    double laserPosX=laserpoint->pos().x();
    double laserPosY=laserpoint->pos().y();

    double binocularDistance=sqrtf(powf((binocularPosX-laserPosX), 2)+powf((binocularPosY-laserPosY), 2));
    double attenuatedDNRO= attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());


    addBinocularCommand = new AddBinocularCommand(attenuatedDNRO, binocularDistance, scale,
                                                            binSeqNumber, laserWindow, laserpoint,
                                                            &myBinoculars, binocularsModel,
                                                            binocularPos);

    undoStack->push(addBinocularCommand);

    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(binocularPos, QTransform());
    binocular= qgraphicsitem_cast<Binocular*>(item);

    setMaxEhnacedOpticalDiameter();
    binocular->laserParametersChanged();

    binSeqNumber++;

    if(footprint!=nullptr)
    {
        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();

        while (myIterator != myFootprints.end() )
        {
            footprint=*myIterator;
            footprint->laserParameterChanged();
            ++myIterator;
        }
    }

    setShadowZone();
    setWindowModified(true);

    connect(binocular, SIGNAL(xChanged()), this, SLOT(updateBinocularList()));
    connect(binocular, SIGNAL(yChanged()), this, SLOT(updateBinocularList())); 
    connect(binocular, SIGNAL(xChanged()), this, SLOT(setLaserpointShapePathForBinoculars()));
    connect(binocular, SIGNAL(yChanged()), this, SLOT(setLaserpointShapePathForBinoculars()));

    //emit binocularListChanged();
}

void MainWindow::addBeamInspector()
{
    QPointF inspectorPos=QPointF(laserpoint->pos().x()+ (150.0/scale * ((inspectorSeqNumber+1) % 5)),
                        laserpoint->pos().y()+(50.0/scale * (((inspectorSeqNumber+1) / 5) % 11)));

    double inspectorPosX=inspectorPos.x();
    double inspectorPosY=inspectorPos.y();
    double laserPosX=laserpoint->pos().x();
    double laserPosY=laserpoint->pos().y();

    double inspectorDistance=sqrtf(powf((inspectorPosX-laserPosX), 2)+powf((inspectorPosY-laserPosY), 2));

    addBeamInspectorCommand = new AddBeamInspectorCommand(inspectorDistance, scale, inspectorSeqNumber, laserWindow,
                                   laserpoint, &myBeamInspectors, inspectorsModel,  inspectorPos);

    undoStack->push(addBeamInspectorCommand);
    QPointF shiftPosition =inspectorPos-BeamInspector::positionShift(scale);
    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(shiftPosition, QTransform());
    beamInspector= qgraphicsitem_cast<BeamInspector*>(item);

    beamInspector->laserParametersChanged();
    inspectorSeqNumber++;

    setWindowModified(true);

    connect(beamInspector, SIGNAL(xChanged()), this, SLOT(updateInspectorList()));
    connect(beamInspector, SIGNAL(yChanged()), this, SLOT(updateInspectorList()));
}

void MainWindow::addReflector(const target &target)
{    
    qDebug() << "Container's count before addReflector: " << myReflectors.count();
    QPointF reflectorPos=QPointF(laserpoint->pos().x()+ (100.0/scale * ((seqNumber+1) % 5)),
                        laserpoint->pos().y()+(50.0/scale * (((seqNumber+1) / 5) % 7)));

    double reflectorPosX=reflectorPos.x();
    double reflectorPosY=reflectorPos.y();

    double laserPosX=laserpoint->pos().x();
    double laserPosY=laserpoint->pos().y();

    double reflectorDistance=sqrtf(powf((reflectorPosX-laserPosX), 2)+powf((reflectorPosY-laserPosY), 2));
    double attenuatedDNRO= attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());
    double attenuatedDNRC= attenuatedDistance(laserWindow->myDockControls->getSkinDistances());

    addReflectorCommand = new AddReflectorCommand(attenuatedDNRO, attenuatedDNRC, reflectorDistance, scale,
                                                            seqNumber, target, laserWindow, laserpoint,
                                                            &myReflectors, reflectorsModel,
                                                            QPointF(reflectorPosX, reflectorPosY));
    undoStack->push(addReflectorCommand);
    //int index=undoStack->index();
    //const AddCommand* command=dynamic_cast<const AddCommand*>(undoStack->command(index-1));
    //reflector=command->getReflector();
    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(reflectorPos, QTransform());
    reflector= qgraphicsitem_cast<Reflector*>(item);
    reflector->setUndoStack(undoStack);
    setMaxEhnacedOpticalDiameter();
    setLaserpointShapePathForReflectors();

    seqNumber++;

    connect(reflector, SIGNAL(xChanged()), this, SLOT(updateList()));
    connect(reflector, SIGNAL(yChanged()), this, SLOT(updateList()));
    connect(reflector, SIGNAL(xChanged()), this, SLOT(setLaserpointShapePathForReflectors()));
    connect(reflector, SIGNAL(yChanged()), this, SLOT(setLaserpointShapePathForReflectors()));

    //emit reflectorListChanged();
}

void MainWindow::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return;

    Link *link = new Link(nodes.first, nodes.second);
    laserWindow->graphicsView->scene->addItem(link);
}

MainWindow::NodePair MainWindow::selectedNodePair() const
{
    return NodePair(laserpoint, reflector);
}

void MainWindow::addBinocularLink()
{
    BinocularNodePair binocularNodes = selectedBinocularNodePair();
    if (binocularNodes == BinocularNodePair())
        return;

    BinocularLink *binocularlink = new BinocularLink(binocularNodes.first, binocularNodes.second);
    laserWindow->graphicsView->scene->addItem(binocularlink);
}

void MainWindow::del()
{
    QList<QGraphicsItem *> list = laserWindow->graphicsView->scene->selectedItems();
    list.first()->setSelected(false);

    Reflector *reflector= qgraphicsitem_cast<Reflector*>(list.first());
    Binocular *binocular= qgraphicsitem_cast<Binocular*>(list.first());
    BeamInspector *beamInspector =qgraphicsitem_cast<BeamInspector*>(list.first());
    FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(list.first());

    if (reflector)
    {
        QPointF deleletePosition=reflector->pos();
        Link *link=reflector->getLink();
        deleteReflectorCommand = new DeleteReflectorCommand(reflector, link, scale, laserWindow,
            laserpoint, &myReflectors, reflectorsModel, deleletePosition);

    undoStack->push(deleteReflectorCommand);
    }

     else if (binocular)
    {
        QPointF deleletePosition=binocular->pos();
        BinocularLink *binocularLink=binocular->getBinocularLink();
        deleteBinocularCommand = new DeleteBinocularCommand(binocular, binocularLink, scale, laserWindow,
            laserpoint, &myBinoculars, binocularsModel, deleletePosition);

    undoStack->push(deleteBinocularCommand);
    setMaxEhnacedOpticalDiameter();
    }

    else if (beamInspector)
   {
       QPointF deleletePosition=beamInspector->pos();
       InspectorLink *inspectorLink=beamInspector->getBeamInspectorLink();
       deleteBeamInspectorCommand = new DeleteBeamInspectorCommand(beamInspector, inspectorLink, scale, laserWindow,
           laserpoint, &myBeamInspectors, inspectorsModel, deleletePosition);

   undoStack->push(deleteBeamInspectorCommand);
   }

    else if (footprint)
   {
       QPointF deleletePosition=footprint->pos();
       ObjectLink *objectLink=footprint->getObjectLink();
       deleteFootprintCommand = new DeleteFootprintCommand(footprint, objectLink, scale, laserWindow,
           laserpoint, &myFootprints, deleletePosition);
       footprintsCount--;

       undoStack->push(deleteFootprintCommand);
   }
}

void MainWindow::cut()
{
    Reflector *reflector = selectedReflector();
    if (!reflector)
        return;

    copy();
    delete laserpoint;
}

void MainWindow::copy()
{
    Reflector *reflector = selectedReflector();
    if (!reflector)
        return;

    QString str = QString("Node %1 %2 %3 %4")
                  .arg(reflector->textColor().name())
                  .arg(reflector->outlineColor().name())
                  .arg(reflector->backgroundColor().name())
                  .arg(reflector->text());
    QApplication::clipboard()->setText(str);
}

void MainWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList parts = str.split(" ");

    if (parts.count() >= 5 && parts.first() == "Laser Point")
    {
        LaserPoint *laserpoint = new LaserPoint;
        laserpoint->setTextColor(QColor(parts[1]));
        laserpoint->setOutlineColor(QColor(parts[2]));
        laserpoint->setBackgroundColor(QColor(parts[3]));
    }
}

void MainWindow::setZValue(int z)
{
    Reflector *reflector = selectedReflector();
    LaserPoint *laserpoint = selectedLaserPoint();
    Binocular *binocular = selectedBinocular();
    FootprintObject *footprint = selectedFootprint();
    QGraphicsItem *myLab = selectedLab();

    if (reflector)
        reflector->setZValue(z);
    else if(laserpoint)
        laserpoint->setZValue(z);
    else if (binocular)
        binocular->setZValue(z);
    else if (footprint)
        footprint->setZValue(z);
    else if(myLab)
        myLab->setZValue(z);
}

void MainWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void MainWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void MainWindow::wetTarget()
{
    Reflector *reflector = selectedReflector();

    if (reflector)
    {
        WetChartDialog dialog(reflector, this);
        dialog.exec();
    }
}

void MainWindow::fresnelTarget()
{
    Reflector *reflector = selectedReflector();

    if (reflector)
    {
        FresnelChartDialog dialog(reflector, this);
        dialog.exec();
    }
}


void MainWindow::lambertianTarget()
{
    Reflector *reflector = selectedReflector();

    if (reflector)
    {
        LambertianChartDialog dialog(reflector, this);
        dialog.exec();
    }
}

void MainWindow::updateActions()
{
    bool hasSelection = !laserWindow->graphicsView->scene->selectedItems().isEmpty();
    bool isReflector = (selectedReflector() != 0);
    bool reflectorInHazardArea;
    bool isLaserPont = (selectedLaserPoint() != 0);

    deleteAction->setEnabled(hasSelection && !isLaserPont);
    bringToFrontAction->setEnabled(hasSelection);
    sendToBackAction->setEnabled(hasSelection);

    if(isReflector)
    {
        reflectorInHazardArea=laserpoint->shapePathContainsPoint(laserpoint->mapFromScene(selectedReflector()->pos()));
        bool isMirror=selectedReflector()->getReflectorKind()==MIRROR_TARGET;

        if(selectedReflector()->getReflectorKind()==WET_TARGET)
        {
            if(reflectorInHazardArea)
            {
                wetTargetAction->setEnabled(true);
                lambertianTargetAction->setEnabled(false);
                fresnelTargetAction->setEnabled(false);
            }
            else
            {
                wetTargetAction->setEnabled(false);
                lambertianTargetAction->setEnabled(false);
                fresnelTargetAction->setEnabled(false);
            }
        }
        else if(selectedReflector()->getReflectorKind()==LAMBERTIAN_TARGET)
        {
            if(reflectorInHazardArea)
            {
                wetTargetAction->setEnabled(false);
                lambertianTargetAction->setEnabled(true);
                fresnelTargetAction->setEnabled(false);
            }
            else
            {
                wetTargetAction->setEnabled(false);
                lambertianTargetAction->setEnabled(false);
                fresnelTargetAction->setEnabled(false);
            }
        }
        else if(selectedReflector()->getReflectorKind()==GLASS_TARGET)
        {
            if(reflectorInHazardArea)
            {
                wetTargetAction->setEnabled(false);
                lambertianTargetAction->setEnabled(false);
                fresnelTargetAction->setEnabled(true);
            }
            else
            {
                wetTargetAction->setEnabled(false);
                lambertianTargetAction->setEnabled(false);
                fresnelTargetAction->setEnabled(false);
            }
        }
        else if(isMirror)
        {
            wetTargetAction->setEnabled(false);
            lambertianTargetAction->setEnabled(false);
            fresnelTargetAction->setEnabled(false);
        }
    }
    else
    {
        wetTargetAction->setEnabled(false);
        lambertianTargetAction->setEnabled(false);
        fresnelTargetAction->setEnabled(false);
    }

    foreach (QAction *action, laserWindow->graphicsView->actions())
        laserWindow->graphicsView->removeAction(action);

    foreach (QAction *action, reflectorsEditMenu->actions())
    {
        if (action->isEnabled())
            laserWindow->graphicsView->addAction(action);
    }
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(exportImageAct);
    fileToolBar->addAction(setPreviewAct);
    fileToolBar->addAction(printAct);

    viewToolBar = addToolBar(tr("Visualizza"));  
    viewToolBar->setObjectName(tr("Visualizza"));
    sceneScaleCombo = new QComboBox;

    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(4);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged, this, &MainWindow::barSceneScaleChanged);
    viewToolBar->addWidget(sceneScaleCombo);

    viewToolBar->addAction(zoomInAction);
    viewToolBar->addAction(zoomOutAction);
    viewToolBar->addAction(selectAct);
    viewToolBar->addAction(dragAct);

    viewToolBar->addAction(showDockWidgetControls);
    viewToolBar->addAction(showDockWidgetResults);
    viewToolBar->addAction(showDockWidgetSkin);
    viewToolBar->addAction(showDockWidgetEffects);
    viewToolBar->addAction(showDockLea);
    viewToolBar->addAction(showDockWidgetGoggle);
    viewToolBar->addAction(showReflectorsList);
    viewToolBar->addAction(centerOnViewAction);
    viewToolBar->addAction(showGridAction);
    viewToolBar->addAction(showDockHistory);

    sceneToolBar = addToolBar(tr("Scena"));
    sceneToolBar->setObjectName(tr("Scena"));
    sceneToolBar->addAction(undoAction);
    sceneToolBar->addAction(redoAction);
    sceneToolBar->addAction(deleteAction);
    sceneToolBar->addAction(sendToBackAction);
    sceneToolBar->addAction(bringToFrontAction);
    sceneToolBar->addAction(changeMeteoAct);

    environmentToolBar = addToolBar(tr("Ambiente"));
    environmentToolBar->setObjectName(tr("Ambiente"));
    environmentToolBar->addAction(setPolygonAct);
    environmentToolBar->addAction(addLabAct);
    environmentToolBar->addAction(addBinocularAct);
    environmentToolBar->addAction(addWetReflectorAction);
    environmentToolBar->addAction(addPinInspectorAction);
    environmentToolBar->addAction(addGlassReflectorAction);
    environmentToolBar->addAction(addLambertianReflectorAction);
    environmentToolBar->addAction(addMirrorReflectorAction);
    environmentToolBar->addAction(addFootprintAct);
    environmentToolBar->addAction(addAtmosphericEffectsAct);
    environmentToolBar->addAction(addScintillationAct);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(*watched);
    if (event->type() == QEvent::MouseMove)
    {
        double xCoordinate;
        double yCoordinate;

        xCoordinate=laserWindow->graphicsView->getMousePosition().x();
        yCoordinate=laserWindow->graphicsView->getMousePosition().y();

        if(scale>=15)
        {
        QString xCoordinateString=QString::number(xCoordinate, 'f', 2);
        QString yCoordinateString=QString::number(yCoordinate, 'f', 2);
        laserWindow->label->setText(QString("Coordinate del punto (%1,%2)")
                                     .arg(xCoordinateString)
                                     .arg(yCoordinateString));
        }
        else
        {
        QString xCoordinateString=QString::number(xCoordinate, 'f', 0);
        QString yCoordinateString=QString::number(yCoordinate, 'f', 0);
        laserWindow->label->setText(QString("Coordinate del punto (%1,%2)")
                                     .arg(xCoordinateString)
                                     .arg(yCoordinateString));
        }
    }
    return false;
}

void MainWindow::noFeasibleInputFunction()
{
    statusBar()->showMessage(tr("Ingresso non fisicamente realizzabile"), 2000);
}

double MainWindow::attenuatedDistance(const double & _distance)
{
    double distance=_distance;
    double attenuatedlDistance=_distance;
    double wavelength=laserWindow->myDockControls->getWavelength();

    if(laserWindow->getAtmEffectsBool())
    {
        if((wavelength>=400)&&(wavelength<=2000))
        {
            double atmCoeff=laserWindow->getAtmoshericEffectsCoefficient();
           attenuatedlDistance=distance/(2-powf(CentralWidget::NEPERO_N, (-0.5*atmCoeff*distance)));
        }
        else
            attenuatedlDistance=distance;
    }

    if(laserWindow->getScintillationBool())
    {
        attenuatedlDistance=distance*CentralWidget::SCINTILLATION_CONST;
    }

    if(laserpoint->isFilterOn())
    {
        double transmittance=laserpoint->getTransmittance();
        attenuatedlDistance=sqrtf(transmittance)*distance;
        laserModel->myDataHasChanged();
    }

    laserpoint->setStrings();
    return attenuatedlDistance;
}

void MainWindow::setDNRO_ForLaserpoint()
{
    double attenuatedDNRO=attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());
    laserpoint->setOpticalDiameter(attenuatedDNRO);
    laserModel->myDataHasChanged();
    reflectorsModel->myDataHasChanged();
}

void MainWindow::setDNRC_ForLaserpoint()
{
    double attenuatedDNRC=attenuatedDistance(laserWindow->myDockControls->getSkinDistances());
    laserpoint->setSkinDistance(attenuatedDNRC);
    laserModel->myDataHasChanged();
    reflectorsModel->myDataHasChanged();
}

void MainWindow::setDNRO_ForReflector()
{
    if(reflector==0)
        return;

    double attenuatedDNRO=attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();

    while(myIterator != myReflectors.end())
    {
        reflector=myIterator->first;

        /***********************************************************************************
        * con mapFromScene considero le coordinate del punto nel sistema di riferimento    *
        * dell'item di origine (in questo caso reflector) e le trasformo nel sistema di    *
        * riferimento dell'item da cui invoco la funzione membro (laserpoint)              *
        ************************************************************************************/

        if(laserpoint->shapePathContainsPoint(laserpoint->mapFromScene(reflector->pos())))
            reflector->setOpticalDiameter(attenuatedDNRO);
        else
            reflector->setOpticalDiameter(0);

        ++myIterator;
    }
    updateActions();
    reflectorsModel->myDataHasChanged();
}

void MainWindow::setDNRO_ForBinocular()
{
    if(binocular==0)
        return;

    double attenuatedDNRO=attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());

    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();

    while (myIterator != myBinoculars.end() )
    {
        binocular=myIterator->first;
        //opticalDistance è la NOHD che viene moltiplicata per 2 da setDNRO_Diameter
        binocular->setDNRO_Diameter(attenuatedDNRO);
        setMaxEhnacedOpticalDiameter();
        binocular->laserParametersChanged();

        ++myIterator;
    }
    updateActions();
    binocularsModel->myDataHasChanged();
}

void MainWindow::setDNRO_ForFootprint()
{
    if(footprint==0)
        return;

    double attenuatedDNRO=attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());

    QList<FootprintObject*>::iterator myIterator; // iterator
    myIterator = myFootprints.begin();

    while (myIterator != myFootprints.end() )
    {
        footprint=*myIterator;
        //opticalDistance è la NOHD che viene moltiplicata per 2 da setDNRO_Diameter
        footprint->setDNRO_Diameter(attenuatedDNRO);
        footprint->laserParameterChanged();
        setMaxEhnacedOpticalDiameter();

        ++myIterator;
    }
    shadowZoneForLaser();
}

void MainWindow::setDivergenceForReflector()
{
    if(reflector==0)
        return;

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while(myIterator != myReflectors.end())
    {
        reflector=myIterator->first;
        reflector->setDivergence(laserWindow->myDockControls->getDivergence());
        qDebug()<< "Optical distance: " << laserWindow->myDockControls->getLambertianMax();
        ++myIterator;
    }
}

void MainWindow::setDivergenceForBinocular()
{
    if(binocular==0)
        return;

    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
    while(myIterator != myBinoculars.end())
    {
        binocular=myIterator->first;
        binocular->setDivergence(laserWindow->myDockControls->getDivergence());

        ++myIterator;
    }
    binocularsModel->myDataHasChanged();
}

void MainWindow::setWavelengthForBinocular()
{
    if(binocular==0)
        return;

    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
    while (myIterator != myBinoculars.end() )
    {
        binocular=myIterator->first;
        binocular->setWavelength(laserWindow->myDockControls->getWavelength());
        binocular->laserParametersChanged();
        ++myIterator;
    }
    binocularsModel->myDataHasChanged();
}


void MainWindow::setLambertianMaxForReflector()
{
    if(reflector==0)
        return;

    double myLambertianMax=attenuatedDistance(laserWindow->myDockControls->getLambertianMax());
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        {
            reflector->setLambertianMax(myLambertianMax);
        }
    ++myIterator;
    }
}

void MainWindow::setUpdatedPosition()
{
    pointPosition=laserpoint->pos();
}

QPointF MainWindow::getUpdatedPosition()
{
    return pointPosition;
}

void MainWindow::setDistanceForReflector()
{
    if(reflector==0)
        return;

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while(myIterator != myReflectors.end())
    {
        reflector=myIterator->first;
        if(laserpoint->shapePathContainsPoint(laserpoint->mapFromScene(reflector->pos())))
        {
            reflector->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
            reflector->setDivergence(laserWindow->myDockControls->getDivergence());
            reflector->laserPositionChanged();
            qDebug()<<"contiene il riflettore in : "<< reflector->pos() << "ed il punto laser in;" <<laserpoint->pos();
        }
        else
        {
            reflector->setOpticalDiameter(0.0);
            reflector->setDivergence(laserWindow->myDockControls->getDivergence());
            reflector->laserPositionChanged();

            qDebug()<<"non contiene il riflettore in : "<< reflector->pos() << "ed il punto laser in;" <<laserpoint->pos();
        }

        ++myIterator;
    }
    updateActions();
}


void MainWindow::setDistanceForBinocular()
{
    if(binocular==0)
        return;

    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
    while (myIterator != myBinoculars.end() )
    {
        binocular=myIterator->first;
        double exendedOpticalDiameter=binocular->getExendedOpticalDiameter();

        if(laserpoint->shapeEnhacedPathContainsPoint(laserpoint->mapFromScene(binocular->pos()), exendedOpticalDiameter))
            binocular->setInZone(true);
        else
            binocular->setInZone(false);

        binocular->laserPositionChanged();
        ++myIterator;
    }
    binocularsModel->myDataHasChanged();
    updateActions();
}

void MainWindow::setDistanceForInspector()
{
    if(beamInspector==0)
        return;

    QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
    myIterator = myBeamInspectors.begin();
    while (myIterator != myBeamInspectors.end() )
    {
        beamInspector=myIterator->first;

        beamInspector->laserPositionChanged();
        ++myIterator;
    }
    inspectorsModel->myDataHasChanged();
    updateActions();
}

void MainWindow::setDistanceForFootprint()
{
    if(footprint==0)
        return;

    QList<FootprintObject*>::iterator myIterator; // iterator
    myIterator = myFootprints.begin();
    while (myIterator != myFootprints.end() )
    {
        footprint=*myIterator;
        footprint->laserPositionChanged();
        ++myIterator;
    }
}

void MainWindow::setBeamDiameterForBinocular()
{
    if(binocular==0)
        return;

    double myBeamDiameter=laserWindow->myDockControls->getBeamDiameter();
    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
    while(myIterator != myBinoculars.end())
    {
        binocular=myIterator->first;
        binocular->setBeamDiameter(myBeamDiameter);
        ++myIterator;
    }
    binocularsModel->myDataHasChanged();
    updateActions();
}

int MainWindow::seqNumerCount()const
{
    return seqNumber;
}

/**********************************************************************************************
 * Setter per le riflessioni diffuse estese                                                   *
 **********************************************************************************************/

void MainWindow::setEMP()
{
    myEMP=laserWindow->myDockControls->getEMP();
}

void MainWindow::setPowerErg()
{
   powerErg=laserWindow->myDockControls->getPowerErg();
}

void MainWindow::setBeamDiameter()
{
    beamDiameter=laserWindow->myDockControls->getBeamDiameter();
}

/**********************************************************************************************
 * Getter per le riflessioni diffuse estese                                                   *
 **********************************************************************************************/

double MainWindow::getEMP()
{
    return laserWindow->myDockControls->getEMP();
}

double MainWindow::getPowerErg()
{
   return laserWindow->myDockControls->getPowerErg();
}

double MainWindow::getBeamDiameter()
{
    return laserWindow->myDockControls->getBeamDiameter();
}

/**********************************************************************************************
 * Imposto il rilfettore  per le riflessioni diffuse estese                                   *
 **********************************************************************************************/
void MainWindow::setReflectorEMP_ForDiffusion()
{
    if(reflector==0)
        return;

    setEMP();//funzione membro che imposta il valore dell'EMP ricavato in myDockControls
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        {
            reflector->setLaserEMP(myEMP);
        }
    ++myIterator;
    }
}

void MainWindow::setReflectorPowerErgForDiffusion()
{
    setPowerErg();//funzione membro che imposta il valore dei powerErg ricavato in myDockControls
    qDebug()<< "funzione membro che imposta il valore di powerErg ricavato in myDockControl: "<< getPowerErg();

    if(reflector==0)
        return;

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        {
            reflector->setLaserPowerErg(laserWindow->myDockControls->getPowerErgForEMP());
        }
    ++myIterator;
    }
}

void MainWindow::setReflectorBeamDiameterForDiffusion()
{
    if(reflector==0)
        return;

    setBeamDiameter();//funzione membro che imposta il valore di beamDiameter ricavato in myDockControls
    qDebug()<< "funzione membro che imposta il valore di beamDiameter ricavato in myDockControl: "<< getBeamDiameter();
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        {
            reflector->setLaserBeamDiameter(beamDiameter);
        }
    ++myIterator;
    }
}

void MainWindow::makeSceneOfSavedItems(){

    /**************************************************************************
     * I controlli vengono regolati dalla funzione readFile() di centralWidget *
     ***************************************************************************/

    //Dati membro necessari per creare laserpoint
    QPointF laserPosition;
    qreal laserZValue;
    double aperture;
    int installation;
    bool filterOn;
    double transmittance;

    //Dati membro necessari per aggiungere alla scena i riflettori salvati
    QVector <int> seqNumberVect;
    QVector <QPointF> posVect;
    QVector <int> TypeVect;
    QVector <QString> TextVect;
    QVector <QString> StringPositionVect;
    QVector <double> OpticalDiameterVect;
    QVector <double> DivergenceVect;
    QVector <double> ReflectorDistanceVect;
    QVector <double> ReflectionCoeffVect;
    QVector <qreal> ZValueVect;
    QVector <target> ReflectorKindVect;
    QVector <double> ReflectorPositioningVect;
    QVector <QString> ReflectorDescriptionVect;

    //dati relativi al gruppo di dispositivi ottici
    QVector <QPointF> binocularPosVect;
    QVector <int> binocularMagnificationVect;
    QVector <double> binocularTransmissionVect;
    QVector <int> binocular_D0Vect;
    QVector <QString > binocularDescriptionVect;

    //dati relativi ai segnaposti di ispezione
    QVector <QPointF> beamInspectorPosVect;
    QVector <QString > beamInspectorDescriptionVect;

    //dati relativi al singolo riflettore
    QPointF myPos;
    QString myText;
    QString myStringPosition;
    double myOpticalDiameter;
    double myDivergence;
    double myReflectorDistance;
    double myReflectionCoeff;
    qreal myZValue;
    target myReflectorKind;
    double myReflectorPositioning=180.0;
    QString myDescription;
    double myLaserEMP;
    double myLaserBeamDiameter;
    double myLaserPowerErg;
    double myLambertianMax;

    //dati relativi al singolo dispositivo ottico
    QPointF myBinocularPos;
    int myBinocularMagnification;
    double myBinocularTransmission;
    int myBinocular_D0;
    QString myBinocularDescription;

    //dati relativi al singolo segnaposto di ispezione
    QPointF myBeamInspectorPos;
    QString myBeamInspectorDescription;

    //dati relativi agli ingombri
    QVector <QPointF> footprintPosVect;
    QVector <QRectF> footprintRectVect;
    QVector <QString> footprintDescriptionVect;
    QPointF myFootprintPos;
    QRectF myFootprintRect;    
    QString myFootprintDescription;

    /***********************************************************************************
     * Pulisco la scena (cancellando gli oggetti ad uno ad uno con la funzione membro  *
     * clearScene() non essedo possibile con clean()) ed imposto una nuova scena       *
     * grafica.                                                                        *
     ***********************************************************************************/

    clearScene();
    laserWindow->graphicsView->scene->clear();
    laserWindow->setNewScene();

    myReflectors.clear();
    reflectorsModel->deleteList();
    reflectorsSelectionModel->clear();

    myBinoculars.clear();
    binocularsModel->deleteList();
    binocularsSelectionModel->clear();

    myBeamInspectors.clear();
    inspectorsModel->deleteList();
    inspectorsSelectionModel->clear();

    labroomList.clear();
    environmentModel->deleteList();
    environmentSelectionModel->clear();

    laserPointList.clear();
    laserModel->deleteList();
    laserSelectionModel->clear();

    myFootprints.clear();

    //riporto la variabile contatore degli elementi inseriti a 0
    seqNumber=0;
    binSeqNumber=0;
    footprintSeqNumber=0;
    inspectorSeqNumber=0;

    //leggo i valori riguradanti laserpoint
    laserPosition= laserWindow->getLaserPosition();
    laserZValue=laserWindow->getLaserZValue();
    aperture=laserWindow->getAperture();
    installation=laserWindow->getInstallation();
    filterOn=laserWindow->isFilterOn();
    transmittance=laserWindow->getTransmittance();

    //leggo i valori riguardante il laser
    myLaserEMP= laserWindow->getLaserEMP();
    myLaserBeamDiameter= laserWindow->getBeamDiameter();
    myLaserPowerErg= laserWindow->getPowerErg();
    myLambertianMax= laserWindow->getLambertianMax();

    //Visualizzo la griglia se presente durante il salvataggio del file
    scale=laserWindow->getScale();
    showGridAction->setChecked(laserWindow->getGridState());
    backgroundGrid();

    LaserGoggle::material goggleMaterial=laserWindow->myDockControls->getGoggleMaterial();
    if(goggleMaterial==LaserGoggle::GLASS)
        glassGoggleAction->setChecked(true);
    else if(goggleMaterial==LaserGoggle::PLASTIC)
        plasticGoggleAction->setChecked(true);
    else if(goggleMaterial==LaserGoggle::ONLY_REFLECTOR)
        onlyReflectorGoggleAction->setChecked(true);

    laserpoint = new LaserPoint();
    laserWindow->graphicsView->scene->addItem(laserpoint);

    laserpoint->setSelected(true);
    laserpoint->setStringPosition();
    laserpoint->setPos(laserPosition);
    laserpoint->setZValue(laserZValue);
    laserpoint->setAperture(aperture);
    laserpoint->setInstallationIndex(installation);
    laserpoint->setFilterOn(filterOn);
    laserpoint->setTrasmittance(transmittance);

    //Leggo i vettori riguardanti i riflettori
    posVect=laserWindow->getPosVect();
    TypeVect= laserWindow->getTypeVect();
    TextVect= laserWindow->getTextVect();
    StringPositionVect= laserWindow->getStringPositionVect();
    OpticalDiameterVect= laserWindow->getOpticalDiameterVect();
    DivergenceVect= laserWindow->getDivergenceVect();
    ReflectorDistanceVect= laserWindow->getReflectorDistanceVect();
    ReflectionCoeffVect=laserWindow->getReflectionCoeffVect();
    ZValueVect= laserWindow->getZValueVect();
    ReflectorKindVect= laserWindow->getReflectorKindVect();   
    ReflectorPositioningVect= laserWindow->getReflectorPositioningVect();
    ReflectorDescriptionVect= laserWindow->getDescriptionVect();

    //Leggo i vettori riguardanti i dispositivi ottici

    binocularPosVect= laserWindow->getBinocularPosVect();
    binocularMagnificationVect= laserWindow->getBinocularMagnificationVect();
    binocularTransmissionVect= laserWindow->getBinocularTransmissionVect();
    binocular_D0Vect= laserWindow->getBinocular_D0Vect();
    binocularDescriptionVect= laserWindow->getBinocularDescriptionVect();

    //Leggo i vettori riguardanti i segnaposti di ispezione
    beamInspectorPosVect=laserWindow->getBeamInspectorPosVect();
    beamInspectorDescriptionVect=laserWindow->getBeamInspectorDescriptionVect();

    //Leggo l'oggetto riguardante l'ambiente
    QRectF labRoomRect= laserWindow->getLabRect();
    QPointF labRoomPos= laserWindow->getLabPosition();
    roomNumber=laserWindow->getRoomNumber();


    //leggo i valori relativi agli ingombri
    footprintPosVect=laserWindow->getFootprintPosVect();
    footprintRectVect=laserWindow->getFootprintRectVect();
    footprintDescriptionVect=laserWindow->getFootprintDescriptionVect();

    //ricavo il numero di riflettori
    int NumberOfElements= ReflectorDistanceVect.size();

    //inizializzo la variabile a zero
    int i=0 ;
    //per ogni riflettore ripeto la procedura per la creazione, laggiunta dei link

    while(i< NumberOfElements)
    {
        myOpticalDiameter=OpticalDiameterVect.at(i);
        myPos=posVect.at(i);
        myStringPosition=StringPositionVect.at(i);
        myDivergence=DivergenceVect.at(i);
        myReflectorDistance=ReflectorDistanceVect.at(i);
        myReflectionCoeff=ReflectionCoeffVect.at(i);
        myZValue=ZValueVect.at(i);
        myReflectorKind= ReflectorKindVect.at(i);

        if(myReflectorKind!=MIRROR_TARGET)
            myReflectorPositioning =ReflectorPositioningVect[i];

        myDescription=ReflectorDescriptionVect.at(i);

        reflector = new Reflector(myOpticalDiameter,  myDivergence, myReflectorDistance,
                                   myLaserBeamDiameter,  myLaserEMP, myLaserPowerErg,
                                   myLambertianMax,  myReflectorKind);

        reflector->setPixmap();
        laserWindow->graphicsView->scene->addItem(reflector);
        reflector->setPos(myPos);

        reflector->setSeqNumber(seqNumber);
        reflector->setReflectorColor();
        reflector->setBackgroundColor(QColor(247, 247, 247, 170));
        reflector->setZValue(myZValue);
        reflector->setReflectorKindString();
        reflector->setMaterialCoeff(myReflectionCoeff);
        reflector->setDescription(myDescription);

        if(myReflectorKind==LAMBERTIAN_TARGET)
            reflector->setLambertianMax(laserWindow->getLambertianMax());

        if(myReflectorKind!=MIRROR_TARGET)
            reflector->setPositioning(myReflectorPositioning);

        addLink();

        myReflectors.append(make_pair(reflector, seqNumber));

        connect(reflector, SIGNAL(xChanged()), this, SLOT(updateList()));
        connect(reflector, SIGNAL(yChanged()), this, SLOT(updateList()));
        connect(reflector, SIGNAL(xChanged()), this, SLOT(setLaserpointShapePathForReflectors()));
        connect(reflector, SIGNAL(yChanged()), this, SLOT(setLaserpointShapePathForReflectors()));

        reflector->laserParametersChanged();
        ++seqNumber;
        ++i;
    }

    int NumberOfBinoculars= binocularPosVect.size();
    int j=0;

    while(j< NumberOfBinoculars)
    {
        myBinocularPos=binocularPosVect.at(j);
        myBinocularMagnification=binocularMagnificationVect.at(j);
        myBinocularTransmission=binocularTransmissionVect.at(j);
        myBinocular_D0=binocular_D0Vect.at(j);
        myBinocularDescription=binocularDescriptionVect.at(j);

        double binocularDistance=sqrtf(powf(myBinocularPos.x()-laserPosition.x(), 2)+powf((myBinocularPos.y()-laserPosition.y()), 2));

        //Costruttore DNRO, binocularDistance, wavelength, divergence, beamDiameter
        binocular=new Binocular(laserWindow->myDockControls->getOpticalDistance(),
                                     binocularDistance,
                                     laserWindow->myDockControls->getWavelength(),
                                     laserWindow->myDockControls->getDivergence(),
                                     laserWindow->myDockControls->getBeamDiameter());

        binocular->setPos(myBinocularPos);
        binocular->setMagnification(myBinocularMagnification);
        binocular->setTransmissionCoeff(myBinocularTransmission);
        binocular->set_D0(myBinocular_D0);
        binocular->setDescription(myBinocularDescription);

        laserWindow->graphicsView->scene->addItem(binocular);

        binocular->setTextLabel();
        binocular->setStringPosition();
        binocular->setBinSeqNumber(binSeqNumber);

        addBinocularLink();
        myBinoculars.append(make_pair(binocular, binSeqNumber));

        connect(binocular, SIGNAL(xChanged()), this, SLOT(updateBinocularList()));
        connect(binocular, SIGNAL(yChanged()), this, SLOT(updateBinocularList()));
        connect(binocular, SIGNAL(xChanged()), this, SLOT(setLaserpointShapePathForBinoculars()));
        connect(binocular, SIGNAL(yChanged()), this, SLOT(setLaserpointShapePathForBinoculars()));
        ++binSeqNumber;
        ++j;
    }

    int NumberOfInspectors= beamInspectorPosVect.size();
    int m=0;

    while(m< NumberOfInspectors)
    {
        myBeamInspectorPos=beamInspectorPosVect.at(m);
        myBeamInspectorDescription=beamInspectorDescriptionVect.at(m);

        double inspectorDistance=sqrtf(powf(myBeamInspectorPos.x()-laserPosition.x(), 2)+powf((myBeamInspectorPos.y()-laserPosition.y()), 2));

        //Costruttore DNRO, binocularDistance, wavelength, divergence, beamDiameter
        beamInspector=new BeamInspector(inspectorDistance,
                                        laserWindow->myDockControls->getWavelength(),
                                        laserWindow->myDockControls->getDivergence(),
                                        laserWindow->myDockControls->getBeamDiameter());

        beamInspector->setPixScale(scale);
        beamInspector->setPos(myBeamInspectorPos);
        beamInspector->setDescription(myBeamInspectorDescription);

        laserWindow->graphicsView->scene->addItem(beamInspector);

        beamInspector->setStringPosition();
        beamInspector->setInspectorSeqNumber(inspectorSeqNumber);

        addBeamInspectorLink();
        beamInspector->laserParametersChanged();
        myBeamInspectors.append(make_pair(beamInspector, inspectorSeqNumber));

        connect(beamInspector, SIGNAL(xChanged()), this, SLOT(updateInspectorList()));
        connect(beamInspector, SIGNAL(yChanged()), this, SLOT(updateInspectorList()));
        ++inspectorSeqNumber;
        ++m;
    }


    laserWindow->graphicsView->scene->clearSelection();
    laserPointList.clear();
    laserModel->addDescriptor(*laserpoint);
    reflectorsModel->setElementList(myReflectors);
    binocularsModel->setElementList(myBinoculars);
    inspectorsModel->setElementList(myBeamInspectors);
    int NumberOfFootprint= footprintPosVect.size();
    int k=0;

    while(k<NumberOfFootprint)
    {
        myFootprintPos=footprintPosVect.at(k);
        myFootprintRect=footprintRectVect.at(k);
        myFootprintDescription=footprintDescriptionVect.at(k);

        double attenuatedDNRO= attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());

        //Costruttore DNRO, binocularDistance
        footprint= new FootprintObject(scale);

        footprint->setPos(myFootprintPos);
        auto myRectangle=footprint->getRectangle();
        myRectangle.setRect(myFootprintRect);
        footprint->setRectangle(myRectangle);
        footprint->setDescription(myFootprintDescription);
        qDebug()<< "myFootprintDescription: "<< myFootprintDescription;

        footprint->setFootprintSeqNumber(footprintSeqNumber);
        footprint->setDNRO_Diameter(attenuatedDNRO);
        footprint->setLaserBeamPath(laserpoint->mapToItem(footprint, laserpoint->shapePath()));
        laserWindow->graphicsView->scene->addItem(footprint);

        addObjectLink();
        footprint->setFootprintSeqNumber(footprintSeqNumber);
        footprint->setLaserPosition();
        myFootprints.append(footprint);

        setMaxEhnacedOpticalDiameter();
        footprint->laserParameterChanged();
        setShadowZone();

        connect(footprint, SIGNAL(xChanged()), this, SLOT(setShadowZone()));
        connect(footprint, SIGNAL(yChanged()), this, SLOT(setShadowZone()));

        ++footprintSeqNumber;
        ++k;
    }

    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    laserpoint->setSkinDistance(laserWindow->myDockControls->getSkinDistances());

    scale=laserWindow->getScale();
    QString scaleString = QString::number(scale*100).append("%");
    int index=laserWindow->getScaleIndex();

    menuSceneScaleChanged(scaleString, index);
    labroomList.append(myFakeRoom);
    environmentModel->addDescriptor(*myFakeRoom);

    if(laserWindow->isLabRoomInserted())
    {
        bool meteoWidgetsON=false;
        myLabRoom=new LabRoom(labRoomRect);
        myLabRoom->setPos(labRoomPos);
        laserWindow->graphicsView->scene->addItem(myLabRoom);
        labroomList.append(myLabRoom);

        environmentModel->addDescriptor(*myLabRoom);
        environmentModel->setState(true);

        meteoWidgets(meteoWidgetsON, meteoWidgetsON, meteoWidgetsON);
        updateForCondMeteo();

        addScintillationAct->setEnabled(false);
        addScintillationAct->setChecked(false);
        addAtmosphericEffectsAct->setEnabled(false);
        addAtmosphericEffectsAct->setChecked(false);
        changeMeteoAct->setEnabled(false);

        addLabAct->setChecked(true);
    }
    else
    {
        bool isAtmEffects=laserWindow->getAtmEffectsBool();
        bool isScintillation=laserWindow->getScintillationBool();

        meteoWidgets(true, isAtmEffects, isScintillation);

        atmosphericEffectsOn(isAtmEffects);
        scintillationOn(isScintillation);
        environmentModel->setState(false);
        setPolygonAct->setChecked(true);
    }

    environmentModel->myDataHasChanged();

    laserWindow->myDockControls->updateGoggle();
    laserWindow->myDockControls->updateAllCompositeControlsFunctions();
    updateForCondMeteo();
    setWindowModified(false);
    updateActions();
}

void MainWindow::enableControlsAndItems(bool enabled)
{
    fileMenu->setEnabled(enabled);
    viewMenu->setEnabled(enabled);
    reflectorsMenu->setEnabled(enabled);
    reflectorsEditMenu->setEnabled(enabled);
    helpMenu->setEnabled(enabled);
    prospectiveMenu->setEnabled(enabled);
    fileToolBar->setEnabled(enabled);
    viewToolBar->setEnabled(enabled);   
    environmentToolBar->setEnabled(enabled);
    sceneToolBar->setEnabled(enabled);
    laserWindow->myDockControls->setEnabled(enabled);
    laserpoint->setEnabled(enabled);

    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        reflector->setEnabled(enabled);
        ++myIterator;
    }

    QList<pair<Binocular*, int>>::iterator myBinocularIterator; // iterator
    myBinocularIterator = myBinoculars.begin();
    while (myBinocularIterator != myBinoculars.end() )
    {
        binocular=myBinocularIterator->first;
        binocular->setEnabled(enabled);
        ++myBinocularIterator;
    }
}

void MainWindow::goToPoint()
{
    GoToPointDialog dialog(this, laserpoint->pos());
    QPointF center;

    if(dialog.exec()==1)
        {
            center=dialog.getViewCenter();
            qDebug()<< "Laser point position: "<< center;
        }

    laserWindow->graphicsView->centerOn(center);
}

void MainWindow::addElementList()
{
//    reflectorsModel->addElement(*reflector);
}

void MainWindow::addBinocularList()
{
    binocularsModel->addElement(*binocular);
}

void MainWindow::addInspectorList()
{
    int modelIndex=inspectorsModel->addElement(*beamInspector);
    myBeamInspectors.append(make_pair(beamInspector, modelIndex));
    beamInspector->setInspectorSeqNumber(modelIndex);

    inspectorsModel->myDataHasChanged();
}


void MainWindow::addLabList()
{
    environmentModel->addDescriptor(*myLabRoom);
}

void MainWindow::meteoWidgets(bool meteoEnabled, bool isAtmEffects, bool isScintillation)
{
    atmosphericEffectsOn(isAtmEffects);
    scintillationOn(isScintillation);
    addScintillationAct->setEnabled(meteoEnabled);
    addScintillationAct->setChecked(isScintillation);
    addAtmosphericEffectsAct->setEnabled(meteoEnabled);
    addAtmosphericEffectsAct->setChecked(isAtmEffects);
    changeMeteoAct->setEnabled(meteoEnabled);
}

void MainWindow::goToSelectedReflector()
{
    QModelIndex index;
    laserSelectionModel->clear();
    binocularsSelectionModel->clear();
    inspectorsSelectionModel->clear();
    environmentSelectionModel->clear();
    index=reflectorsSelectionModel->currentIndex();
    qDebug()<< "Numero di riflettori della lista: " << myReflectors.count();
    reflector=myReflectors.at(index.row()).first;
    laserWindow->graphicsView->centerOn(reflector->pos());
    laserWindow->graphicsView->scene->clearSelection();
    reflector->setSelected(true);
}

void MainWindow::goToSelectedBinocular()
{
    QModelIndex index;
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    environmentSelectionModel->clear();
    inspectorsSelectionModel->clear();
    index=binocularsSelectionModel->currentIndex();
    binocular=myBinoculars.at(index.row()).first;
    laserWindow->graphicsView->centerOn(binocular->pos());
    laserWindow->graphicsView->scene->clearSelection();
    binocular->setSelected(true);
}

void MainWindow::goToSelectedBeamInspector()
{
    QModelIndex index;
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();
    environmentSelectionModel->clear();
    index=inspectorsSelectionModel->currentIndex();
    beamInspector=myBeamInspectors.at(index.row()).first;
    laserWindow->graphicsView->centerOn(beamInspector->pos());
    laserWindow->graphicsView->scene->clearSelection();
    beamInspector->setSelected(true);
}

void MainWindow::goToLab()
{
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();
    inspectorsSelectionModel->clear();

    if(environmentModel->getState())
    {
        QModelIndex index;

        index=environmentSelectionModel->currentIndex();
        myLabRoom=labroomList.at(index.row());
        sceneScaleChanged("8000%");
        laserWindow->graphicsView->centerOn(myLabRoom->pos());
        laserWindow->graphicsView->scene->clearSelection();
        myLabRoom->setSelected(true);
    }
}

void MainWindow::gotToLaserpoint()
{
    laserWindow->graphicsView->centerOn(laserpoint->pos());
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();    
    environmentSelectionModel->clear();
    inspectorsSelectionModel->clear();
    laserWindow->graphicsView->scene->clearSelection();
    laserpoint->setSelected(true);
}

void MainWindow::updateList()
{
    reflectorsModel->myDataHasChanged();
}

void MainWindow::updateBinocularList()
{
    binocularsModel->myDataHasChanged();
}

void MainWindow::updateInspectorList()
{
    inspectorsModel->myDataHasChanged();
}

void MainWindow::updateLaserList()
{
    laserModel->myDataHasChanged();
}


void MainWindow::updateLabList()
{
    environmentModel->myDataHasChanged();
}

void MainWindow::listSelectionFromGraphics()
{
    int reflectorIndex;
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    QModelIndex selectedIndex;
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->isSelected())
        {
            reflectorIndex=myIterator->second;
            selectedIndex=reflectorsModel->index(reflectorIndex,0);
            updateActions();
         }
         ++myIterator;
    }
    laserSelectionModel->clear();
    binocularsSelectionModel->clear();
    inspectorsSelectionModel->clear();
    environmentSelectionModel->clear();
    reflectorsSelectionModel->select(selectedIndex, QItemSelectionModel::Clear | QItemSelectionModel::Select);
}


void MainWindow::binocularListSelectionFromGraphics()
{
    int binocularIndex;
    QList<pair<Binocular*, int>>::iterator myIterator; // iterator
    myIterator = myBinoculars.begin();
    QModelIndex selectedIndex;
    while (myIterator != myBinoculars.end() )
    {
        binocular=myIterator->first;
        if(binocular->isSelected())
        {
            binocularIndex=myIterator->second;
            selectedIndex=binocularsModel->index(binocularIndex,0);
            updateActions();
        }
        ++myIterator;
    }
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    inspectorsSelectionModel->clear();
    environmentSelectionModel->clear();
    binocularsSelectionModel->select(selectedIndex, QItemSelectionModel::Clear | QItemSelectionModel::Select);
}

void MainWindow::inspectorListSelectionFromGraphics()
{
    int inspectorIndex;
    QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
    myIterator = myBeamInspectors.begin();
    QModelIndex selectedIndex;
    while (myIterator != myBeamInspectors.end() )
    {
        beamInspector=myIterator->first;
        if(beamInspector->isSelected())
        {
            inspectorIndex=myIterator->second;
            selectedIndex=inspectorsModel->index(inspectorIndex,0);
            updateActions();
        }
        ++myIterator;
    }
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    environmentSelectionModel->clear();
    binocularsSelectionModel->clear();
    inspectorsSelectionModel->select(selectedIndex, QItemSelectionModel::Clear | QItemSelectionModel::Select);
}

void MainWindow::listMultipleSelectionFromGraphics()
{
    int reflectorIndex;
    QList<pair<Reflector*, int>>::iterator myIterator; // iterator
    myIterator = myReflectors.begin();
    reflectorsSelectionModel->clear();
    QModelIndex selectedIndex;
    while (myIterator != myReflectors.end() )
    {
        reflector=myIterator->first;
        if(reflector->isSelected())
        {
            reflectorIndex=myIterator->second;
            selectedIndex=reflectorsModel->index(reflectorIndex,0);
            reflectorsSelectionModel->select(selectedIndex,QItemSelectionModel::Select);
        }
        ++myIterator;
    }

    int binocularIndex;
    QList<pair<Binocular*, int>>::iterator myBinocularIterator; // iterator
    myBinocularIterator = myBinoculars.begin();
    binocularsSelectionModel->clear();
    QModelIndex binocularSelectedIndex;
    while (myBinocularIterator != myBinoculars.end() )
    {
        binocular=myBinocularIterator->first;
        if(binocular->isSelected())
        {
            binocularIndex=myBinocularIterator->second;
            binocularSelectedIndex=binocularsModel->index(binocularIndex,0);
            binocularsSelectionModel->select(binocularSelectedIndex,QItemSelectionModel::Select);
        }
        ++myBinocularIterator;
    }

    int inspectorIndex;
    QList<pair<BeamInspector*, int>>::iterator myBeamInspectorIterator; // iterator
    myBeamInspectorIterator = myBeamInspectors.begin();
    inspectorsSelectionModel->clear();
    QModelIndex inspectorSelectedIndex;
    while (myBeamInspectorIterator != myBeamInspectors.end() )
    {
        beamInspector=myBeamInspectorIterator->first;
        if(beamInspector->isSelected())
        {
            inspectorIndex=myBeamInspectorIterator->second;
            inspectorSelectedIndex=inspectorsModel->index(inspectorIndex,0);
            inspectorsSelectionModel->select(inspectorSelectedIndex,QItemSelectionModel::Select);
        }
        ++myBeamInspectorIterator;
    }

    if(laserpoint->isSelected())
        laserSelectionModel->select(laserModel->index(0, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select);

    if(!labroomList.empty())
    {
        if(myLabRoom)
        {
            if(myLabRoom->isSelected())
                environmentSelectionModel->select(environmentModel->index(0, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select);
        }
    }
}

void MainWindow::laserpointSelectionFromGraphics()
{
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();
    environmentSelectionModel->clear();
    laserSelectionModel->select(laserModel->index(0, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select);
    updateActions();
}

void MainWindow::labroomSelectionFromGraphics()
{
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();
    laserSelectionModel->clear();
    environmentSelectionModel->select(laserModel->index(0, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select);
    updateActions();
}

QPainterPath MainWindow::laserpointShapePath()
{
    QPainterPath laserShape;

    laserShape=laserpoint->shapePath();
    return laserShape;
}

void MainWindow::setLaserpointShapePathForReflectors()
{
    if(selectedReflector())
    {
        if(laserpoint->shapePathContainsPoint(laserpoint->mapFromScene(selectedReflector()->pos())))
            selectedReflector()->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
        else
            selectedReflector()->setOpticalDiameter(0.0);

        updateActions();
    }
}

void MainWindow::setLaserpointShapePathForBinoculars()
{
    if(selectedBinocular())
    {
        double exendedOpticalDiameter=selectedBinocular()->getExendedOpticalDiameter();

        if(laserpoint->shapeEnhacedPathContainsPoint(laserpoint->mapFromScene(selectedBinocular()->pos()), exendedOpticalDiameter))
            selectedBinocular()->setInZone(true);
        else
            selectedBinocular()->setInZone(false);

        updateActions();
    }
}

void MainWindow::viewCenter()
{
    laserWindow->graphicsView->centerOn(laserpoint->pos());
}

void MainWindow::deletedViewCenter()
{
    QTimer::singleShot(100, this, SLOT(viewCenter()));
}

void MainWindow::setPolygon()
{
    if ((myLabRoom)&&(environmentModel->getState()))
    {
        laserWindow->graphicsView->scene->removeItem(myLabRoom);
        environmentModel->setState(false);
        environmentModel->setMeteoVisibility(laserWindow->getMeteoRange());
        environmentModel->addDescriptor(*myFakeRoom);
        environmentModel->myDataHasChanged();
        labroomList.clear();
        addScintillationAct->setEnabled(true);
        addAtmosphericEffectsAct->setEnabled(true);
        changeMeteoAct->setEnabled(true);
        menuSceneScaleChanged("100%", 4);
        setWindowModified(true);
        undoStack->clear();
        laserWindow->setUndoStack(undoStack);
    }
}

void MainWindow::addRoom()
{
    if(environmentModel->getState())
    {
        statusBar()->showMessage(tr("Ambiente chiuso già presente"), 2000);
        return;
    }

    menuSceneScaleChanged("8000%", 15);

    environmentModel->setState(true);
    laserWindow->graphicsView->scene->addItem(myLabRoom);
    labroomList.append(myLabRoom);
    myLabRoom->setPos(laserpoint->pos().x(), laserpoint->pos().y());

    if(dragModeState)
    {
        myLabRoom->setFlag(QGraphicsItem::ItemIsMovable, false);
        myLabRoom->setFlag(QGraphicsItem::ItemIsSelectable, false);
        myLabRoom->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }
    else
    {
        myLabRoom->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    }

    bool meteoWidgetsON=false;
    meteoWidgets(meteoWidgetsON, meteoWidgetsON, meteoWidgetsON);
    updateForCondMeteo();

    int zValue = 0;

    setWindowModified(true);

    zValue=laserpoint->zValue()-1;
    myLabRoom->setZValue(zValue);

    laserpoint->setSelected(true);
    laserWindow->graphicsView->centerOn(myLabRoom->pos()); 

    undoStack->clear();
    laserWindow->setUndoStack(undoStack);

    emit myLabRoomListChanged();
}

void MainWindow::createRoom()
{
    QPen pen;
    pen.setCosmetic(true);
    pen.setColor(QColor::fromRgb(0, 200, 0));

    QRectF labRect(0.0, 0.0, 10, 6);
    QPointF center= QPointF(labRect.width()/2, labRect.height()/2);
    labRect.translate(-center);

    myLabRoom=new LabRoom(labRect);

    connect(myLabRoom, SIGNAL(xChanged()), this, SLOT(updateLabList()));
    connect(myLabRoom, SIGNAL(xChanged()), this, SLOT(updateLabList()));
}

void MainWindow::atmosphericEffectsOn(bool _atmEffectsBool)
{
    laserWindow->setAtmEffectsBool(_atmEffectsBool);
}

void MainWindow::scintillationOn(bool _scintillationBool)
{
    laserWindow->setScintillationBool(_scintillationBool);
}

void MainWindow::listDeselectionFromGraphics()
{
    laserWindow->graphicsView->scene->clearSelection();
    laserSelectionModel->clear();
    reflectorsSelectionModel->clear();
    binocularsSelectionModel->clear();
    environmentSelectionModel->clear();
}

void MainWindow::addObjectLink()
{
    ObjectNodePair objectNodes = selectedObjectNodePair();
    if (objectNodes == ObjectNodePair())
        return;

    ObjectLink *objectlink = new ObjectLink(objectNodes.first, objectNodes.second);
    laserWindow->graphicsView->scene->addItem(objectlink);
}

void MainWindow::addFootprint()
{
    QPointF footprintPos=QPointF(laserpoint->pos().x()+ (100.0/scale * ((footprintsCount+1) % 5)),
                        laserpoint->pos().y()+(50.0/scale * (((footprintsCount+1) / 5) % 7)));

    double attenuatedDNRO= attenuatedDistance(laserWindow->myDockControls->getOpticalDistance());

    addFootprintCommand = new AddFootprintCommand(attenuatedDNRO, scale, laserWindow,
                        laserpoint, &myFootprints, footprintPos);

    undoStack->push(addFootprintCommand);

    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(footprintPos, QTransform());
    footprint= qgraphicsitem_cast<FootprintObject*>(item);

    footprint->setUndoStack(undoStack);
//addFootprintCommand finisce qui
    setMaxEhnacedOpticalDiameter();
    footprint->laserParameterChanged();
    setShadowZone();

    footprintsCount++;

    setWindowModified(true);

    connect(footprint, SIGNAL(xChanged()), this, SLOT(setShadowZone()));
    connect(footprint, SIGNAL(yChanged()), this, SLOT(setShadowZone()));
}

void MainWindow::setShadowZone()
{
    if(footprint==0)
        return;

    shadowPathZone.clear();
    ehnacedPathZone.clear();
    QList<FootprintObject*>::iterator myIterator; // iterator
    myIterator = myFootprints.begin();

    while (myIterator != myFootprints.end() )
    {
        footprint=*myIterator;
        footprint->setTipString();
        QPainterPath myPath;
        QPainterPath myEhnacedPath;

        myPath=footprint->mapToItem(laserpoint, footprint->getShadowPath());
        shadowPathZone=shadowPathZone.united(myPath);

        myEhnacedPath=footprint->mapToItem(laserpoint, footprint->getEhnacedShadowPath());
        ehnacedPathZone=ehnacedPathZone.united(myEhnacedPath);
        footprint->laserParameterChanged();

        ++myIterator;
    }
    laserpoint->setShadowZone(shadowPathZone);
    laserpoint->setEhnacedShadowZone(ehnacedPathZone);
    setDistanceForBinocular();
    setDistanceForReflector();
}

void MainWindow::shadowZoneForLaser()
{
    setShadowZone();
    laserpoint->setShadowZone(shadowPathZone);
    laserpoint->setEhnacedShadowZone(ehnacedPathZone);
}

void MainWindow::setMaxEhnacedOpticalDiameter()
{    
    if(footprint==0)
        return;

    QVector<double>ExendedOpticalDiameterVect;
    QList<pair<Binocular*, int>>::iterator binocularIterator; // iterator
    binocularIterator = myBinoculars.begin();
    double maxEhnacedOpticalDiameter;
    while (binocularIterator != myBinoculars.end() )
    {
        binocular=binocularIterator->first;
        ExendedOpticalDiameterVect.push_back(binocular->getExendedOpticalDiameter());
        ++binocularIterator;
    }
    maxEhnacedOpticalDiameter = *max_element(ExendedOpticalDiameterVect.begin(), ExendedOpticalDiameterVect.end());

    QList<FootprintObject*>::iterator footprintIterator; // iterator
    footprintIterator = myFootprints.begin();
    while (footprintIterator != myFootprints.end() )
    {
        footprint=*footprintIterator;
        footprint->setEhnacedDiameter(maxEhnacedOpticalDiameter);
        footprint->laserParameterChanged();
        ++footprintIterator;
    }
}

void MainWindow::setEhnacedShadowZone()
{

}

void MainWindow::clearScene()
{
    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();
    QMutableListIterator<QGraphicsItem *> i(items);
    while(i.hasNext())
    {
        Link *link = qgraphicsitem_cast<Link *>(i.next());
        if(link)
        {
            delete link;
            i.remove();
        }
    }
    QMutableListIterator<QGraphicsItem *> j(items);
    while(j.hasNext())
    {
        BinocularLink *binocularlink = qgraphicsitem_cast<BinocularLink *>(j.next());
        if(binocularlink)
        {
            delete binocularlink;
            j.remove();
        }
    }
    QMutableListIterator<QGraphicsItem *> k(items);
    while(k.hasNext())
    {
        InspectorLink *inspectorLink = qgraphicsitem_cast<InspectorLink *>(k.next());
        if(inspectorLink)
        {
            delete inspectorLink;
            k.remove();
        }
    }
    QMutableListIterator<QGraphicsItem *> m(items);
    while(m.hasNext())
    {
        ObjectLink *objectlink = qgraphicsitem_cast<ObjectLink *>(m.next());
        if(objectlink)
        {
            delete objectlink;
            m.remove();
        }
    }
    qDeleteAll(items);
}

void MainWindow::setViewportRect()
{
    if(gridlines==nullptr)
        return;

    QRectF viewportRect=laserWindow->graphicsView->getViewportRect();
    viewportRect.adjust(-40.0, -40.0, 40.0, 40.0);
    QSizeF viewportSize=viewportRect.size()/scale;
    viewportRect.setSize(viewportSize);
    QRectF sceneRect=laserWindow->graphicsView->scene->itemsBoundingRect();
    double width=fmax(viewportRect.width(), sceneRect.width());
    double height=fmax(viewportRect.height(), sceneRect.height());
    QRectF myRect=QRectF(0.0, 0.0, width, height);
    gridlines->setSceneRect(myRect);

    QTransform transform =laserWindow->graphicsView->viewportTransform();
    QPointF bottomRightViewport=gridlines->deviceTransform(transform)
            .inverted()
            .map(laserWindow->graphicsView->viewport()->rect().bottomRight()-QPointF(50, 50));

   gridlines->setTopLeftViewport(bottomRightViewport*scale);
}

void MainWindow::setSceneRect()
{
    if(gridlines==nullptr)
        return;

    QRectF rect=laserWindow->graphicsView->scene->sceneRect();
    qDebug() << "Rettangolo dalla scena: " << rect;

    QRectF sceneRect=rect.translated(rect.center());

    gridlines->setSceneRect(sceneRect);
}

QString MainWindow::installationKind()const
{
    QString installation;
    if(myLabRoomInserted)
        installation="laboratorio";
    else
        installation="Poligono di tiro militare";

    return  installation;
}

void MainWindow::setGoggleMaterial(LaserGoggle::material myMaterial)
{
    laserWindow->myDockControls->setGoggleMaterial(myMaterial);
    setWindowModified(true);
}

void MainWindow::changeGuiTheme()
{
    theme=darkThemeAct->isChecked();
    setGuiDarkTheme();
}

void MainWindow::setGuiDarkTheme()
{
    if(theme)
    {
        setStyleSheet(tr("QWidget{background-color:#555555;}"
                         "QWidget{color:#f0f0f0;}"
                         "QMenu::item:selected{background-color:#00c800;}"
                         "QMenu::item:selected{color:#f0f0f0;}"
                         "QComboBox::item:selected{background-color:#00c800;}"
                         "QComboBox::item:selected{color:#f0f0f0;}"
                         "QMenu::item:checked{background-color:#666666;}"
                         "QMenuBar::item:selected{color:#f0f0f0;}"
                         "QMenuBar::item:selected{background-color:#00c800;}"
                         "QToolBar{background-color:#555555;}"
                         "QToolBar QToolButton:checked{background-color:#666666;}"
                         "QTextEdit{background-color:#f0f0f0;}"
                         "QGridLayout{color:#f0f0f0;}"
                         "QGraphicsView{background-color:#f0f0f0;}"));

        laserWindow->setStyleSheet(tr("QWidget {background-color: #555555;}\n"
                         "QLabel {background: none;}\n"
                         "QMenu::item:selected{background-color:#00c800;}"
                         "QMenu::item:selected{color:#f0f0f0;}"
                         "QGraphicsView {background-color:#f0f0f0;}"
                         ));
        laserWindow->myDockGoggle->setStyleSheet(tr("QLabel {color: #fafafa}"));
    }
    else
    {
        setStyleSheet(tr(""));
        laserWindow->setStyleSheet(tr(""));
        laserWindow->myDockGoggle->setStyleSheet(tr(""));
    }
}

void MainWindow::setThemeOnStart()
{
    QSettings settings("Carmine Giordano", "Laser Studio");
    settings.beginGroup("mainWindow");
    theme=settings.value("theme").toBool();
    settings.endGroup();

    setGuiDarkTheme();
}

QString MainWindow::commandRedoTriggered() const
{
    QString redoCommand;
    int index=undoStack->index();

    if(undoStack->command(index-1)==addFootprintCommand)
        redoCommand="Ripristina \"Aggiungi ingombro\"";
    else if(undoStack->command(index-1)==deleteFootprintCommand)
        redoCommand="Ripristina \"Cancella ingombro\"";
    else if(undoStack->command(index-1)==addReflectorCommand)
        redoCommand="Ripristina \"Aggiungi riflettore\"";
    else if(undoStack->command(index-1)==deleteReflectorCommand)
        redoCommand="Ripristina \"Cancella riflettore\"";
    else if(undoStack->command(index-1)==addBinocularCommand)
        redoCommand="Ripristina \"Aggiungi strumento ottico\"";
    else if(undoStack->command(index-1)==deleteBinocularCommand)
        redoCommand="Ripristina \"Cancella strumento ottico\"";
    else
        redoCommand="Ripristina comando";

    return redoCommand;
}

QString MainWindow::commandUndoTriggered()const
{
    QString undoCommand;
    int index=undoStack->index();

    if(undoStack->command(index)==addFootprintCommand)
        undoCommand="Annulla \"Aggiungi ingombro\"";
    else if(undoStack->command(index)==deleteFootprintCommand)
        undoCommand="Annulla \"Cancella ingombro\"";
    else if(undoStack->command(index)==addReflectorCommand)
        undoCommand="Annulla \"Aggiungi riflettore\"";
    else if(undoStack->command(index)==deleteReflectorCommand)
        undoCommand="Annulla \"Cancella riflettore\"";
    else if(undoStack->command(index)==addBinocularCommand)
        undoCommand="Annulla \"Aggiungi strumento ottico\"";
    else if(undoStack->command(index)==deleteBinocularCommand)
        undoCommand="Annulla \"Cancella strumento ottico\"";
    else
        undoCommand="Annulla comando";

    return undoCommand;
}

void MainWindow::undo()
{
   /*
    * Ripristinata un'operazione di cancellazione il numero effettivo
    * di ingombri risulta maggiore di quello calcolato.
    * In questo caso è necessario connettere il nuovo ingombro
    * con le slot di aggiornamento.
    */
    int index=undoStack->index();
    bool addFootprintCmd=undoStack->command(index)==addFootprintCommand;
    bool addReflectorCmd=undoStack->command(index)==addReflectorCommand;
    bool addBinocularCmd=undoStack->command(index)==addBinocularCommand;
    bool deleteFootprintCmd=undoStack->command(index)==deleteFootprintCommand;
    bool deleteReflectorCmd=undoStack->command(index)==deleteReflectorCommand;
    bool deleteBinocularCmd=undoStack->command(index)==deleteBinocularCommand;
    qDebug()<<"undoStack->command(index)==addFootprintCommand: "<<addFootprintCmd;
    qDebug()<<"undoStack->command(index)==deleteFootprintCommand: "<<deleteFootprintCmd;
    if(addFootprintCmd)
    {
        if(myFootprints.count()<footprintsCount)
        {
            if(!myFootprints.isEmpty())
            {
                footprint=myFootprints.last();
                setMaxEhnacedOpticalDiameter();
                footprint->laserParameterChanged();
            }
            setShadowZone();
            setWindowModified(true);
        }
        footprintsCount=myFootprints.count();
    }
    else if(deleteFootprintCmd)
    {
        if(myFootprints.count()>footprintsCount)
        {
            if(!myFootprints.isEmpty())
            {
                footprint=myFootprints.last();
                setMaxEhnacedOpticalDiameter();
                footprint->laserParameterChanged();
            }
            setShadowZone();
            setWindowModified(true);
        }
        footprintsCount=myFootprints.count();
    }
    else if(addReflectorCmd)
    {

    }
    else if(addBinocularCmd)
        setMaxEhnacedOpticalDiameter();

    else if(deleteReflectorCmd)
    {
        if(!myReflectors.isEmpty())
        {
            reflector=myReflectors.last().first;
            setMaxEhnacedOpticalDiameter();
            setLaserpointShapePathForReflectors();
        }
    }
    else if(deleteBinocularCmd)
        setMaxEhnacedOpticalDiameter();

    if((!addFootprintCmd)&&(!deleteFootprintCmd))
        setShadowZone();

    statusBar()->showMessage(commandUndoTriggered(), 2000);
}

void MainWindow::redo()
{
   /*
    * Ripristinata un'operazione di aggiunta di un nuovo ingombro
    * il numero effettivo risulta maggiore di quello calcolato.
    * In questo caso è necessario connettere il nuovo ingombro
    * con le slot di aggiornamento.
    */

    int index=undoStack->index();
    bool addFootprintCmd=undoStack->command(index-1)==addFootprintCommand;
    bool addReflectorCmd=undoStack->command(index-1)==addReflectorCommand;
    bool addBinocularCmd=undoStack->command(index-1)==addBinocularCommand;
    bool deleteFootprintCmd=undoStack->command(index-1)==deleteFootprintCommand;
    bool deleteReflectorCmd=undoStack->command(index-1)==deleteReflectorCommand;
    bool deleteBinocularCmd=undoStack->command(index-1)==deleteBinocularCommand;
    qDebug()<<"undoStack->command(index-1)==addFootprintCommand: "<<addFootprintCmd;
    qDebug()<<"undoStack->command(index-1)==deleteFootprintCommand: "<<deleteFootprintCmd;
    if(addFootprintCmd)
    {
        if(myFootprints.count()>footprintsCount)
        {
            if(!myFootprints.isEmpty())
            {
                footprint=myFootprints.last();
                setMaxEhnacedOpticalDiameter();
                footprint->laserParameterChanged();
            }
            setShadowZone();
            setWindowModified(true);
        }
        footprintsCount=myFootprints.count();
    }
    else if(deleteFootprintCmd)
    {
        if(myFootprints.count()<footprintsCount)
        {
            if(!myFootprints.isEmpty())
            {
                footprint=myFootprints.last();
                setMaxEhnacedOpticalDiameter();
                footprint->laserParameterChanged();
            }
            setShadowZone();
            setWindowModified(true);
        }
        footprintsCount=myFootprints.count();
    }
    else if(addReflectorCmd)
    {
        setMaxEhnacedOpticalDiameter();
        setLaserpointShapePathForReflectors();
    }
    else if(addBinocularCmd)
    {
        double exendedOpticalDiameter=binocular->getExendedOpticalDiameter();
        bool binocularInZone=laserpoint->shapeEnhacedPathContainsPoint(laserpoint->mapFromScene(binocular->pos()), exendedOpticalDiameter);
        binocular->setInZone(binocularInZone);
        setMaxEhnacedOpticalDiameter();
        binocular->laserParametersChanged();

        if(footprint!=nullptr)
        {
            QList<FootprintObject*>::iterator myIterator; // iterator
            myIterator = myFootprints.begin();

            while (myIterator != myFootprints.end() )
            {
                footprint=*myIterator;
                footprint->laserParameterChanged();
                ++myIterator;
            }
        }
    }
    else if(deleteReflectorCmd)
    {
        if(!myReflectors.isEmpty())
        {
            reflector=myReflectors.last().first;
            setMaxEhnacedOpticalDiameter();
            setLaserpointShapePathForReflectors();
        }
    }
    else if(deleteBinocularCmd)
        setMaxEhnacedOpticalDiameter();

    if((!addFootprintCmd)&&(!deleteFootprintCmd))
        setShadowZone();

    statusBar()->showMessage(commandRedoTriggered(), 2000);
}

void MainWindow::updateUndoStackList(int idx)
{
    qDebug()<<"Idx pari a: "<< idx;

    setMaxEhnacedOpticalDiameter();
    setShadowZone();
    footprintsCount=myFootprints.count();

    environmentModel->setMeteoVisibility(laserWindow->getMeteoRange());
    environmentModel->myDataHasChanged();
    updateForCondMeteo();
}

void MainWindow::controlsModified()
{
    setWindowModified(true);
}

void MainWindow::graphicItemMoveToStack(QGraphicsItem *movingItem, const QPointF& oldPosition)
{
    undoStack->push(new MoveCommand(movingItem, oldPosition));
}

MainWindow::~MainWindow()
{
    if(!myFootprints.isEmpty())
    {
        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();
        while (myIterator != myFootprints.end())
        {
            footprint=*myIterator;
            delete footprint;
        }
    }
    if(!myReflectors.isEmpty())
    {
        QList<pair<Reflector*, int>>::iterator myIterator; // iterator
        myIterator = myReflectors.begin();
        while (myIterator != myReflectors.end())
        {
            reflector=myIterator->first;
            delete reflector;
        }
    }
    if(!myBinoculars.isEmpty())
    {
        QList<pair<Binocular*, int>>::iterator myIterator; // iterator
        myIterator = myBinoculars.begin();
        while (myIterator != myBinoculars.end())
        {
            binocular=myIterator->first;
            delete binocular;
        }
    }
}

void MainWindow::updateForBeamInspection()
{
    BeamInspector::computeRayleighDistance(laserWindow->myDockControls->getWavelength(),
                                           laserWindow->myDockControls->getBeamDiameter(),
                                           laserWindow->myDockControls->getDivergence());
    laserpoint->setRayleighDistance(BeamInspector::getRayleighDistance());
    laserpoint->setQualityFactor(BeamInspector::getQualityFactor());

    QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
    myIterator = myBeamInspectors.begin();

    while (myIterator != myBeamInspectors.end() )
    {
        beamInspector=myIterator->first;
        beamInspector->setWavelength(laserWindow->myDockControls->getWavelength());
        beamInspector->setBeamDiameter(laserWindow->myDockControls->getBeamDiameter()),
        beamInspector->setDivergence(laserWindow->myDockControls->getDivergence());
        //opticalDistance è la NOHD che viene moltiplicata per 2 da setDNRO_Diameter
        beamInspector->laserParametersChanged();

        ++myIterator;
    }
    inspectorsModel->myDataHasChanged();
}
