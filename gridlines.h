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
    QRectF outlineRect() const;
    QRectF labelRect()const;
    QRectF sceneRect;
    double step;
    QPixmap referencePix;
    double scale;
    //QRectF myTextRect;
    QString textLabel;
    QPointF zeroViewportPoint;
    QColor myTextColor;
    QRectF boundingRect() const override;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
};

#endif // GRIDLINES_H
