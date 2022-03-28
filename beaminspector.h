#ifndef BEAMINSPECTOR_H
#define BEAMINSPECTOR_H

#include <QApplication>
#include <QColor>
#include <QGraphicsObject>
#include <QPixmap>
#include <QSet>
#include <utility>
#include <QGraphicsEllipseItem>
#include "inspectorlink.h"
#include "computeemp.h"
#include "dockcontrols.h"

class InspectorLink;

class BeamInspector : public QGraphicsObject
{
    Q_DECLARE_TR_FUNCTIONS(BeamInspector)

public:
    BeamInspector(double _inspectorDistance, double _wavelength, double _divergence, double _beamDiameter,
                  double _attenuatedDNRO, QGraphicsObject *parent = nullptr);
    ~BeamInspector();

    enum { Type = UserType + 9 };
    static const double PI;
    static const double Le;
    static const double fe_min;
    static const double NO_INTERESTING_EXPOSURE_TIME;
    static const double NO_MULTI_PULSE;
    static const double SHORT_TIME_LIMIT;
    static const double PUPIL_DIAMETER;

    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    void setPixScale(const double &);
    void addInspectorLink(InspectorLink *inspectorlink);
    void removeInspectorLink();
    void setInspectorSeqNumber(const int&_inspectorSeqNumber);
    int  getInspectorSeqNumber() const;
    void laserPositionChanged();
    void laserParametersChanged();
    double getInspectorDistance();
    void setPowerErgForEMP(const double&);
    void setEMP(const double&);
    void setEMP_Sort(const std::string&);
    void setExposureTime(const double&);
    void setAttenuatedDNRO(const double& _attenuatedDNRO);
    double getAttenuatedDNRO()const;
    double getPowerErgForEMP()const;
    double getEMP()const;
    std::string getEMP_Unit()const;
    double getAugmentedEMP()const;
    double getLongExposureEMP()const;
    std::string getLongExposureEMP_Unit()const;
    std::string getLongExposureEMP_Sort()const;
    InspectorLink* getBeamInspectorLink();
    double getLinkInspectorPhase()const;
    double getT1()const;
    double getT2()const;
    std::string getNotes()const;

    void setDescription(const QString&);
    QString getDescription()const;
    void inspectorUpdate();

    void setWavelength(const double&);
    double getWavelength()const;

    void setDivergence(const double&);
    double getDivergence()const;

    void setLaserOperation(const DockControls::operation&);
    DockControls::operation getLaserOperation()const;
    void setPowerErg(const double& _powerErg);

    void setBeamDiameter(const double&);
    double getBeamDiameter()const;
    void setNumberOfPulses(const double &);
    double getNumberOfPulses()const;

    void setStringPosition();
    void setTextLabel();
    int type() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    QString getStringPosition()const;
    QString getLaserInstallation();
    void setInZone(bool _inZone);
    bool isInZone()const;
    static void computeRayleighDistance(const double&, const double&, const double&);
    static double getRayleighDistance();
    static void computeTEM00_RayleighDistance(const double&, const double&);
    static void computeQualityFactor(const double&, const double&);
    static double getQualityFactor();
    static QString kindOfOcularDamage(const double &_wavelength);

    //parte dedicata ai dati membro relativi alle caratteristiche del fascio
    void computeSpotDiameter(); 
    void setEffectiveDiameter();
    double getEffectiveDiameter()const;
    void computeCurvaureRadius(const double& distance);
    void computeFm();
    void compute_d_r_FarField();
    void computePercentError();
    void computeForLongExposure();
    std::string mainEffect(const double& meanEMP, const double& pulseEMP);
    std::string getMainEffect()const;
    bool isLongExposure();
    bool isRetinalHazard();
    bool isFmFocusable();
    bool isFarField();
    bool isOutOfLaserAperture();
    static QPointF positionShift(const double &scale);

    void compute_d_r();
    void compute_alpha_r();
    void compute_d_s();
    void computeCE();
    bool isSafePosition();
    void valuatePosition();
    void valuateLongExposurePosition();
    std::string getEMP_Sort()const;
    std::string getLongExposition_EMP_Sort()const;
    void computeEMP_Unit();
    void computeLongExposure_EMP_Sort();
    void computeLongExposureEMP_Unit();
    void computeAugmented_EMP();

    double getSpotDiameter()const;
    double getCurvatureRadius()const;
    double get_fm()const;
    double get_alpha_r()const;
    double get_d_r()const;
    double getPercentError()const;
    double get_d_r_FarField()const;
    double get_d_s()const;
    double getCE()const;
    std::string getFormula()const;
    void setBeamInspectorObjectName(const QPointF& position);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;

private:
    static double rayleighDistance; 
    static double TEM00_RayleighDistance;
    static double qualityFactor;
    static double pixHeight;

    QList<InspectorLink *> myInspectorLinks;
    QRectF outlineRect() const;

    double opticalDiameter;
    double linkInspectorPhase;

    QString myText;
    QString textTip;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    QColor myBeamColor;
    QString position;
    QString textLabel;
    int installationIndex;
    int inspectorSeqNumber;
    double scale;
    double inspectorDistance;
    double wavelength;
    double divergence;
    double beamDiameter;
    double powerErg;
    double longExposurePowerErg;
    double attenuatedDNRO;
    double EMP_PoweErgRatio;
    double exposureTime;
    double numberOfPulses;
    QStringList stringList;
    QPixmap inspectorPix;
    QRectF myTextRect;
    QString description;

    QPainterPath path;
    bool inZone;

    double powerErgForEMP;
    double EMP;
    double augmented_EMP;
    double longExposure_EMP;
    std::string EMP_Sort;
    std::string EMP_Unit;
    std::string longExposure_EMP_Sort;
    std::string longExposure_EMP_Unit;
    std::string notes;

    //parte dedicata ai dati membro relativi alle caratteristiche del fascio

    double spotDiameter;
    double effectiveDiameter;
    double fm;// valore della distanza focale del cristallino che minimizza l'immagine retinica;
    double curvatureRadius;
    double d_r;//diametro dell'immagine retinica
    double alpha_r;//angolo sotteso dall'immagine retinica=angolo sotteso dalla sorgente apparente
    double d_s;//diametro della sorgente apparente
    double d_r_FarField;//diametro della sorgente in campo lontano
    double alpha_r_FarField;//angolo sotteso dalla sorgente apparente in campo lontano
    double percentError;
    double CE;
    std::string formula;
    double T1;
    double T2;
    std::string multiPulseMainEffect;
    DockControls::operation laserOperation;
    bool photochemical;
};

#endif
