#ifndef OBJECTLINK_H
#define OBJECTLINK_H

#include <QGraphicsLineItem>

class LaserPoint;
class FootprintObject;

class ObjectLink : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 8 };
    ObjectLink(LaserPoint *fromNode, FootprintObject *toObject);
    ~ObjectLink();

    int type() const override;
    LaserPoint *fromLaser() const;
    FootprintObject *toObject() const;
    double objectLinkPhase_x();

    void setColor(const QColor &color);
    QColor color() const;

    void trackNodes();
    double objectLinkLenght();
    double objectLinkPhase();

private:
    LaserPoint *fromMyLaser;
    FootprintObject *toMyObject;
    double lenght;
    double phase;
};

#endif
