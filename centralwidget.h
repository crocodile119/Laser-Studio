#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QUndoStack>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "dockresults.h"
#include "dockeffects.h"
#include "dockcontrols.h"
#include "dockskin.h"
#include "docklea.h"
#include "displayscene.h"
#include "graphicsitemtree.h"
#include "reflectorlink.h"
#include "reflector.h"
#include "labroom.h"
#include "safetysignitem.h"

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    const static double SCINTILLATION_CONST;
    const static double NEPERO_N;
    const static int STANDARD_VISIBILITY_DISTANCE;

    void setNewScene();
    void deleteScene();

    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);

    void setForce(const QString);
    void setCustomer(const QString);
    void set_UASL(const QString);
    void set_UASL_Assistant(const QString);
    void setLaserDescription(const QString);
    void setPlaceDescription(const QString);
    void setScale(const double& _scale);
    void setScaleIndex(const int&);
    int getScaleIndex()const;
    double getScale()const;

    void setAtmEffectsBool(bool);
    bool getAtmEffectsBool();

    void setScintillationBool(bool);
    bool getScintillationBool();

    void setAtmoshericEffectsCoefficient(const double&);
    double getAtmoshericEffectsCoefficient() const;

    void setA_Coefficient(const double& _a_Coefficient);
    void setMeteoRange(const int& _meteoRange);
    double getA_Coefficient() const;
    int getMeteoRange() const;

    void clearInstallationDesription();
    void setUndoStack(QUndoStack*);

    QString getForce()const;
    QString getCustomer()const;
    QString get_UASL()const;
    QString get_UASL_Assistant()const;
    QString getLaserDescription()const;
    QString getPlaceDescription()const;

    QRect getPreviewRect()const;

    DockControls *myDockControls;
    DockControls *outDockControls;
    DockResults *myDockResults;
    DockEffects *myDockEffects;
    DockSkin *myDockSkin;
    DockLea *myDockLea;
    DockGoggle *myDockGoggle;
    GraphicsItemTree *myGraphicsItemTree;
    DisplayScene *graphicsView;
    QGridLayout *gridLayout;
    QWidget *myPanel;

    //Getters per il laser
    QPointF getLaserPosition()const;
    qreal getLaserZValue()const;
    double getAperture()const;
    int getInstallation()const;
    bool isFilterOn();
    double getTransmittance()const;

    //Getters per i riflettori
    QVector <QPointF> getPosVect()const;
    QVector <int> getTypeVect()const;
    QVector <QString> getTextVect()const;
    QVector <QString> getStringPositionVect()const;
    QVector <double> getOpticalDiameterVect()const;
    QVector <double> getDivergenceVect()const;
    QVector <double> getReflectorDistanceVect()const;
    QVector <double> getReflectionCoeffVect()const;
    QVector <qreal> getZValueVect()const;
    QVector <target> getReflectorKindVect()const;
    QVector <double> getReflectorPositioningVect()const;
    QVector <QString> getDescriptionVect()const;

    //Getters per i dispositivi ottici
    QVector <QPointF> getBinocularPosVect()const;   
    QVector <double> getBinocularOpticalGainVect()const;
    QVector <int> getBinocularMagnificationVect()const;
    QVector <double> getBinocularTransmissionVect()const;
    QVector <int> getBinocular_D0Vect()const;
    QVector <QString > getBinocularDescriptionVect()const;

    //Getters per i segnaposti di ispezione.
    QVector <QPointF> getBeamInspectorPosVect()const;
    QVector <QString > getBeamInspectorDescriptionVect()const;

    double getLambertianMax()const;
    double getLaserEMP()const;
    double getPowerErg()const;
    double getBeamDiameter()const;

    //parametri ingombri
    QVector <QPointF> getFootprintPosVect();
    QVector <QRectF> getFootprintRectVect();
    QVector <QString> getFootprintDescriptionVect();

    //parametri cartelli
    QVector <QPointF> getSafetySignPosVect();
    QVector <SafetySignItem::SafetyClass> getSafetySignKindVect();

    //Getters per gli ambienti
    QPointF getLabPosition()const;
    QRectF getLabRect()const;
    int getRoomNumber()const;

    bool isLabRoomInserted();
    QDate getCompilingDate()const;
    void setCompilingDate(const QDate& _compilingDate);

    QRectF getMySceneRect()const;
    void setMySceneRect(const QRectF&);
    QPointF getOrigin()const;
    void setOrigin(const QPointF&);

signals:
    void modified();

private:
    quint32 MagicNumber = 0x6E51D683;


    //Installation description variables
    QString force;
    QString customer;
    QString uasl;
    QString uaslAssistant;
    LaserGoggle::material goggleMaterial;
    QString laserDescription;
    QString placeDescription;

    QList<Reflector*> myReflectors;
    QList<pair<Reflector*, int>> myReflectorsList;

    //Pararmetri del laser
    QPointF laserPosition;
    //bool laserIsSelected;
    qreal laserZValue;
    double aperture;
    int installation;
    bool filterOn;
    double transmittance;

    //Parametri riflettori
    double lambertianMax;
    double laserEMP;
    double laserBeamDiameter;
    double laserPowerErg;
    double scale;
    QRectF mySceneRect;
    QPointF origin;
    int scaleIndex;
    QVector <QPointF> posVect;
    QVector <bool> isSelectedVect;
    QVector <int> TypeVect;
    QVector <QString> TextVect;
    QVector <QColor> TextColorVect;
    QVector <QColor> BackgroundColorVect;
    QVector <QString> StringPositionVect;
    QVector <double> OpticalDiameterVect;
    QVector <double> DivergenceVect;
    QVector <double> ReflectorDistanceVect;
    QVector <double> ReflectionCoeffVect;
    QVector <qreal> ZValueVect;
    QVector <target> ReflectorKindVect;
    QVector <double> ReflectorPositioningVect;
    QVector <QString> ReflectorDescriptionVect;
    QPointF myLabPosition;
    QRectF myLabRect;    
    bool myLabRoomInserted;

    //Parametri dispositivi ottici
    QVector <QPointF> binocularPosVect;
    QVector <double> binocularOpticalGainVect;
    QVector <int> binocularMagnificationVect;
    QVector <double> binocularTransmissionVect;
    QVector <int> binocular_D0Vect;
    QVector <QString > binocularDescriptionVect;

    //Parametri dispositivi ottici
    QVector <QPointF> beamInspectorPosVect;
    QVector <QString > beamInspectorDescriptionVect;

    double atmoshericEffectsCoefficient;
    int meteoRange;
    int  roomNumber;
    double a_coefficient;

    //Parametri ingombri
    QVector <QPointF> footprintPosVect;
    QVector <QRectF> footprintRectVect;
    QVector <QString> footprintDescriptionVect;

    //parametri cartelli
    QVector <QPointF> SafetySignPosVect;
    QVector <SafetySignItem::SafetyClass> SafetySignKindVect;

    bool atmEffectsBool;
    bool scintillationBool;   

    QDate compilingDate;
    QRect previewRect;

    //variabile non impiegata
    bool free;
};
#endif // CENTRALWIDGET_H
