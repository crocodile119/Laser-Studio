#include <QtGui>

#include "inspectorlink.h"
#include <cmath>

InspectorLink::InspectorLink(LaserPoint *fromLaser, BeamInspector *toBeamInspector): fromMyLaser(fromLaser),
                toMyBeamInspector(toBeamInspector)
{
    fromMyLaser->addInspectorLink(this);
    toMyBeamInspector->addInspectorLink(this);

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

InspectorLink::~InspectorLink()
{
    fromMyLaser->removeInspectorLink(this);
    toMyBeamInspector->removeInspectorLink();
}

int InspectorLink::type() const
{
    return Type;
}

LaserPoint *InspectorLink::fromLaser() const
{
    return fromMyLaser;
}

void InspectorLink::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor InspectorLink::color() const
{
    return pen().color();
}

void InspectorLink::trackNodes()
{
    setLine(QLineF(fromMyLaser->pos(), toMyBeamInspector->pos()));
}

double InspectorLink::linkInspectorLenght()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyBeamInspector->pos());
    lenght=myLine.length();
    return lenght;
}

double InspectorLink::linkInspectorPhase()
{
    QLineF myLine=QLineF(fromMyLaser->pos(), toMyBeamInspector->pos());
    phase=asin(myLine.dy()/myLine.length());
    return phase;
}
