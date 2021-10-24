#include <QtGui>

#include "reflectorlink.h"
#include "laserpoint.h"
#include "reflector.h"
#include <cmath>

const double ReflectorLink::degRad = 180/3.1415926535897932384626433832795;

ReflectorLink::ReflectorLink(LaserPoint *fromLaser, Reflector *toReflector): fromMyLaser(fromLaser), toMyReflector(toReflector)
{
    fromMyLaser->addReflectorLink(this);
    toMyReflector->addReflectorLink(this);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);

    setColor(Qt::blue);
    trackNodes();

    QPen pen(color());
    pen.setStyle(Qt::DotLine);
    pen.setCosmetic(true);
    setPen(pen);

    setFlag(ItemIsSelectable, false);

    //hide();
}

ReflectorLink::~ReflectorLink()
{
    fromMyLaser->removeReflectorLink(this);
    toMyReflector->removeReflectorLink();
}

int ReflectorLink::type() const
{
    return Type;
}

LaserPoint *ReflectorLink::fromLaser() const
{
    return fromMyLaser;
}

void ReflectorLink::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor ReflectorLink::color() const
{
    return pen().color();
}

void ReflectorLink::trackNodes()
{
    setLine(QLineF(fromMyLaser->pos(), toMyReflector->pos()));
}

double ReflectorLink::reflectorLinkLenght()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyReflector->pos());
    lenght=myLine.length();
    return lenght;
}

double ReflectorLink::reflectorLinkPhase()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyReflector->pos());

    if(lenght!=0)
        phase=degRad*std::asin(myLine.dy()/myLine.length());
    else
        phase=0;

    return phase;
}

QLineF ReflectorLink::getTrack()
{
    QLineF myTrack=QLineF(fromMyLaser->pos(), toMyReflector->pos());
    return myTrack;
}
