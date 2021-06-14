#include <QtGui>

#include "link.h"
#include "laserpoint.h"
#include "reflector.h"
#include <cmath>

const double Link::degRad = 180/3.1415926535897932384626433832795;

Link::Link(LaserPoint *fromLaser, Reflector *toReflector): fromMyLaser(fromLaser), toMyReflector(toReflector)
{
    fromMyLaser->addLink(this);
    toMyReflector->addLink(this);

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

Link::~Link()
{
    fromMyLaser->removeLink(this);
    toMyReflector->removeLink(this);
}

int Link::type() const
{
    return Type;
}

LaserPoint *Link::fromLaser() const
{
    return fromMyLaser;
}

void Link::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor Link::color() const
{
    return pen().color();
}

void Link::trackNodes()
{
    setLine(QLineF(fromMyLaser->pos(), toMyReflector->pos()));
}

double Link::linkLenght()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyReflector->pos());
    lenght=myLine.length();
    return lenght;
}

double Link::LinkPhase()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyReflector->pos());

    if(lenght!=0)
        phase=degRad*std::asin(myLine.dy()/myLine.length());
    else
        phase=0;

    return phase;
}

QLineF Link::getTrack()
{
    QLineF myTrack=QLineF(fromMyLaser->pos(), toMyReflector->pos());
    return myTrack;
}
