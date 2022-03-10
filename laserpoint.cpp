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
#include "reflectorlink.h"
#include "laserpoint.h"
#include "objectlink.h"
#include "inspectorlink.h"

const double LaserPoint::radDeg = 3.1415926535897932384626433832795/180;

LaserPoint::LaserPoint():QGraphicsObject(), laserPix(":/images/laserpix.png"), prohibitedPix(":/images/prohibitedpix.png")
{
    myTextColor = Qt::black;
    myOutlineColor = Qt::transparent;
    myBeamColor = Qt::darkGray;
    myBackgroundColor = Qt::white;
    opticalDiameter=2*144.0;
    skinDiameter=2*6.1;
    aperture=60;
    installationIndex=0;
    pillow= 0.11465;
    transmittance=1.0;
    scale=1.0;
    filterOn=false;
    feasibleDutyCycle=true;
    setStringPosition();
    setTextLabel();
    setToolTip(position);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

LaserPoint::~LaserPoint()
{
    foreach (ReflectorLink *reflectorlink, myReflectorLinks)
        delete reflectorlink;

    foreach (BinocularLink *binocularlink, myBinocularLinks)
        delete binocularlink;

    foreach (ObjectLink *objectlink, myObjectLinks)
        delete objectlink;

    foreach (InspectorLink *inspectorLink, myInspectorLinks)
        delete inspectorLink;
}

int LaserPoint::type() const
{
    return Type;
}

QString LaserPoint::text() const
{
    return myText;
}

void LaserPoint::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor LaserPoint::textColor() const
{
    return myTextColor;
}

void LaserPoint::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor LaserPoint::outlineColor() const
{
    return myOutlineColor;
}

void LaserPoint::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor LaserPoint::backgroundColor() const
{
    return myBackgroundColor;
}

void LaserPoint::addReflectorLink(ReflectorLink *reflectorlink)
{
    myReflectorLinks.insert(reflectorlink);
}

void LaserPoint::addBinocularLink(BinocularLink *binocularlink)
{
    myBinocularLinks.insert(binocularlink);
}

void LaserPoint::addObjectLink(ObjectLink *objectlink)
{
    myObjectLinks.insert(objectlink);
}

void LaserPoint::addInspectorLink(InspectorLink *inspectorLink)
{
    myInspectorLinks.insert(inspectorLink);
}

void LaserPoint::removeReflectorLink(ReflectorLink *reflectorlink)
{
    myReflectorLinks.remove(reflectorlink);
}

void LaserPoint::removeBinocularLink(BinocularLink *binocularlink)
{
    myBinocularLinks.remove(binocularlink);
}

void LaserPoint::removeObjectLink(ObjectLink *objectLink)
{
    myObjectLinks.remove(objectLink);
}


void LaserPoint::removeInspectorLink(InspectorLink *inspectorLink)
{
    myInspectorLinks.remove(inspectorLink);
}

QRectF LaserPoint::boundingRect() const
{
    const int Margin = 1;
    QRectF rect;

    QRectF myRect=unitedBounding();

    if(opticalDiameter>=skinDiameter)
    {
        if(opticalDiameter>=myRect.width())
            rect=selectionOpticalRect();
        else if(opticalDiameter<myRect.width())
            rect=myRect;
    }
    else
    {
        if(skinDiameter>=myRect.width())
            rect=selectionSkinRect();
        else if(skinDiameter<myRect.height())
            rect=myRect;
    }

    if(myRect.width()>enhacedPathBoudingRect.width())
        return rect.adjusted(-Margin, -Margin, +Margin, +Margin);
    else
        return enhacedPathBoudingRect.adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath LaserPoint::shape() const
{
    QPainterPath path;
    path.addRect(outlineRect());
    return path;
}

void LaserPoint::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        pen.setColor(Qt::darkGray);
    }
    pen.setCosmetic(true);

    QPen beamPen(myBeamColor);
    beamPen.setStyle(Qt::DashLine);

    if (option->state & QStyle::State_Selected) {
        beamPen.setStyle(Qt::DotLine);
        beamPen.setWidth(2);
        beamPen.setColor(Qt::darkGray);
    }

    beamPen.setCosmetic(true);

    painter->setPen(beamPen);
    painter->setBrush(myBackgroundColor);
    double firstPie=(360-aperture/2-pillow);
    double secondPie=(aperture+pillow);

    if(opticalDiameter>skinDiameter)
    {
        QPainterPath myOpticalPie;
        QPointF laserpointOrigin=QPointF(0.0,0.0);

        myOpticalPie.moveTo(laserpointOrigin);

        myOpticalPie.arcTo(selectionOpticalRect(), firstPie, secondPie);
        myOpticalPie.lineTo(laserpointOrigin);
        QPainterPath room;

        QPainterPath mySkinPie;
        mySkinPie.moveTo(laserpointOrigin);
        mySkinPie.arcTo(selectionSkinRect(), firstPie, secondPie);
        mySkinPie.lineTo(laserpointOrigin);

        if(roomLimits!=QRectF())
        {
            room.addRect(roomLimits);
            myOpticalPie=room.intersected(myOpticalPie);
            mySkinPie=room.intersected(mySkinPie);
            if(roomLimits.contains(QPointF(0,0)))
            {
                painter->setBrush(QColor(245,250,250, 127));
                painter->drawPath(myOpticalPie);
                painter->setBrush(QColor(240,240,240, 127));
                painter->drawPath(mySkinPie);
            }
        }
        else
        {
            painter->setBrush(QColor(245,250,250, 127));
            painter->drawPath(myOpticalPie);
            painter->setBrush(QColor(240,240,240, 127));
            painter->drawPath(mySkinPie);
        }
    }
    else
    {
        painter->setBrush(QColor(240,240,240, 127));
        painter->drawPie(selectionSkinRect(), firstPie, secondPie);
        painter->setBrush(QColor(245,250,250, 127));
        painter->drawPie(selectionSkinRect(), firstPie, secondPie);
    }


    painter->setPen(Qt::transparent);
    painter->setBrush(QColor(80, 255, 80, 200));

    painter->drawPath(laserShadowZone);
    painter->drawPath(enhacedShadowZone);
    //painter->drawPath(enhacedPath);

    prepareGeometryChange();

    QRectF pixRect=outlineRect();
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(pixRect);

    QRectF source(0.0, 0.0, laserPix.width(), laserPix.height());

    if(qualityFactor>=1 && feasibleDutyCycle)
        painter->drawPixmap(pixRect, laserPix, source);
    else
        painter->drawPixmap(pixRect, prohibitedPix, source);

    QTransform transform=painter->transform();

    double m13=transform.m13();
    double m23=transform.m23();
    double m32=transform.m32();
    double m31=transform.m31();
    double m21=transform.m21();
    double m12=transform.m12();

    painter->save();
    painter->resetTransform();
    QTransform myTransform=QTransform(1.0, m12, m13, m21, 1.0, m23, m31, m32, 1.0);

    painter->setTransform(myTransform);
    QRectF boundingRect=labelRect();
    setTextLabel();

    QPen textPen(myTextColor);
    painter->setPen(textPen);
    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);

    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);
    setStringPosition();
    update();
}

QVariant LaserPoint::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        foreach (ReflectorLink *reflectorlink, myReflectorLinks)
            reflectorlink->trackNodes();

        setStrings();
        return pos();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF LaserPoint::outlineRect() const
{
    double rectWidht=laserPix.width()/scale;
    double rectHeight=laserPix.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF center=rect.center();
    rect.translate(-center);
    return rect;
}

QRectF LaserPoint::selectionOpticalRect() const
{
    QSizeF rectSize(opticalDiameter, opticalDiameter);
    QPointF center(outlineRect().center());
    QRectF rect(center, rectSize);
    rect.translate(-rect.center());
    return rect;
}

QRectF LaserPoint::selectionSkinRect() const
{
    QSizeF rectSize(skinDiameter, skinDiameter);
    QPointF center(outlineRect().center());
    QRectF rect(center, rectSize);
    rect.translate(-rect.center());
    return rect;
}

//non più usata
int LaserPoint::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}

void LaserPoint::setOpticalDiameter(double newRadius)
{
    if(2*newRadius==opticalDiameter)
        return;

    prepareGeometryChange();
    opticalDiameter=2*newRadius;
}

void LaserPoint::setSkinDistance(double newRadius)
{
    if(2*newRadius==skinDiameter)
        return;

    prepareGeometryChange();
    skinDiameter=2*newRadius;
}

void LaserPoint::setStringPosition()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();

    QString xString;
    QString yString;
    QString opticalDistanceString;
    QString skinDistanceString;
    QString filterOnString;
    QString installationString;
    QString rayleighDistanceString;
    QString qualityFactorString;

    if(filterOn)
        filterOnString="<br>filtro montato su ottica con transmittanza τ="+ QString::number(getTransmittance());
    else
        filterOnString.clear();

    xString=QString::number(xCoordinate,'f', 0);
    yString=QString::number(yCoordinate,'f', 0);
    opticalDistanceString=QString::number(opticalDiameter/2,'f',2);
    skinDistanceString=QString::number(skinDiameter/2,'f',2);
    installationString=getLaserInstallation();
    rayleighDistanceString=QString::number(rayleighDistance, 'e', 2);
    qualityFactorString=QString::number(qualityFactor, 'e', 2);

    position ="Punto Laser" + QString(" (%1,%2) %3, DRO[m]: %4  <br>DRP[m]: %5 Postazione: %6 <br>z<sub>R</sub>[m]: %7 M<sup>2</sup>: %8" )
                                    .arg(xString)
                                    .arg(yString)
                                    .arg(filterOnString)
                                    .arg(opticalDistanceString)
                                    .arg(skinDistanceString)
                                    .arg(installationString)
                                    .arg(rayleighDistanceString)
                                    .arg(qualityFactorString);
}

void LaserPoint::setTextLabel()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();


    QString xString;
    QString yString;

    xString=QString::number(xCoordinate,'f', 0);
    yString=QString::number(yCoordinate,'f', 0);
    prepareGeometryChange();
    textLabel ="laser\n"+QString("(%1,%2)")
                                    .arg(xString)
                                    .arg(yString);
    update();
}

void LaserPoint::setRoomLimits(const QRectF &_roomLimits)
{
    roomLimits=_roomLimits;
}

QString LaserPoint::getStringPosition()const
{
   return position;
}

int LaserPoint::getInstallationIndex()const
{
    return installationIndex;
}

double LaserPoint::getAperture()const
{
    return aperture;
}

void LaserPoint::setInstallationIndex(const int& _installationIndex)
{
    installationIndex=_installationIndex;
}

void LaserPoint::setAperture(const double &_aperture)
{
    aperture=_aperture;
}

void LaserPoint::computePillowAreas()
{
    switch(installationIndex)
    {
       case 0 :
       pillow= 0.11465;
       break;

       case 1 :
       pillow= 0.286624;
       break;

       case 2 :
       pillow= 0.573248;
       break;

       case 3 :
       pillow= 1.714795;
       break;

       default:
       pillow= 1.714795;
       break;
    }
}

double LaserPoint::getOpticalDiameter()const
{
    return opticalDiameter;
}

double LaserPoint::getPillow()const
{
    return pillow;
}

double LaserPoint::getSkinDistance()const
{
    return skinDiameter;
}

QString LaserPoint::getLaserInstallation()
{
    QString installationIndexString;

    switch(installationIndex)
    {
        case(0):
        installationIndexString="Stabile";
        break;

        case(1):
        installationIndexString="Poco stabile";
        break;

        case(2):
        installationIndexString="Mobile";
        break;

        case(3):
        installationIndexString="Assenza di piattaforma";
        break;

        default:
        installationIndexString="Assenza di piattaforma";
        break;
        }
    return installationIndexString;
}

QString LaserPoint::getInstallationforIndex(const int &installationIndex)
{
    QString installationIndexString;

    switch(installationIndex)
    {
        case(0):
        installationIndexString="Stabile";
        break;

        case(1):
        installationIndexString="Poco stabile";
        break;

        case(2):
        installationIndexString="Mobile";
        break;

        case(3):
        installationIndexString="Assenza di piattaforma";
        break;

        default:
        installationIndexString="Assenza di piattaforma";
        break;
        }
    return installationIndexString;
}

void LaserPoint::setDutyCycleCheck(bool _feasibleDutyCycle)
{
    feasibleDutyCycle=_feasibleDutyCycle;
}

QPainterPath LaserPoint::shapePath()
{
    QPainterPath myPie;
    QPointF laserpointOrigin=QPointF(0.0,0.0);
    myPie.moveTo(laserpointOrigin);
    QRectF myBoudingRect=QRectF(0.0, 0.0, opticalDiameter, opticalDiameter);
    myBoudingRect.moveCenter(laserpointOrigin);
    double firstPie=(360-aperture/2-pillow);
    double secondPie=(aperture+pillow);

    myPie.arcTo(myBoudingRect, firstPie, secondPie);
    myPie.moveTo(laserpointOrigin);

    laserPath=myPie;
    return laserPath;
}

QPainterPath LaserPoint::shapeEnhacedPath(const double& _EnhacedOptDistance)
{    
    QPainterPath myPie;
    QPointF laserpointOrigin=QPointF(0.0,0.0);
    myPie.moveTo(laserpointOrigin);
    enhacedPathBoudingRect=QRectF(0.0, 0.0, _EnhacedOptDistance, _EnhacedOptDistance);
    enhacedPathBoudingRect.moveCenter(laserpointOrigin);
    double firstPie=(360-aperture/2-pillow);
    double secondPie=(aperture+pillow);

    myPie.arcTo(enhacedPathBoudingRect, firstPie, secondPie);
    myPie.moveTo(laserpointOrigin);

    enhacedPath=myPie;
    return enhacedPath;
}

bool LaserPoint::shapePathContainsPoint(const QPointF &point)
{
    bool containsItem=shapePath().contains(point)&&(!laserShadowZone.contains(point));
    return containsItem;
}

bool LaserPoint::shapeEnhacedPathContainsPoint(const QPointF &point, const double &_distance)
{
    bool containsItem=shapeEnhacedPath(_distance).contains(point)&&(!enhacedShadowZone.contains(point));
    return containsItem;
}

QRectF LaserPoint::labelRect()
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

QRectF LaserPoint::unitedBounding() const
{
    QRectF unitedRect=outlineRect();
    unitedRect.adjust(-0.1*outlineRect().width(), -0.1*outlineRect().height(),
                      1.2*outlineRect().width(), 1.2*outlineRect().height());
    return unitedRect;
}

void LaserPoint::setPixScale(const double &newScale)
{
    scale=newScale;
}

void LaserPoint::setStrings()
{
    setStringPosition();
    setToolTip(position);
    setTextLabel();
}

QPainterPath LaserPoint::getLaserShapePath()
{
    return laserPath;
}

QPainterPath LaserPoint::getShapeEnhacedPath()
{
    return enhacedPath;
}

void LaserPoint::setShadowZone(const QPainterPath& shadowZone)
{
    prepareGeometryChange();
    laserShadowZone=shadowZone;
}

void LaserPoint::setEhnacedShadowZone(const QPainterPath& shadowZone)
{
    prepareGeometryChange();
    enhacedShadowZone=shadowZone;
}

void LaserPoint::setTrasmittance(const double& _transmittance)
{
    transmittance=_transmittance;
}

double LaserPoint::getTransmittance()const
{
    return transmittance;
}

bool LaserPoint::isFilterOn()
{
    return filterOn;
}

void LaserPoint::setFilterOn(bool filter)
{
    filterOn=filter;
}

void LaserPoint::setUndoStack(QUndoStack *_undoStack)
{
    undoStack=_undoStack;
}

void LaserPoint::setRayleighDistance(const double& _rayleighDistance)
{
    rayleighDistance=_rayleighDistance;
}

double LaserPoint::getRayleighDistance()const
{
    return rayleighDistance;
}

void LaserPoint::setQualityFactor(const double& _qualityFactor)
{
    qualityFactor=_qualityFactor;
}

double LaserPoint::getQualityFactor()const
{
    return qualityFactor;
}
