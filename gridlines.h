#ifndef GRIDLINES_H
#define GRIDLINES_H
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

class GridLines : public  QGraphicsItem
{

public:
    GridLines();

    public :
    void setSceneRect(const QRectF& _sceneRect);
    void setScale(const double&);
    void setGridLines(const double& _step);
    void setTopLeftViewport(const QPointF&);
    void setTextLabel(const QString&);
    void setTextColor(const QColor&);


private:
    QRectF labelRect()const;
    QRectF sceneRect;
    double step;
    double scale;
    //QRectF myTextRect;
    QString textLabel;
    QPointF zeroViewportPoint;
    QColor myTextColor;

    QRectF boundingRect() const;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRIDLINES_H
