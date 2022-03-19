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
#include <QUndoStack>
#include <QUndoView>
#include "centralwidget.h"
#include "dockhistory.h"
#include "ui_dockhistory.h"

//Scene
#include <QPair>
#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "reflectorlink.h"
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "beaminspector.h"
#include "beaminspectordialog.h"
#include "inspectorlink.h"
#include "wetchartdialog.h"
#include "fresnelchartdialog.h"
#include "lambertianchartdialog.h"
#include "labroom.h"
#include "footprintobject.h"
#include "objectlink.h"
#include "gridlines.h"
#include "laserreport.h"
#include "treemodel.h"
#include "safetysignitem.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#endif


class ReflectorLink;
class LaserPoint;
class Reflector;
class Binocular;
class SafetySignItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum htmlConfig{REPORT, PDF};
    MainWindow();
    ~MainWindow();
    bool eventFilter (QObject *watched, QEvent *event)override;
    int seqNumerCount()const;

    const static QString HTML_DEF;
    const static QSize mySceneImageSize;

    void setEMP();
    void setPowerErg();
    void setBeamDiameter();
    void enableControlsAndItems(bool enabled);

    double getEMP();
    double getPowerErg();
    double getBeamDiameter();

protected:
    void closeEvent(QCloseEvent *event) override;

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

signals:
    void sceneRectChanged(const QRectF &);

private slots:
    bool save();
    bool saveAs();
    void open();
    void setOpenFile();
    void setCurrentDirectory();
    void newFile();
    void openRecentFile();
    void about();
    void onLineHelp();
    bool loadFile(const QString &fileName);

    void viewCenter();
    void deletedViewCenter();
    void laserModified();
    void updateScale();
    void sceneScaleUp();
    void sceneScaleDown();
    void sceneScaleReset();
    void sceneScroll();
    void sceneRoomView();
    void setSceneScale();
    void sceneGridScaleUpdate();
    void boundingRectForScale();
    void setSceneArea(QRect) const;
    void dragMode();
    void addRoom();
    void createRoom();
    void setStatusBarState();
    void setGridState();
    void setBoundingRect();

    void addSafetySign(SafetySignItem::SafetyClass mySafetySign);

    void selectionMode();
    void statusBarSignalFunction(const QString &);
    double attenuatedDistance(const double&);
    void setDNRO_ForLaserpoint();
    void setDNRC_ForLaserpoint();
    void setDivergenceForReflector();
    void setDivergenceForBinocular();
    void setDNRO_ForReflector();
    void setDNRO_ForBinocular();
    void setDNRO_ForFootprint();
    void setDNRO_ForInspector();
    void setWavelengthForBinocular();
    void setLambertianMaxForReflector();
    void addReflector(const target& target);
    void addBinocular();
    void addBeamInspector();
    void addFootprint();
    void setGuiDarkTheme();
    void changeGuiTheme();
    void setThemeOnStart();
    void addBinocularLink();
    void addObjectLink();
    void addReflectorLink();
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
    void setDistanceForInspector();
    void setBeamDiameterForBinocular();
    void setupLaserProspective();
    void setupFieldProspective();
    void setupGoggleProspective();
    void setupClassifierProspective();
    void setCondMeteo();
    void atmosphericEffectsOn(bool);
    void scintillationOn(bool);
    void properties();
    void setBackgroundGrid();
    void atmosphericEffects();
    void scintillation();
    void installationDescription();
    void createBackgroundGrid();
    void backgroundGridOn();
    void backgroundGridOff();
    void setPrintPreview();
    void exportReport();
    void setSelectionRect();
    void setNewOriginPoint();
    void originOnClick(const QPointF &);
    void setImageRect();
    void setImagePreview();
    void setZoomWin();
    void setZoomRect();
    void setShadowZone();
    void setEhnacedShadowZone();
    void setViewportRect();
    void setSceneRect();
    QString installationKind()const;
    void controlsModified();
    void undo();
    void redo();
    void updateUndoStackList(int idx);

    void setReflectorEMP_ForDiffusion();
    void setReflectorPowerErgForDiffusion();
    void setReflectorBeamDiameterForDiffusion();
    void treeSelectionFromGraphics(QGraphicsItem *item);
    void treeSelectionFromLab();
    void listDeselectionFromGraphics();

    void setGoggleMaterial(LaserGoggle::material);
    void goToGraphicsItem(QModelIndex index);
    void updateForCondMeteo();
    void selectItemFromTree(QModelIndex index);
    void setLaserpointShapePathForReflectors();
    void setLaserpointShapePathForBinoculars();
    void setLaserpointShapePathForInspectors();
    void setDistanceForFootprint();
    void shadowZoneForLaser();
    void meteoWidgets(bool, bool, bool);
    void graphicItemMoveToStack(QGraphicsItem *movingItem, const QPointF& oldPosition);
    void updateForBeamInspection();
    void setFeasibleDutyCicle();
    //scene

    void addBeamInspectorLink();
    void setTreeModel();
    bool insertGraphicsItem(TreeModel::GraphicsItem graphicsItem);
    bool updateGraphicsItem(TreeModel::GraphicsItem graphicsItem);
    void updateGraphicsItemList();
    void updateEnvironmentItem();
    void removeRow();
    void updateLaserItem();
    void updateReflectorItem();
    void updateBeamInspectorItem();
    void updateBinocularItem();


private:
    typedef QPair<LaserPoint *, Reflector *> ReflectorNodePair;
    typedef QPair<LaserPoint *, Binocular *> BinocularNodePair;   
    typedef QPair<LaserPoint *, FootprintObject *> ObjectNodePair;
    typedef QPair<LaserPoint *, BeamInspector *> InspectorNodePair;
    InspectorNodePair selectedInspectorNodePair() const;

    //scene
    void createActions();
    void createMenus();
    void createToolBars();
    void createActionsForToolbar();
    void createUndoView();
    void clearScene();
    void setZValue(int z);
    void createSceneForOrigin(const QPointF&);
    Reflector *selectedReflector() const;
    LaserPoint *selectedLaserPoint() const;
    LabRoom *selectedLabRoom() const;
    Binocular *selectedBinocular() const;
    ReflectorLink *selectedLink() const;
    LabRoom *selectedLab() const;
    FootprintObject *selectedFootprint()const;
    BeamInspector *selectedBeamInspector() const;
    SafetySignItem *selectedSafetySignItem() const;
    ReflectorNodePair selectedReflectorNodePair() const;
    BinocularNodePair selectedBinocularNodePair() const;
    ObjectNodePair selectedObjectNodePair() const;
    BeamInspector selectedBeamInspectorNodePair() const;

    void setMaxEhnacedOpticalDiameter();
    void createStatusBar();
    void setStatusBar();
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

    void print();
    void on_printPreviewAction_triggered();
    void printReport(QPrinter* painter);
    void saveReportImages();
    void printHtml(const QString&);
    QString commandUndoTriggered()const;
    QString commandRedoTriggered()const;

    QPainterPath laserpointShapePath();

    TreeModel *treeModel;
    QItemSelectionModel *treeSelectionModel;

    QString curFile;
    QString curDirectoryFile;
    bool externalFile;
    QStringList recentFiles;
    double reflectorDistance;
    QTimer *timer;
    QStringList reflectorsFilenameList;
    QList <QImage> reflectorsGraphImageList;
    QStringList inspectorsFilenameList;
    QList <QImage> inspectorsGraphImageList;
    CentralWidget *laserWindow;
    QRect previewRect;
    QComboBox *sceneScaleCombo;
    QPainterPath shadowPathZone;
    QPainterPath ehnacedPathZone;
    QPainterPath hazardZone;    
    LaserReport *myLaserReport;

    int roomNumber;
    double beamDiameter;
    double myEMP;
    double powerErg;
    double scale;
    bool theme;

    DockHistory* myDockHistory;

    enum { MaxRecentFiles = 5 };

    //Scene
    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *viewMenu;
    QMenu *toolbarMenu;
    QMenu *reflectorsMenu;
    QMenu *sceneDetailsMenu;
    QMenu *helpMenu;
    QMenu *prospectiveMenu;
    QMenu *zoomMenu;
    QMenu *displayMenu;
    QMenu *environmentMenu;
    QMenu *placeMenu;
    QMenu *goggleMenu;
    QMenu *signsMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *environmentToolBar;
    QToolBar *sceneToolBar;
    QToolBar *signSafetyToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *setPreviewAct;
    QAction *exportImageAct;
    QAction *exportReportAct;
    QAction *descriptionAct;
    QAction *exitAction;
    QAction *quitAct;
    QAction *undoAction;
    QAction *redoAction;
    QAction *showDockHistory;
    QAction *laserSettingsAction;
    QAction *fieldSettingsAction;
    QAction *classifierSettingsAction;
    QAction *goggleSettingsAction;
    QAction *showDockAerealView;
    QAction *showGridAction;
    QAction *recentFileActions[MaxRecentFiles];

    QAction *selectAct;
    QAction *darkThemeAct;
    QAction *separatorAction;
    QAction *showDockWidgetEffects;
    QAction *showDockWidgetResults;
    QAction *showDockWidgetControls;
    QAction *showDockWidgetSkin;
    QAction *showDockWidgetGoggle;
    QAction *showReflectorsList;
    QAction *showDockLea;
    QAction *centerOnViewAction;
    QAction *setNewOriginAct;
    QAction *printPreviewAct;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomWinAction;
    QAction *zoomResetAction;
    QAction *statusBarViewAction;
    QAction *dragAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *onLineHelpAct;

    QUndoStack *undoStack=nullptr;
    QUndoView *undoView=nullptr;

    LaserPoint *laserpoint;
    Reflector *reflector;
    Binocular *binocular;
    BeamInspector *beamInspector;
    LabRoom *myLabRoom;
    LabRoom *myFakeRoom;
    FootprintObject *footprint;
    GridLines *gridlines;
    BeamInspectorDialog *beamInspectorDialog;

    int minZ;
    int maxZ;
    int seqNumber;
    int binSeqNumber;
    int footprintSeqNumber;
    int inspectorSeqNumber;
    int scaleNumber;
    QPointF origin;
    double gridUnit;
    bool gridOn;
    bool dragModeState;
    bool myLabRoomInserted;
    bool environmentState;
    bool statusBarVisible;

    QAction *addBinocularAct;
    QAction *addLabAct;
    QAction *addLaserSignAct;
    QAction *addForbiddenSignAct;
    QAction *addProtectionSignAct;
    QAction *addPinInspectorAction;
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

    QUndoCommand *addFootprintCommand;
    QUndoCommand *addReflectorCommand;
    QUndoCommand *addSafetySignCommand;
    QUndoCommand *addBinocularCommand;
    QUndoCommand *addBeamInspectorCommand;
    QUndoCommand *addMeteoCommand;
    QUndoCommand *deleteFootprintCommand;
    QUndoCommand *deleteReflectorCommand;
    QUndoCommand *deleteBinocularCommand;
    QUndoCommand *deleteBeamInspectorCommand;
    QUndoCommand *deleteSafetySignCommand;

    QGraphicsView *view;
    QList <Reflector *> myReflectors;
    QList <Binocular *> myBinoculars;
    QList <BeamInspector*> myBeamInspectors;
    QList <FootprintObject*> myFootprints;
    QList <LaserPoint*> laserPointList;
    QList <LabRoom*> labroomList;
    QList <SafetySignItem*> safetySignList;
    QPointF pointPosition;

    QModelIndex selectedIndex;
    QFont sceneFont;
    QTransform myTransform;
    QRectF boundingRect;
    QRectF boundingRectForOrigin;

    int footprintsCount=0;
};

#endif
