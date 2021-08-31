#include <QtGui>
#include <QInputDialog>
#include <QLineEdit>
#include <QFont>
#include <QFontMetricsF>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGuiApplication>
#include <QDir>
#include <QPalette>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QBitmap>
#include <cmath>
#include "inspectorlink.h"
#include "beaminspector.h"
#include "computeemp.h"

const double BeamInspector::PI = 3.1415926535897932384626433832795;
const double BeamInspector::Le= 17.0*1.0e-03;//distanza tra il cristallino e la retina in m
const double BeamInspector::fe_min= 14.53*1.0e-03;//distanza di focalizzazione minima per l'occhio in m
double BeamInspector::rayleighDistance;
double BeamInspector::TEM00_RayleighDistance;
double BeamInspector::qualityFactor;
double BeamInspector::pixHeight=20;

BeamInspector::BeamInspector(double _inspectorDistance, double _wavelength, double _divergence, double _beamDiameter): QGraphicsObject(),
     myTextColor(Qt::black), myBackgroundColor(Qt::white), myOutlineColor(Qt::transparent), myBeamColor(Qt::darkGray),
     scale(1.0), inspectorDistance(_inspectorDistance), wavelength(_wavelength), divergence(_divergence), beamDiameter(_beamDiameter),
     inspectorPix(":/images/inspectorpix.png")
{

    inspectorUpdate();
    setStringPosition();
    setToolTip(position);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

BeamInspector::~BeamInspector()
{
    foreach (InspectorLink *inspectorlink, myInspectorLinks)
        delete inspectorlink;
}

void BeamInspector::addInspectorLink(InspectorLink *inspectorlink)
{
    myInspectorLinks.push_back(inspectorlink);
}

void BeamInspector::removeInspectorLink()
{
    myInspectorLinks.clear();
}

int BeamInspector::type() const
{
    return Type;
}

QString BeamInspector::text() const
{
    return myText;
}

void BeamInspector::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor BeamInspector::textColor() const
{
    return myTextColor;
}

void BeamInspector::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor BeamInspector::outlineColor() const
{
    return myOutlineColor;
}

void BeamInspector::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor BeamInspector::backgroundColor() const
{
    return myBackgroundColor;
}

QRectF BeamInspector::boundingRect() const
{
    const int Margin = 1;
    QRectF rect;
    rect=outlineRect();

    return rect.adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BeamInspector::shape() const
{
    QRectF myRect=outlineRect();
    QPainterPath path;
    path.addRect(myRect);

    return path;
}

void BeamInspector::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
    }
    pen.setCosmetic(true);

    prepareGeometryChange();

    painter->setPen(pen);
    QRectF rect = outlineRect();
    painter->drawRect(rect);
    QRectF pixRect=outlineRect();

    inspectorPix=QPixmap(":/images/inspectorpix.png");

    QRectF source(0.0, 0.0, inspectorPix.width(), inspectorPix.height());
    painter->drawPixmap(pixRect, inspectorPix, source);

    QTransform transform=painter->transform();

    double m13=transform.m13();
    double m23=transform.m23();
    double m32=transform.m32();
    double m31=transform.m31();
    double m21=transform.m21();
    double m12=transform.m12();

    QRectF boundingRect=outlineRect();
    painter->save();

    painter->resetTransform();
    QTransform myTransform=QTransform(1.0, m12, m13, m21, 1.0, m23, m31, m32, 1.0);

    painter->setTransform(myTransform);

    QPen textPen(Qt::black);
    painter->setPen(textPen);

    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);
    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

QVariant BeamInspector::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        foreach (InspectorLink *inspectorlink, myInspectorLinks)
        {
            inspectorDistance=inspectorlink->linkInspectorLenght();
            inspectorlink->trackNodes();
            linkInspectorPhase=inspectorlink->linkInspectorPhase();

            inspectorUpdate();

            setStringPosition();
            setToolTip(position);
        }
        if (change == ItemSelectedChange) {
            return isSelected();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void BeamInspector::laserParametersChanged()
{
  foreach (InspectorLink *inspectorlink, myInspectorLinks)
  {
      inspectorDistance=inspectorlink->linkInspectorLenght();
      inspectorlink->trackNodes();
      linkInspectorPhase=inspectorlink->linkInspectorPhase();

      inspectorUpdate();

      setStringPosition();
      setToolTip(position);
  }
}

void BeamInspector::laserPositionChanged()
{
  foreach (InspectorLink *inspectorlink, myInspectorLinks)
  {
      inspectorDistance=inspectorlink->linkInspectorLenght();
      inspectorlink->trackNodes();
      linkInspectorPhase=inspectorlink->linkInspectorPhase();

      inspectorUpdate();

      setStringPosition();
      setToolTip(position);
   }
}

QRectF BeamInspector::outlineRect() const
{
    double rectWidht=inspectorPix.width()/scale;
    double rectHeight=inspectorPix.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF bottom=rect.bottomLeft();
    QPointF center=rect.center();
    rect.translate(QPointF(-center.x(), -bottom.y()));
    //rect.translate(-center);
    return rect;
}

QPointF BeamInspector::positionShift(const double & scale)
{
    double rectHeight=pixHeight/scale;
    return QPointF(0.0, rectHeight/2);
}

void BeamInspector::setStringPosition()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();

    QString xString;
    QString yString;
    QString inspectorDistanceString;
    QString CE_String;

    xString=QString::number(xCoordinate=pos().x(),'f', 2);
    yString=QString::number(yCoordinate=pos().y(),'f', 2);
    inspectorDistanceString=QString::number(inspectorDistance,'f',2);
    CE_String=QString::number(CE,'2', 2);


    position ="Segnaposto di indagine" + QString(" (%1,%2) <br>dist[m]: %3 C<sub>E</sub>: %4<br>")
                                    .arg(xString)
                                    .arg(yString)           
                                    .arg(inspectorDistanceString)
                                    .arg(CE_String);

}

QString BeamInspector::getStringPosition()const
{
   return position;
}

void BeamInspector::setPixScale(const double &newScale)
{
    scale=newScale;
}

void BeamInspector::setInspectorSeqNumber(const int& _inspectorSeqNumber)
{
    inspectorSeqNumber=_inspectorSeqNumber;
}

int BeamInspector::getInspectorSeqNumber() const
{
    return inspectorSeqNumber;
}

double BeamInspector::getInspectorDistance()
{
    return inspectorDistance;
}

void BeamInspector::setDescription(const QString& _description)
{
    description=_description;
}

QString BeamInspector::getDescription() const
{
    return description;
}

InspectorLink* BeamInspector::getBeamInspectorLink()
{
    return myInspectorLinks.first();
}

void BeamInspector::setWavelength(const double& _wavelength)
{
    wavelength=_wavelength;
}

double BeamInspector::getWavelength()const
{
    return wavelength;
}

void BeamInspector::setBeamDiameter(const double& _beamDiameter)
{
    beamDiameter=_beamDiameter;
}

double BeamInspector::getBeamDiameter()const
{
    return beamDiameter;
}

void BeamInspector::setDivergence(const double& _divergence)
{
    divergence=_divergence;
}

double BeamInspector::getDivergence()const
{
    return divergence;
}

void BeamInspector::setInZone(bool _inZone)
{
    inZone=_inZone;
}

double BeamInspector::getLinkInspectorPhase()const
{
    return linkInspectorPhase;
}

void BeamInspector::computeTEM00_RayleighDistance(const double& _wavelength, const double& _beamDiameter)
{
    TEM00_RayleighDistance=PI/2*pow(_beamDiameter,2)/(_wavelength)*1e+03;
}

void BeamInspector::computeQualityFactor(const double& _beamDiameter, const double& _divergence)
{
    double divergenceTem00=_beamDiameter/TEM00_RayleighDistance;
    qualityFactor=_divergence/divergenceTem00;
}

void BeamInspector::computeRayleighDistance(const double& _wavelength, const double& _beamDiameter, const double& _divergence)
{
    computeTEM00_RayleighDistance(_wavelength, _beamDiameter);
    computeQualityFactor(_beamDiameter, _divergence);
    rayleighDistance=TEM00_RayleighDistance/qualityFactor;
}

double BeamInspector::getRayleighDistance()
{
    return rayleighDistance;
}

double BeamInspector::getQualityFactor()
{
    return qualityFactor;
}

//parte dedicata alle fuzioni membro relative alle caratteristiche del fascio

void BeamInspector::computeCurvaureRadius(const double& distance)
{
    curvatureRadius=distance+pow(rayleighDistance, 2)/distance;
}

bool BeamInspector::isFmFocusable()
{
    bool isFocusable=false;
    double comigatePointOfLe=1/fe_min-1/Le;
    bool f_minCondition=(1/curvatureRadius)<=comigatePointOfLe;
    if((fm<=Le) && (f_minCondition))
        isFocusable=true;

    return isFocusable;
}

void BeamInspector::computeFm()
{
    double inverseFm=1/Le+1/curvatureRadius;
        if(isFmFocusable())
        {
            fm=1/inverseFm;
        }
        else
        {
            if(1/inverseFm>Le)
                fm=Le;
            else
                fm=fe_min;
        }
}

void BeamInspector::compute_d_r()
{
    if((wavelength>=400)&&(wavelength<=1400))
    {
        if(isFmFocusable())
        {
            d_r=beamDiameter*1.0e-03*Le/sqrt(pow(inspectorDistance,2)+pow(rayleighDistance,2));
        }
        else
        {
            double zR_L1_square=pow(inspectorDistance,2)+pow(rayleighDistance,2);
            d_r=beamDiameter*1.0e-03/sqrt(zR_L1_square)*sqrt(pow(Le, 2)+pow((zR_L1_square/rayleighDistance),2)*
                                                    pow((1-Le/fm+(beamDiameter*1.0e-03*Le)/zR_L1_square),2));
        }
    }
    else
        d_r=0;
}

void BeamInspector::compute_alpha_r()
{
    if((wavelength>=400)&&(wavelength<=1400))
        {
        if(isFmFocusable())
        {
            alpha_r=beamDiameter/sqrt(pow(inspectorDistance,2)+pow(rayleighDistance,2));
        }

        else
        {
            double zR_L1_square=pow(inspectorDistance,2)+pow(rayleighDistance,2);
            alpha_r=beamDiameter/sqrt(zR_L1_square)*sqrt(1+pow((zR_L1_square/rayleighDistance),2)*
                                                        pow((1/Le-1/fm+(inspectorDistance/zR_L1_square)),2));
        }
    }
    else
        alpha_r=0;
}

void BeamInspector::compute_d_s()
{
    d_s=alpha_r*curvatureRadius;
}

void BeamInspector::computeSpotDiameter()
{
    spotDiameter=beamDiameter+divergence*inspectorDistance;
}

double BeamInspector::getSpotDiameter()const
{
    return spotDiameter;
}

double BeamInspector::getCurvatureRadius()const
{
    return curvatureRadius;
}

double BeamInspector::get_fm()const
{
    return fm;
}

double BeamInspector::get_alpha_r()const
{
    return alpha_r;
}

double BeamInspector::get_d_s()const
{
    return d_s;
}

double BeamInspector::get_d_r()const
{
    return d_r;
}

QString BeamInspector::kindOfOcularDamage(const double& _wavelength)
{
    QString damage;
    if(_wavelength<400)
    {
        damage= "fotocheratite(infiammazione corneale);"
                "\ncongiuntivite (infiammazione congiuntivale);"
                "\ncataratta (opacizzazione del cristallino.";
    }
    else if((_wavelength>=400)&&(_wavelength<=1400))
    {
        damage="la radiazione è assorbita dall'epitelio pigmentato e dalla coroide;"
               "\nbruciatura della retina con conseguente perdita parziale o tatale della capacità visiva.";
    }
    else if((_wavelength>1400)&&(_wavelength<=3000))
    {
        damage="bruciatura del cristallino;"
                "\nbruciatura della cornea.";
    }
    else if((_wavelength>3000)&&(_wavelength<1.0e+06))
        damage="bruciatura del cristallino.";

    return damage;
}

void BeamInspector::compute_d_r_FarField()
{
    d_r_FarField=1.0e-03*beamDiameter*Le/inspectorDistance;
}

double BeamInspector::get_d_r_FarField()const
{
    return d_r_FarField;
}

void BeamInspector::computePercentError()
{
    percentError=(d_r_FarField-d_r)/d_r*100.0;
}

double BeamInspector::getPercentError()const
{
    return percentError;
}

void BeamInspector::computeCE()
{
    if(alpha_r<=ComputeEMP::ALPHA_MIN)
        CE=1.0;
    else if((ComputeEMP::ALPHA_MIN<alpha_r)&&(alpha_r<=ComputeEMP::ALPHA_MAX))
        CE=alpha_r/ComputeEMP::ALPHA_MIN;
    else if(alpha_r>ComputeEMP::ALPHA_MAX)
        CE=ComputeEMP::ALPHA_MAX/ComputeEMP::ALPHA_MIN;
}

double BeamInspector::getCE()const
{
    return CE;
}

bool BeamInspector::isFarField()
{
    bool farField=false;
    if((inspectorDistance/rayleighDistance)>10)
        farField=true;

    return farField;
}

void BeamInspector::inspectorUpdate()
{
    computeSpotDiameter(); 
    computeCurvaureRadius(inspectorDistance);
    computeFm();
    compute_d_r();
    compute_alpha_r();
    compute_d_r_FarField();
    computePercentError();
    computeCE();
    compute_d_s();
}
