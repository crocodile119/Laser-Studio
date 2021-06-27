#include <QtGui>

#include "objectlink.h"
#include "laserpoint.h"
#include "footprintobject.h"
#include <cmath>

ObjectLink::ObjectLink(LaserPoint *fromLaser, FootprintObject *toObject): fromMyLaser(fromLaser), toMyObject(toObject)
{
    fromMyLaser->addObjectLink(this);
    toMyObject->addObjectLink(this);

    setZValue(-1);

    setColor(Qt::darkYellow);

    trackNodes();   

    setFlags(QGraphicsItem::ItemIsSelectable);
    hide();
}

ObjectLink::~ObjectLink()
{
    fromMyLaser->removeObjectLink(this);
    toMyObject->removeObjectLink();
}

int ObjectLink::type() const
{
    return Type;
}

LaserPoint *ObjectLink::fromLaser() const
{
    return fromMyLaser;
}

FootprintObject *ObjectLink::toObject() const
{
    return toMyObject;
}

void ObjectLink::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor ObjectLink::color() const
{
    return pen().color();
}

void ObjectLink::trackNodes()
{
    setLine(QLineF(fromMyLaser->pos(), toMyObject->pos()));
}

double ObjectLink::objectLinkLenght()
{   
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyObject->pos());
    lenght=myLine.length();
    return lenght;
}

double ObjectLink::objectLinkPhase()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyObject->pos());

    if(lenght!=0)
        phase=asin(myLine.dy()/myLine.length());
    else
        phase=0;

    return phase;
}

double ObjectLink::objectLinkPhase_x()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyObject->pos());
    return myLine.dx();
}
