#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

class LaserPoint;
class Reflector;

class Link : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 6 };
    Link(LaserPoint *fromNode, Reflector *toNode);
    ~Link();

    const static double degRad;

    int type() const override;
    LaserPoint *fromLaser() const;
    Reflector *toReflector() const;

    void setColor(const QColor &color);
    QColor color() const;
    QLineF getTrack();
    void trackNodes();
    double linkLenght();
    double LinkPhase();

private:
    LaserPoint *fromMyLaser;
    Reflector *toMyReflector;
    double lenght;
    double phase;
};

#endif
