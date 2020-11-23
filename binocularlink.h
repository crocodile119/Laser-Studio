#ifndef BINOCULARLINK_H
#define BINOCULARLINK_H

#include <QGraphicsLineItem>

class LaserPoint;
class Binocular;

class BinocularLink : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 7 };
    BinocularLink(LaserPoint *fromNode, Binocular *toNode);
    ~BinocularLink();

    int type() const override;
    LaserPoint *fromLaser() const;
    Binocular *toBinocular() const;

    void setColor(const QColor &color);
    QColor color() const;

    void trackNodes();
    double linkBinocularLenght();
    double linkBinocularPhase();

private:
    LaserPoint *fromMyLaser;
    Binocular *toMyBinocular;
    double lenght;
    double phase;
};

#endif
