#ifndef INSPECTORLINK_H
#define INSPECTORLINK_H

#include <QGraphicsLineItem>
#include "laserpoint.h"
#include "beaminspector.h"

class BeamInspector;
class LaserPoint;

class InspectorLink : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 10 };
    InspectorLink(LaserPoint *fromNode, BeamInspector *toNode);
    ~InspectorLink();

    int type() const override;
    LaserPoint *fromLaser() const;
    BeamInspector *toBeamInspector() const;

    void setColor(const QColor &color);
    QColor color() const;

    void trackNodes();
    double linkInspectorLenght();
    double linkInspectorPhase();

private:
    LaserPoint *fromMyLaser;
    BeamInspector *toMyBeamInspector;
    double lenght;
    double phase;
};

#endif
