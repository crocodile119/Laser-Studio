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

class InspectorLink;

class BeamInspector : public QGraphicsObject
{
    Q_DECLARE_TR_FUNCTIONS(BeamInspector)

public:
    BeamInspector(double _inspectorDistance, double _wavelength, double _divergence, double _beamDiameter);
    ~BeamInspector();

    enum { Type = UserType + 9 };
    static const double PI;
    static const double Le;
    static const double fe_min;

    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    void setPixScale(const double &);
    void addInspectorLink(InspectorLink *inspectorlink);
    void removeInspectorLink(InspectorLink *inspectorlink);
    void setInspectorSeqNumber(const int&_inspectorSeqNumber);
    int  getInspectorSeqNumber() const;
    void laserPositionChanged();
    void laserParametersChanged();
    double getInspectorDistance();
    double getLinkInspectorPhase()const;
    void setDescription(const QString&);
    QString getDescription()const;
    void inspectorUpdate();

    void setWavelength(const double&);
    double getWavelength()const;

    void setDivergence(const double&);
    double getDivergence()const;

    void setBeamDiameter(const double&);
    double getBeamDiameter()const;

    void setStringPosition();
    int type() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    QString getStringPosition()const;
    QString getLaserInstallation();
    void setInZone(bool _inZone);
    static void computeRayleighDistance(const double&, const double&, const double&);
    static double getRayleighDistance();
    static void computeTEM00_RayleighDistance(const double&, const double&);
    static void computeQualityFactor(const double&, const double&);
    static double getQualityFactor();
    static QString kindOfOcularDamage(const double &_wavelength);

    //parte dedicata ai dati membro relativi alle caratteristiche del fascio
    void computeSpotDiameter();
    void computeCurvaureRadius(const double& distance);
    void computeFm();
    void compute_d_r_FarField();
    void computePercentError();
    bool isFmFocusable();
    bool isFarField();

    void compute_d_r();
    void compute_alpha_r();
    void compute_d_s();
    void computeCE();

    double getSpotDiameter()const;
    double getCurvatureRadius()const;
    double get_fm()const;
    double get_alpha_r()const;
    double get_d_r()const;
    double getPercentError()const;
    double get_d_r_FarField()const;
    double get_d_s()const;
    double getCE()const;

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;

private:
    static double rayleighDistance; 
    static double TEM00_RayleighDistance;
    static double qualityFactor;

    QSet<InspectorLink *> myInspectorLinks;
    QRectF outlineRect() const;

    double opticalDiameter;
    double linkInspectorPhase;

    QString myText;
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
    QStringList stringList;
    QPixmap inspectorPix;
    QRectF myTextRect;
    QString description;

    QPainterPath path;
    bool inZone;

    //parte dedicata ai dati membro relativi alle caratteristiche del fascio

    double spotDiameter;
    double fm;// valore della distanza focale del cristallino che minimizza l'immagine retinica;
    double curvatureRadius;
    double d_r;//diametro dell'immagine retinica
    double alpha_r;//angolo sotteso dall'immagine retinica=angolo sotteso dalla sorgente apparente
    double d_s;//diametro della sorgente apparente
    double d_r_FarField;//diametro della sorgente in campo lontano
    double alpha_r_FarField;//angolo sotteso dalla sorgente apparente in campo lontano
    double percentError;
    double CE;
};

#endif