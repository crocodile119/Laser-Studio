#include <QtGui>

#include "binocularlink.h"
#include "laserpoint.h"
#include "binocular.h"
#include <cmath>

BinocularLink::BinocularLink(LaserPoint *fromLaser, Binocular *toBinocular)
{
    fromMyLaser = fromLaser;
    toMyBinocular = toBinocular;

    fromMyLaser->addBinocularLink(this);
    toMyBinocular->addBinocularLink(this);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);

    setColor(Qt::red);

    trackNodes();
    QPen pen(color());
    pen.setStyle(Qt::DotLine);
    pen.setCosmetic(true);
    setPen(pen);

    setFlag(ItemIsSelectable, false);
}

BinocularLink::~BinocularLink()
{
    fromMyLaser->removeBinocularLink(this);
    toMyBinocular->removeBinocularLink(this);
}

int BinocularLink::type() const
{
    return Type;
}

LaserPoint *BinocularLink::fromLaser() const
{
    return fromMyLaser;
}

void BinocularLink::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor BinocularLink::color() const
{
    return pen().color();
}

void BinocularLink::trackNodes()
{
    setLine(QLineF(fromMyLaser->pos(), toMyBinocular->pos()));
}

double BinocularLink::linkBinocularLenght()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyBinocular->pos());
    lenght=myLine.length();
    return lenght;
}

double BinocularLink::linkBinocularPhase()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyBinocular->pos());
    phase=asin(myLine.dy()/myLine.length());
    return phase;
}
