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
    void setDescription(const QString&);
    QString getDescription()const;

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
    void computeSpotDiameter();
    static void computeRayleighDistance(const double&, const double&, const double&);
    static double getRayleighDistance();
    static void computeTEM00_RayleighDistance(const double&, const double&);
    static void computeQualityFactor(const double&, const double&);
    static double getQualityFactor();

    //parte dedicata ai dati membro relativi alle caratteristiche del fascio
    void computeCurvaureRadius(const double& distance);
    void computeFm();
    bool isFmFocusable();

    void compute_d_r();
    void compute_alpha_r();

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

    double fm;// valore della distanza focale del cristallino che minimizza l'immagine retinica;
    double curvatureRadius;
    double d_r;
    double alpha_r;
};

#endif
