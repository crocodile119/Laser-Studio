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
#include "binocularlink.h"
#include "binocular.h"


const double Binocular::radDeg = 3.1415926535897932384626433832795/180;
const double Binocular::g= 7.0;

Binocular::Binocular(double DNRO, double _binocularDistance, double _wavelength, double _divergence, double _beamDiameter): QGraphicsObject(),
     opticalDiameter(2*DNRO), myTextColor(Qt::black), myBackgroundColor(Qt::white), myOutlineColor(Qt::transparent), myBeamColor(Qt::darkGray),
     scale(1.0), binocularDistance(_binocularDistance), wavelength(_wavelength), divergence(_divergence), beamDiameter(_beamDiameter), D0(50),
     magnification(5), transmissionCoeff(0.90), binocularPix(":/images/binocularpix.png")
{
    computeOpticalGain();
    exendedOpticalDiameter=2*DNRO*sqrtf(opticalGain);

    if(exendedOpticalDiameter>binocularDistance)
        dangerous=true;
    else
        dangerous=false;

    setStringPosition();
    setTextLabel();
    setToolTip(position);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

Binocular::~Binocular()
{
    foreach (BinocularLink *binocularlink, myBinocularLinks)
        delete binocularlink;
}

void Binocular::addBinocularLink(BinocularLink *binocularlink)
{
    myBinocularLinks.push_back(binocularlink);
}

void Binocular::removeBinocularLink()
{
    myBinocularLinks.clear();
}

BinocularLink* Binocular::getBinocularLink()
{
    return myBinocularLinks.first();
}

int Binocular::type() const
{
    return Type;
}

QString Binocular::text() const
{
    return myText;
}

void Binocular::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor Binocular::textColor() const
{
    return myTextColor;
}

void Binocular::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Binocular::outlineColor() const
{
    return myOutlineColor;
}

void Binocular::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Binocular::backgroundColor() const
{
    return myBackgroundColor;
}

QRectF Binocular::boundingRect() const
{
    const int Margin = 1;
    QRectF rect;
    rect=unitedBounding();

    return rect.adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath Binocular::shape() const
{
    QRectF myRect=outlineRect();
    QPainterPath path;
    path.addRect(myRect);

    return path;
}

void Binocular::paint(QPainter *painter,
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

    binocularPix=QPixmap(":/images/binocularpix.png");

    QRectF source(0.0, 0.0, binocularPix.width(), binocularPix.height());
    painter->drawPixmap(pixRect, binocularPix, source);

    QTransform transform=painter->transform();

    double m13=transform.m13();
    double m23=transform.m23();
    double m32=transform.m32();
    double m31=transform.m31();
    double m21=transform.m21();
    double m12=transform.m12();

    QRectF boundingRect=labelRect();
    painter->save();

    painter->resetTransform();
    QTransform myTransform=QTransform(1.0, m12, m13, m21, 1.0, m23, m31, m32, 1.0);

    painter->setTransform(myTransform);

    QPen textPen(Qt::black);
    painter->setPen(textPen);

    setTextLabel();

    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);
    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

QVariant Binocular::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        foreach (BinocularLink *binocularlink, myBinocularLinks)
        {
            binocularDistance=binocularlink->linkBinocularLenght();
            binocularlink->trackNodes();
            computeOpticalGain();
            computeExendedOpticalDiameter();
            isDangerous();
            setStringPosition();
            setToolTip(position);
            setTextLabel();
        }
        if (change == ItemSelectedChange) {
            return isSelected();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void Binocular::laserParametersChanged()
{
  foreach (BinocularLink *binocularlink, myBinocularLinks)
  {
      binocularDistance=binocularlink->linkBinocularLenght();
      binocularlink->trackNodes();
      computeOpticalGain();
      computeExendedOpticalDiameter();
      isDangerous();
      setStringPosition();
  }
}

void Binocular::laserPositionChanged()
{
  foreach (BinocularLink *binocularlink, myBinocularLinks)
  {
      binocularDistance=binocularlink->linkBinocularLenght();
      binocularlink->trackNodes();
      computeOpticalGain();
      computeExendedOpticalDiameter();
      isDangerous();
      setToolTip(position);
      setStringPosition();
      setTextLabel();
      qDebug()<<"setStringPosition()"<<getStringPosition();
   }
}

QRectF Binocular::outlineRect() const
{
    double rectWidht=binocularPix.width()/scale;
    double rectHeight=binocularPix.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF center=rect.center();
    rect.translate(-center);
    return rect;
}

QRectF Binocular::unitedBounding() const
{
    QRectF unitedRect=outlineRect();
    unitedRect.adjust(-0.5*outlineRect().width(), -0.5*outlineRect().height(),
                      outlineRect().width(), outlineRect().height());
    return unitedRect;
}

void Binocular::setOpticalGain(const double &_opticalGain)
{
    opticalGain=_opticalGain;
}

double Binocular::getOpticalGain()const
{
    return opticalGain;
}

void Binocular::setDNRO_Diameter(double newRadius)
{
   if(2*newRadius==opticalDiameter)
       return;

    prepareGeometryChange();
    opticalDiameter=2*newRadius;
    laserParametersChanged();
}

void Binocular::setStringPosition()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();

    QString xString;
    QString yString;
    QString exendedOpticalDiameterString;
    QString binocularDistanceString;
    QString opticalGainString;
    QString DbString;
    QString D0String;
    QString magnificationString;
    QString transmissionCoeffString;

    xString=QString::number(xCoordinate=pos().x(),'f', 0);
    yString=QString::number(yCoordinate=pos().y(),'f', 0);
        binocularDistanceString=QString::number(binocularDistance,'f',2);      
        exendedOpticalDiameterString=QString::number(exendedOpticalDiameter/2,'f',2);
        transmissionCoeffString=QString::number(transmissionCoeff,'f',2);
        magnificationString=QString::number(magnification,'f',2);
        DbString=QString::number(Db,'f',2);
        D0String=QString::number(D0,'f',2);
        opticalGainString=QString::number(opticalGain,'f',2);


    QString effectsString;
    qDebug()<<"isDangerous():"<< isDangerous();
    if(isDangerous())
        effectsString="PERICOLOSO";
                else
                effectsString.clear();

    if((wavelength>=400)&&(wavelength<=1400))
    {
    position ="Dispositivo ottico" + QString(" (%1,%2) <br>dist[m]: %3, DNROE[m]: %4 <br>τ= %5, D<sub>b</sub>[mm]=%6, D<sub>0</sub>[mm]=%7, <br>M= %8, K=%9 <br>%10")
                                    .arg(xString)
                                    .arg(yString)           
                                    .arg(binocularDistanceString)
                                    .arg(exendedOpticalDiameterString)
                                    .arg(transmissionCoeffString)
                                    .arg(DbString)
                                    .arg(D0String)
                                    .arg(magnificationString)
                                    .arg(opticalGainString)
                                    .arg(effectsString);
    }

    else if(((wavelength>=320)&&(wavelength<400))||((wavelength>1400)&&(wavelength<=4500)))
    {
        position ="Dispositivo ottico" + QString(" (%1,%2) \ndist[m]: %3, DNROE[m]: %4 \nτ= %5, M= %6, K=%7 \n%8")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(binocularDistanceString)
                                        .arg(exendedOpticalDiameterString)
                                        .arg(transmissionCoeffString)
                                        .arg(magnificationString)
                                        .arg(opticalGainString)
                                        .arg(effectsString);
    }
    else
    {
    position ="Dispositivo ottico" + QString(" (%1,%2) \ndist[m]: %3, DNROE[m]: %4")
                                    .arg(xString)
                                    .arg(yString)
                                    .arg(binocularDistanceString)
                                    .arg(exendedOpticalDiameterString);
    }
}

void Binocular::setTextLabel()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();


    QString xString;
    QString yString;

    xString=QString::number(xCoordinate=pos().x(),'f', 0);
    yString=QString::number(yCoordinate=pos().y(),'f', 0);
    prepareGeometryChange();
    textLabel ="dispositivo ottico\n"+QString("(%1,%2)")
                                    .arg(xString)
                                    .arg(yString);
    update();
}

QString Binocular::getStringPosition()const
{
   return position;
}

QRectF Binocular::labelRect()
{
        const int Padding = 10;
        QFontMetricsF metrics =(QFontMetricsF)qApp->font();
        myTextRect = metrics.boundingRect(textLabel);
        myTextRect.adjust(-Padding, -Padding, +Padding, +Padding);
        QRectF pixRect=QRectF(0, 0, 60.0, 60.0);

        QPointF rectPoint(-pixRect.width()/2+Padding, pixRect.height()/2+myTextRect.height()/2+Padding);
        myTextRect.translate(rectPoint);
        return myTextRect;
}

void Binocular::setPixScale(const double &newScale)
{
    scale=newScale;
}

void Binocular::setBinSeqNumber(const int& _binSeqNumber)
{
    binSeqNumber=_binSeqNumber;
}

int Binocular::getBinSeqNumber() const
{
    return binSeqNumber;
}

bool Binocular::isDangerous()
{
    if((exendedOpticalDiameter>2*binocularDistance)&&(inZone))
        dangerous=true;
    else
        dangerous=false;

    return dangerous;
}

void Binocular::setAtmoshericEffectsCoefficient(const double& _atmoshericEffectsCoefficient)
{
    atmoshericEffectsCoefficient=_atmoshericEffectsCoefficient;
}

double Binocular::setAtmoshericEffectsCoefficient() const
{
    return atmoshericEffectsCoefficient;
}

void Binocular::computeExendedOpticalDiameter()
{
    exendedOpticalDiameter=opticalDiameter*sqrtf(opticalGain);
}

double Binocular::getExendedOpticalDiameter()
{
    return exendedOpticalDiameter;
}

double Binocular::getBinocularDistance()
{
    return binocularDistance;
}

void Binocular::setDescription(const QString& _description)
{
    description=_description;
}

QString Binocular::getDescription() const
{
    return description;
}

void Binocular::setWavelength(const double& _wavelength)
{
    wavelength=_wavelength;
}

double Binocular::getWavelength()const
{
    return wavelength;
}

void Binocular::setBeamDiameter(const double& _beamDiameter)
{
    beamDiameter=_beamDiameter;
}

double Binocular::getBeamDiameter()const
{
    return beamDiameter;
}

void Binocular::setDivergence(const double& _divergence)
{
    divergence=_divergence;
}

double Binocular::getDivergence()const
{
    return divergence;
}

void Binocular::set_D0(const int& _D0)
{
    D0=_D0;
}

int Binocular::get_D0()
{
    return D0;
}

void Binocular::setMagnification(const int& _magnification)
{
    magnification=_magnification;
}

int Binocular::getMagnification()const
{
    return magnification;
}

double Binocular::getTransmissionCoeff()const
{
    return transmissionCoeff;
}

void Binocular::setTransmissionCoeff(const double& _transmissionCoeff)
{
    transmissionCoeff=_transmissionCoeff;
}

void Binocular::computeOpticalGain()
{
    if((wavelength>=400)&&(wavelength<=1400))
    {
        double k_evaluation_1;
        double k_evaluation_2;
        double k_evaluation_3;

        k_evaluation_1=transmissionCoeff*pow(magnification, 2.0);

        if(k_evaluation_1<1)
        k_evaluation_1=1.0;

        k_evaluation_2=transmissionCoeff*pow(D0,2.0)/pow(g,2.0);
        if(k_evaluation_2<1)
        k_evaluation_2=1.0;

        computeSpotDiameter();

        k_evaluation_3=transmissionCoeff*pow(Db,2.0)/pow(g,2.0);
        if(k_evaluation_3<1)
        k_evaluation_3=1.0;

        if((k_evaluation_1<k_evaluation_2)and(k_evaluation_1<k_evaluation_3))
        {
            opticalGain=k_evaluation_1;
            opticalGainFormula="τ M^2";
        }
        else if((k_evaluation_2<k_evaluation_1)and(k_evaluation_2<k_evaluation_3))
        {
            opticalGain=k_evaluation_2;
            opticalGainFormula="τ D0^2 / g^2";
        }
        else if((k_evaluation_3<=k_evaluation_2)and(k_evaluation_3<=k_evaluation_1))
        {
            opticalGain=k_evaluation_3;
            opticalGainFormula="τ Db^2 / g^2";
        }
    }
    else if(((wavelength>=320)&&(wavelength<400))||((wavelength>1400)&&(wavelength<=4500)))
        {
        computeSpotDiameter();
        opticalGain=transmissionCoeff*magnification;
        opticalGainFormula="τ M<sup>2</sup>";
        }
    else
        {
        computeSpotDiameter();
        opticalGain=1;
        opticalGainFormula="1";
        }
}

double Binocular::get_Db()
{
    return Db;
}


QString Binocular::getOpticalGainFormula()const
{
    return opticalGainFormula;
}

void Binocular::computeSpotDiameter()
{
    Db=beamDiameter+binocularDistance*divergence;
}

QString Binocular::getBinocularEffects()
{
QString effectsString;
if(isDangerous())
    effectsString="PERICOLOSO";
            else
            effectsString= "/";

return effectsString;
}

void Binocular::setInZone(bool _inZone)
{
    inZone=_inZone;
}
