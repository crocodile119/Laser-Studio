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


const double BeamInspector::PI = 3.1415926535897932384626433832795;
const double BeamInspector::Le= 17.0;//distanza tra il cristallino e la retina in mm
const double BeamInspector::fe_min= 14.53;//distanza di focalizzazione minima per l'occhio in mm
double BeamInspector::rayleighDistance;
double BeamInspector::TEM00_RayleighDistance;
double BeamInspector::qualityFactor;

BeamInspector::BeamInspector(double _inspectorDistance, double _wavelength, double _divergence, double _beamDiameter): QGraphicsObject(),
     myTextColor(Qt::black), myBackgroundColor(Qt::white), myOutlineColor(Qt::transparent), myBeamColor(Qt::darkGray),
     scale(1.0), inspectorDistance(_inspectorDistance), wavelength(_wavelength), divergence(_divergence), beamDiameter(_beamDiameter),
     inspectorPix(":/images/inspectorpix.png")
{
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
    myInspectorLinks.insert(inspectorlink);
}

void BeamInspector::removeInspectorLink(InspectorLink *inspectorlink)
{
    myInspectorLinks.remove(inspectorlink);
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

            //Elaborazione da inserire

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

      //Elaborazione da inserire

      setStringPosition();
  }
}

void BeamInspector::laserPositionChanged()
{
  foreach (InspectorLink *inspectorlink, myInspectorLinks)
  {
      inspectorDistance=inspectorlink->linkInspectorLenght();
      inspectorlink->trackNodes();

      //Elaborazione da inserire

      setToolTip(position);
      setStringPosition();
      qDebug()<<"setStringPosition()"<<getStringPosition();
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
    return rect;
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

    xString=QString::number(xCoordinate=pos().x(),'f', 0);
    yString=QString::number(yCoordinate=pos().y(),'f', 0);
        inspectorDistanceString=QString::number(inspectorDistance,'f',2);



    position ="Segnaposto di indagine" + QString(" (%1,%2) \ndist[m]: %3")
                                    .arg(xString)
                                    .arg(yString)           
                                    .arg(inspectorDistanceString);

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
    curvatureRadius=distance+ pow(rayleighDistance, 2)/distance;
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
    if(isFmFocusable())
    {
        double inverseFm=1/Le+1/(curvatureRadius);
        fm=1/inverseFm;
    }
    else
    {
        if(fm>Le)
            fm=Le;
        else
            fm=fe_min;
    }
}

void BeamInspector::compute_d_r()
{
    if(isFmFocusable())
    {
        d_r=beamDiameter*Le/sqrt(pow(inspectorDistance,2)+pow(rayleighDistance,2));
    }
    else
    {
        double zR_L1_square=pow(inspectorDistance,2)+pow(rayleighDistance,2);
        d_r=beamDiameter/sqrt(zR_L1_square)*sqrt(pow(Le, 2)+pow((zR_L1_square/rayleighDistance),2)*
                                                pow((1-Le/fm+(beamDiameter*Le)/zR_L1_square),2));
    }
}

void BeamInspector::compute_alpha_r()
{
    if(isFmFocusable())
    {
        alpha_r=beamDiameter/sqrt(pow(inspectorDistance,2)+pow(rayleighDistance,2));
    }

    else
    {
        double zR_L1_square=pow(inspectorDistance,2)+pow(rayleighDistance,2);
        d_r=beamDiameter/sqrt(zR_L1_square)*sqrt(1+pow((zR_L1_square/rayleighDistance),2)*
                                                    pow((1/Le-1/fm+(beamDiameter/zR_L1_square)),2));
    }
}
