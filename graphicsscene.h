#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QRubberBand>

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent=0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:

signals:
    void reflectorSelected();
    void laserSelected();
    void binocularSelected();
    void labroomSelected();
    void deselected();
    void footprintRelease();
};

#endif // GRAPHICSSCENE_H
