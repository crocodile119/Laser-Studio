#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QVariant>
#include <QTimer>
#include <QStringList>
#include <QPrintDialog>
#include <QPrinter>
#include <QLabel>
#include <QMouseEvent>
#include <QComboBox>
#include "centralwidget.h"

//Scene
#include <QPair>
#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "link.h"
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "wetchartdialog.h"
#include "fresnelchartdialog.h"
#include "lambertianchartdialog.h"
#include "reflectorslistmodel.h"
#include "laserlistmodel.h"
#include "binocularslistmodel.h"
#include "environmentlistmodel.h"
#include "labroom.h"
#include "footprintobject.h"
#include "objectlink.h"
#include "gridlines.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#endif


class Link;
class LaserPoint;
class Reflector;
class Binocular;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool eventFilter (QObject *watched, QEvent *event)override;
    int seqNumerCount()const;

    const static QString HTML_DEF;

    void setEMP();
    void setPowerErg();
    void setBeamDiameter();
    void enableControlsAndItems(bool enabled);

    double getEMP();
    double getPowerErg();
    double getBeamDiameter();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void nohdModified();
    void reflectorListChanged();
    void binocularListChanged();
    void myLabRoomListChanged();

private slots:
    bool save();
    bool saveAs();
    void open();
    void newFile();
    void openRecentFile();
    void about();
    void onLineHelp();
    bool loadFile(const QString &fileName);

    void print();   
    void on_printPreviewAction_triggered();
    void printReport(QPrinter* painter);
    void viewCenter();
    void deletedViewCenter();
    void laserModified();
    void updateScale();
    void sceneScaleChanged(const QString &scale);
    void sceneScaleUp();
    void sceneScaleDown();
    void menuSceneScaleChanged(const QString &, const int &);
    void barSceneScaleChanged(const QString &scale);
    void setSceneArea(QRect) const;
    void dragMode();
    void addRoom();
    void selectionMode();
    void noFeasibleInputFunction();
    double attenuatedDistance(const double&);
    void setDNRO_ForLaserpoint();
    void setDNRC_ForLaserpoint();
    void setDivergenceForReflector();
    void setDivergenceForBinocular();
    void setDNRO_ForReflector();
    void setDNRO_ForBinocular();
    void setDNRO_ForFootprint();
    void setWavelengthForBinocular();
    void setLambertianMaxForReflector();
    void addReflector(const target& target);
    void addBinocular();
    void addFootprint();
    void addLabList();
    void addBinocularList();
    void addBinocularLink();
    void addObjectLink();
    void addLink();
    void setPolygon();
    void del();
    void cut();
    void copy();
    void paste();   
    void makeSceneOfSavedItems();
    void bringToFront();
    void sendToBack();
    void wetTarget();
    void fresnelTarget();
    void lambertianTarget();
    void updateActions();
    void setUpdatedPosition();
    QPointF getUpdatedPosition();
    void setDistanceForReflector();
    void setDistanceForBinocular();
    void setBeamDiameterForBinocular();
    void setupLaserProspective();
    void setupFieldProspective();
    void setupGoggleProspective();
    void setupClassifierProspective();
    void setCondMeteo();
    void atmosphericEffectsOn(bool);
    void scintillationOn(bool);
    void properties();
    void atmosphericEffects();
    void scintillation();
    void installationDescription();
    void backgroundGrid();
    void setPrintPreview();
    void setSelectionRect();
    void goToPoint();
    void setImageRect();
    void setImagePreview();
    void setShadowZone();
    void setEhnacedShadowZone();
    void setViewportRect();
    void setSceneRect();
    QString installationKind()const;

    void setReflectorEMP_ForDiffusion();
    void setReflectorPowerErgForDiffusion();
    void setReflectorBeamDiameterForDiffusion();
    void labroomSelectionFromGraphics();
    void listSelectionFromGraphics();
    void binocularListSelectionFromGraphics();
    void listMultipleSelectionFromGraphics();
    void laserpointSelectionFromGraphics();
    void listDeselectionFromGraphics();

    void addElementList();
    void goToLab();

    void setGoggleMaterial(LaserGoggle::material);
    void goToSelectedReflector();
    void goToSelectedBinocular();
    void gotToLaserpoint();
    void updateList();
    void updateLabList();
    void updateLaserList();
    void updateBinocularList();
    void updateForCondMeteo();
    void selectFromList();
    void selectBinocularFromList();
    void propertyFromList();
    void environmentFromList();
    void setLaserpointShapePathForReflectors();
    void setLaserpointShapePathForBinoculars();
    void setDistanceForFootprint();
    void shadowZoneForLaser();
    void meteoWidgets(bool, bool, bool);
    QString printGoggleLimits(const vector< pair <int,double> > &, const int &, const string &);

    //scene

private:
    typedef QPair<LaserPoint *, Reflector *> NodePair;
    typedef QPair<LaserPoint *, Binocular *> BinocularNodePair;   
    typedef QPair<LaserPoint *, FootprintObject *> ObjectNodePair;

    //scene
    void createActions();
    void createMenus();
    void createToolBars();
    void createContextMenu();
    void clearScene();
    void setZValue(int z);
    Reflector *selectedReflector() const;
    LaserPoint *selectedLaserPoint() const;   
    Binocular *selectedBinocular() const;
    Link *selectedLink() const;
    LabRoom *selectedLab() const;
    FootprintObject *selectedFootprint()const;
    NodePair selectedNodePair() const;
    BinocularNodePair selectedBinocularNodePair() const;
    ObjectNodePair selectedObjectNodePair() const;

    void setMaxEhnacedOpticalDiameter();
    void createStatusBar();
    QString strippedName(const QString &fullFileName);
    bool okToContinue();
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void updateRecentFileActions();
    void readSettings();
    void writeSettings();
    void setLaserPoint();
    void setControls();   
    void backgroundGridPixmap();

    QString makeHtml();
    //QString makeHtmlClassifier();
    void htmlResults();
    void htmlClassifierResults();
    void firstPageReport();
    void footprintsPageReport();
    void reflectorsPageReport();
    void binocularsPageReport();
    QString htmlReflectors();
    QString htmlBinoculars();
    QString htmlFootprints();
    QString htmlClassifier();
    QString htmlMeteo();
    void printHtml(const QString&);
    void laserMoved();
    QString printReflectorTable(vector< pair <double,double> >);
    QString printSpecularReflectorCoefficients( vector< pair <double,double> >);
    void htmlClassifierDetails();

    QPainterPath laserpointShapePath();

    ReflectorsListModel *reflectorsModel;
    BinocularsListModel *binocularsModel;
    LaserListModel *laserModel;
    EnvironmentListModel *environmentModel;
    QItemSelectionModel *reflectorsSelectionModel;
    QItemSelectionModel *laserSelectionModel;
    QItemSelectionModel *binocularsSelectionModel; 
    QItemSelectionModel *environmentSelectionModel;

    QString curFile;
    QString FILENAME;
    bool externalFile;
    QStringList recentFiles;
    double reflectorDistance;
    QTimer *timer;
    QStringList effects;
    QStringList input;
    QStringList output;
    QStringList goggle;
    QStringList reflectors;
    QStringList binoculars;
    QStringList footprints;
    QStringList laser;
    QStringList skin;
    QStringList classifierOutput;
    QStringList classifierDetails;
    QLabel *statusLabel;
    CentralWidget *laserWindow;
    QStringList *firstPage;
    QStringList *entries;
    QRect previewRect;
    QRect previewImage;
    QComboBox *sceneScaleCombo;
    QPainterPath shadowPathZone;
    QPainterPath ehnacedPathZone;
    QPainterPath hazardZone;

    int roomNumber;
    double beamDiameter;
    double myEMP;
    double powerErg;
    double scale;

    enum { MaxRecentFiles = 5 };
    enum { nScales = 20 };

    //Scene
    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *viewMenu;
    QMenu *reflectorsMenu;
    QMenu *reflectorsEditMenu;
    QMenu *helpMenu;
    QMenu *prospectiveMenu;
    QMenu *zoomMenu;
    QMenu *displayMenu;
    QMenu *environmentMenu;
    QMenu *placeMenu;
    QMenu *goggleMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *environmentToolBar;
    QToolBar *sceneToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *setPreviewAct;
    QAction *exportImageAct;
    QAction *descriptionAct;
    QAction *exitAction;
    QAction *quitAct;
    QAction *laserSettingsAction;
    QAction *fieldSettingsAction;
    QAction *classifierSettingsAction;
    QAction *goggleSettingsAction;
    QAction *showGridAction;
    QAction *recentFileActions[MaxRecentFiles];

    QStringList scales;
    QAction *zoomActions[nScales];
    QAction *selectAct;
    QAction *separatorAction;
    QAction *showDockWidgetEffects;
    QAction *showDockWidgetResults;
    QAction *showDockWidgetControls;
    QAction *showDockWidgetSkin;
    QAction *showDockWidgetGoggle;
    QAction *showReflectorsList;
    QAction *showDockLea;
    QAction *centerOnViewAction;
    QAction *printPreviewAct;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *dragAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *onLineHelpAct;

    LaserPoint *laserpoint;
    Reflector *reflector;
    Binocular *binocular;
    LabRoom *myLabRoom;
    LabRoom *myFakeRoom;
    FootprintObject *footprint;
    GridLines *gridlines;

    int minZ;
    int maxZ;
    int seqNumber;
    int binSeqNumber;
    int footprintSeqNumber;
    int scaleNumber;
    double gridUnit;
    bool dragModeState;
    bool myLabRoomInserted;
    bool state;

    QAction *addBinocularAct;
    QAction *addLabAct;
    QAction *addWetReflectorAction;
    QAction *addGlassReflectorAction;
    QAction *addMirrorReflectorAction;
    QAction *addLambertianReflectorAction;
    QAction *addFootprintAct;
    QAction *changeMeteoAct;
    QAction *addAtmosphericEffectsAct;
    QAction *addScintillationAct;
    QAction *setPolygonAct;
    QAction *deleteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;
    QAction *wetTargetAction;
    QAction *fresnelTargetAction;
    QAction *lambertianTargetAction;

    QAction *onlyReflectorGoggleAction;
    QAction *glassGoggleAction;
    QAction *plasticGoggleAction;


    QGraphicsView *view;
    QList <pair<Reflector *, int>> myReflectors;
    QList <pair<Binocular *, int>> myBinoculars;
    QList<pair<FootprintObject*, int>> myFootprints;
    QList <LaserPoint*> laserPointList;
    QList <LabRoom*> labroomList;
    QPointF pointPosition;

    int SmallGap;
    int MediumGap;
    int LargeGap;

    QFont titleFont;
    QFont bodyFont;
    QFont footerFont;
    QFont sceneFont;
    QTransform myTransform;
};

#endif
