#ifndef LASERPOINT_H
#define LASERPOINT_H

#include <QApplication>
#include <QColor>
#include <QGraphicsObject>
#include <QPixmap>
#include <QSet>
#include <QGraphicsEllipseItem>
#include <QUndoStack>

class ReflectorLink;
class BinocularLink;
class ObjectLink;
class InspectorLink;

class LaserPoint : public QGraphicsObject
{
    Q_DECLARE_TR_FUNCTIONS(LaserPoint)

public:
    LaserPoint();
    ~LaserPoint();

    enum { Type = UserType + 1 };
    const static double radDeg;
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    void setOpticalDiameter(double);
    void setSkinDistance(double);

    double getOpticalDiameter()const;
    double getSkinDistance()const;

    int getInstallationIndex()const;
    double getAperture()const;
    double getPillow()const;

    void setInstallationIndex(const int&);
    void setAperture(const double&);
    void computePillowAreas();

    void setStringPosition();
    int type() const override;

    void addReflectorLink(ReflectorLink *reflectorlink);
    void addBinocularLink(BinocularLink *binocularlink);
    void addObjectLink(ObjectLink *objectLink);
    void addInspectorLink(InspectorLink *inspectorLink);
    void removeReflectorLink(ReflectorLink *reflectorlink);
    void removeBinocularLink(BinocularLink *binocularlink);
    void removeObjectLink(ObjectLink *objectLink);
    void removeInspectorLink(InspectorLink *inspectorLink);

    QPainterPath shapePath();  
    bool shapePathContainsPoint(const QPointF&);

    bool shapeEnhacedPathContainsPoint(const QPointF&, const double&);
    QPainterPath shapeEnhacedPath(const double&);
    QPainterPath getLaserShapePath();
    QPainterPath getShapeEnhacedPath();

    void setPixScale(const double &);
    void setUndoStack(QUndoStack *);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    QString getStringPosition()const;
    QString getLaserInstallation();
    static QString getInstallationforIndex(const int&);
    void setDutyCycleCheck(bool _feasibleDutyCycle);
    void setTextLabel();
    void setStrings();   
    void setShadowZone(const QPainterPath&);
    void setEhnacedShadowZone(const QPainterPath&);
    void setTrasmittance(const double&);
    double getTransmittance()const;
    bool isFilterOn();
    void setFilterOn(bool);
    void setRayleighDistance(const double& _rayleighDistance);
    double getRayleighDistance()const;
    void setQualityFactor(const double& _rayleighDistance);
    double getQualityFactor()const;

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;

private:
    QRectF outlineRect() const;
    QRectF selectionOpticalRect() const;
    QRectF selectionSkinRect() const;
    QRectF unitedBounding() const;
    QRectF labelRect();
    int roundness(double size) const;
    double opticalDiameter;
    double skinDiameter;
    double rayleighDistance;
    double qualityFactor;

    QSet<ReflectorLink *> myReflectorLinks;
    QSet<BinocularLink *> myBinocularLinks;
    QSet<ObjectLink *> myObjectLinks;
    QSet<InspectorLink *> myInspectorLinks;

    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    QColor myBeamColor;
    QString position;
    QString textLabel;
    int installationIndex;
    double aperture;
    double pillow;
    double scale; 
    bool feasibleDutyCycle;

    QStringList stringList;
    QPixmap laserPix;
    QPixmap prohibitedPix;
    QRectF myTextRect;
    QPainterPath laserPath;
    QPainterPath enhacedPath;
    QPainterPath laserShadowZone;
    QPainterPath enhacedShadowZone;
    QRectF enhacedPathBoudingRect;
    bool filterOn;
    double transmittance;
    QUndoStack *undoStack=nullptr;
};

#endif
