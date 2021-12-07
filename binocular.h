#ifndef BINOCULAR_H
#define BINOCULAR_H

#include <QApplication>
#include <QColor>
#include <QGraphicsObject>
#include <QPixmap>
#include <QSet>
#include <utility>
#include <QGraphicsEllipseItem>
#include "binocularlink.h"

class BinocularLink;

class Binocular : public QGraphicsObject
{
    Q_DECLARE_TR_FUNCTIONS(Binocular)

public:
    Binocular(double, double, double, double, double, QGraphicsObject *parent = nullptr);
    ~Binocular();

    enum { Type = UserType + 4 };
    const static double radDeg;
    const static double g;
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    void setDNRO_Diameter(double newRadius);
    void setOpticalGain(const double&);
    double getOpticalGain()const;
    void setPixScale(const double &);
    void addBinocularLink(BinocularLink *binocularlink);
    void removeBinocularLink();
    BinocularLink *getBinocularLink();
    void setBinSeqNumber(const int&);
    int  getBinSeqNumber() const;
    QString getOpticalGainFormula()const;
    bool isDangerous();
    void laserPositionChanged();
    void laserParametersChanged();
    double getExendedOpticalDiameter();
    double getBinocularDistance();
    void setDescription(const QString&);
    QString getDescription()const;
    QString getBinocularEffects();

    void setWavelength(const double&);
    double getWavelength()const;

    void setDivergence(const double&);
    double getDivergence()const;

    void setBeamDiameter(const double&);
    double getBeamDiameter()const;

    void set_D0(const int&);
    int get_D0();

    double get_Db();

    void setMagnification(const int&);
    int getMagnification()const;

    double getTransmissionCoeff()const;
    void setTransmissionCoeff(const double&);

    void setAtmoshericEffectsCoefficient(const double&);
    double setAtmoshericEffectsCoefficient() const;

    void setStringPosition();
    int type() const override;

    void computeOpticalGain();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    QString getStringPosition()const;
    QString getLaserInstallation();
    void setTextLabel();
    void setInZone(bool _inZone);
    void computeSpotDiameter();
    void computeExendedOpticalDiameter();
    void setBinocularObjectName(const QPointF& position);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;

private:
    QList<BinocularLink *> myBinocularLinks;
    QRectF outlineRect() const;
    QRectF labelRect();
    QRectF unitedBounding() const;

    double opticalDiameter;

    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    QColor myBeamColor;
    QString position;
    QString textLabel;
    int installationIndex;
    int binSeqNumber;
    double scale;
    double binocularDistance;
    double opticalGain;
    double exendedOpticalDiameter;  
    double wavelength;
    double divergence;
    double beamDiameter;
    int D0;
    double Db;
    int magnification;
    double transmissionCoeff;

    QStringList stringList;
    QString opticalGainFormula;
    QPixmap binocularPix;
    QRectF myTextRect;
    bool dangerous;   
    QString description;
    double atmoshericEffectsCoefficient;

    QPainterPath path;
    bool inZone;
};

#endif
