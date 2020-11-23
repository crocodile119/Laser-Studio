#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QRubberBand>
#include <QGraphicsScene>
#include <QGraphicsItem>


class Scene : public QGraphicsScene
{
            Q_OBJECT
    public:
        explicit Scene( QObject *parent = 0);
        ~Scene();

    signals:
        void selected();

    protected:

        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    private slots:

    private:
    QVector <QPointF> myPoint;
};

#endif // SCENE_H
