#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QRubberBand>
#include "reflector.h"

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent=nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QPointF oldPos;
    QGraphicsItem *movingItem=nullptr;

signals:
    void reflectorSelected();
    void laserSelected();
    void binocularSelected();
    void labroomSelected();
    void deselected();
    void footprintRelease();
    void graphicItemMoved(QGraphicsItem *movingItem, const QPointF& oldPos);
};

#endif // GRAPHICSSCENE_H
