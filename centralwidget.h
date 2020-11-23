#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "dockresults.h"
#include "dockeffects.h"
#include "dockcontrols.h"
#include "dockskin.h"
#include "displayscene.h"
#include "reflectorsqlist.h"
#include "link.h"
#include "reflector.h"
#include "labroom.h"

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    const static double SCINTILLATION_CONST;
    const static double NEPERO_N;

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

    QString getForce()const;
    QString getCustomer()const;
    QString get_UASL()const;
    QString get_UASL_Assistant()const;
    QString getLaserDescription()const;
    QString getPlaceDescription()const;

    DockControls *myDockControls;
    DockResults *myDockResults;
    DockEffects *myDockEffects;
    DockSkin *myDockSkin;
    DockGoggle *myDockGoggle;
    ReflectorsQList *myDockReflectorsList;
    DisplayScene *graphicsView;
    QLabel *label;
    QGridLayout *gridLayout;
    QWidget *myPanel;

    //Getters per il laser
    QPointF getLaserPosition()const;
    bool getLaserIsSelected()const;
    qreal getLaserZValue()const;
    double getAperture()const;
    int getInstallation()const;
    bool isFilterOn();
    double getTransmittance()const;

    //Getters per i riflettori
    QVector <QPointF> getPosVect()const;
    QVector <bool> getIsSelectedVect()const;
    QVector <int> getTypeVect()const;
    QVector <QString> getTextVect()const;
    QVector <QColor> getTextColorVect()const;
    QVector <QColor> getBackgroundColorVect()const;
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

    double getLambertianMax()const;
    double getLaserEMP()const;
    double getPowerErg()const;
    double getBeamDiameter()const;

    //parametri ingombri
    QVector <QPointF> getFootprintPosVect();
    QVector <QRectF> getFootprintRectVect();
    QVector <QString> getFootprintDescriptionVect();

    //Getters per gli ambienti
    QPointF getLabPosition()const;
    QRectF getLabRect()const;
    int getRoomNumber()const;

    bool getGridState()const;
    void setGridState(const bool&);

    bool isLabRoomInserted();

private slots:

signals:
    void modified();

private:
    quint32 MagicNumber = 0x6E51D683;


    //Installation description variables
    QString force;
    QString customer;
    QString uasl;
    QString uaslAssistant;
    QString laserDescription;
    QString placeDescription;

    QList<Reflector*> myReflectors;
    QList<pair<Reflector*, int>> myReflectorsList;

    //Pararmetri del laser
    QPointF laserPosition;
    bool laserIsSelected;
    qreal laserZValue;
    double aperture;
    int installation;
    bool filterOn;
    double transmittance;

    //Parametri scena
    bool gridState;

    //Parametri riflettori
    double lambertianMax;
    double laserEMP;
    double laserBeamDiameter;
    double laserPowerErg;
    double scale;
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

    double atmoshericEffectsCoefficient;
    int meteoRange;
    int  roomNumber;
    double a_coefficient;

    //Parametri ingombri
    QVector <QPointF> footprintPosVect;
    QVector <QRectF> footprintRectVect;
    QVector <QString> footprintDescriptionVect;

    bool atmEffectsBool;
    bool scintillationBool;
};
#endif // CENTRALWIDGET_H
