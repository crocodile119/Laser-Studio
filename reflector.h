#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <utility>
#include <vector>
#include <QObject>
#include <QGraphicsObject>
#include <QUndoStack>

#include "fresnelreflector.h"
#include "wetreflector.h"
#include "lambertianreflector.h"
#include "laserpoint.h"

class ReflectorLink;
enum target{WET_TARGET, GLASS_TARGET, LAMBERTIAN_TARGET, MIRROR_TARGET};

class Reflector : public QGraphicsObject
{
Q_DECLARE_TR_FUNCTIONS(Reflector)

public:
    Reflector(double, double, double, double, double, double, double, target);
    ~Reflector();

    enum { Type = UserType + 2 };
    int type() const override;
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;    
    void setBeamColor(const QColor &color);
    QColor beamColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    target getReflectorKind();
    void setReflectorColor();
    void setPixmap();
    void addReflectorLink(ReflectorLink *reflectorlink);
    void removeReflectorLink();
    ReflectorLink* getReflectorLink();
    void setOpticalDiameter(double);
    double getOpticalDiameter()const;
    void setSkinDistance(double);
    double getReflectorDistance()const;
    void reflectorOperation();
    void setTextLabel();
    void setStringDetails();
    QString getStringDetails();

    void setDivergence(const double);
    double getDivergence()const;

    void setLaserEMP(const double&);
    double getLaserEMP()const;

    void setLaserBeamDiameter(const double&);
    double getLaserBeamDiameter()const;

    void setLaserPowerErg(const double&);
    double getLaserPowerErg()const;

    void setSeqNumber(const double&);
    double getSeqNumber()const;

    bool isExendedDiffusion();
    void setIsExendedDiffusion(const bool);   
    void setLaserShape(const QPainterPath&);

    double getConstant()const;
    double getSpotDiameter()const;
    double getAlpha()const;
    double getCE()const;
    QString getKindOfSurface()const;
    double getNewRapSolution()const;
    double getAlphaIndicator()const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    //Compute reflection
    const static double radDeg;
    const static double phi_const;

    double getRefration_n() const;
    void setRefraction_n(const double&);
    double getDNRO() const;
    void setDNRO(const double&);
    double getObjectDistance()const;
    void setReflectorDistance(const double&);
    QRectF selectionReflectorRect() const;
    void setReflectorKindString();
    QString getReflectorKindString();
    void setTipString();
    void setLambertianMax(const double);
    double getReflectorHazardDistance();   
    void setPixScale(const double &);

    void setMaxElement();
    double getMaxElement();

    void setPositioningElement();
    double getPositioningElement();

    double getMaterialCoeff()const;
    void setMaterialCoeff(const double&);

    void setPositioning(const int&);
    int getPositioning()const;

    double getCorrectPositioning()const;
    QPointF getLaserPosition()const;

    void setDescription(const QString& _description);
    QString getDescription() const;

    void setAtmoshericEffectsCoefficient(const double&);
    double setAtmoshericEffectsCoefficient() const;
    double getLaserPhase()const;

    void setUndoStack(QUndoStack *);

    std::vector< std::pair <double,double> >getZsVect();
    std::vector< std::pair <double,double> >getRho_sVect();

    void setReflectorObjectName(const QPointF& position);
    QPolygonF createPolygon( vector< pair <double,double> > myVector);   
    bool isZRLShown();
    void setZRL(bool _showZRL);
    double retrieveLaserAperture()const;


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)override;
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;


public slots:
    void laserParametersChanged();
    void laserPositionChanged();

private:
    QRectF outlineRect() const;
    QRectF labelRect();
    QRectF unitedBounding() const;
    int roundness(double size) const;

    QList<ReflectorLink *> myReflectorLinks;
    double reflectorDistance; //la distanza di un probabile esposto dal riflettore
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    QColor myBeamColor;
    double refractionIndex;
    double skinDistance;
    double opticalDiameter;
    double divergence;
    double laserEMP;
    double laserBeamDiameter;
    double laserPowerErg;
    double reflectorHazardDistance;
    double laserPhase;
    QString textLabel;
    QString refrectorDetailsString;
    QString tipString;
    double seqNumber;
    QString reflectionKindString;
    QPainterPath hazardAreaPath;
    double dnro;
    double objectDistance;
    double myMaxElement;
    double myPositioningElement;
    double n;
    double lambertianMax;
    double materialCoeff;
    QString description;

    double constant;
    double spotDiameter;
    double alpha;
    double CE;
    QString kindOfSurface;
    double newRapSolution;
    double alphaIndicator;
    bool exendedDiffusion;
    bool pointSourceDiffusion;
    bool identifiedDiffusion;
    QPixmap reflectorPix;
    QRectF myTextRect;
    bool inHazardZone;
    QPainterPath laserShapePath;
    int positioning;
    double correctPositioning;
    double scale;    
    double atmoshericEffectsCoefficient;
    double phaseAngle;
    QUndoStack *undoStack=nullptr;
    QPointF laserPosition;

    FresnelReflector* MyFresnelReflector_ptr;
    WetReflector* MyWetReflector_ptr;
    LambertianReflector* MyLambertianReflector_ptr;
    LambertianReflector* MyReflector;
    vector< pair <double, double> > myZsVector;
    vector< pair <double, double> >rho_sVect;
    target myTarget;
    bool showZRL;
   };

#endif //end REFLECTOR_H
