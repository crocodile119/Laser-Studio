#ifndef REFLECTORLINK_H
#define REFLECTORLINK_H

#include <QGraphicsLineItem>

class LaserPoint;
class Reflector;

class ReflectorLink : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 6 };
    ReflectorLink(LaserPoint *fromNode, Reflector *toNode);
    ~ReflectorLink();

    const static double degRad;

    int type() const override;
    LaserPoint *fromLaser() const;
    Reflector *toReflector() const;

    void setColor(const QColor &color);
    QColor color() const;
    QLineF getTrack();
    void trackNodes();
    double reflectorLinkLenght();
    double reflectorLinkPhase();

private:
    LaserPoint *fromMyLaser;
    Reflector *toMyReflector;
    double lenght;
    double phase;
};

#endif
