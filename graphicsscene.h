#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QRubberBand>
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "beaminspector.h"
#include "labroom.h"
#include "footprintobject.h"
#include "safetysignitem.h"

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
    QRectF oldRectangle;
    class Rectangle myGraphicRect;
    QPointF releaseScenePosition;

signals:
    void labroomSelected();
    void deselected();
    void footprintRelease();
    void mouseReleasePos(const QPointF&);
    void graphicItemSelected(QGraphicsItem *movingItem);
    void graphicItemMoved(QGraphicsItem *movingItem, const QPointF& oldPos);
};

#endif // GRAPHICSSCENE_H
