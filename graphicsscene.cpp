#include "graphicsscene.h"
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "labroom.h"
#include "footprintobject.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QRect>
#include <QSize>


GraphicsScene::GraphicsScene(QObject *parent)
{
    Q_UNUSED(parent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button()==Qt::LeftButton)
    {
        Reflector *reflector= qgraphicsitem_cast<Reflector*>(itemAt(mouseEvent->scenePos(), QTransform()));
        LaserPoint *laserpoint= qgraphicsitem_cast<LaserPoint*>(itemAt(mouseEvent->scenePos(), QTransform()));
        Binocular *binocular= qgraphicsitem_cast<Binocular*>(itemAt(mouseEvent->scenePos(), QTransform()));
        LabRoom *myLabRoom= qgraphicsitem_cast<LabRoom*>(itemAt(mouseEvent->scenePos(), QTransform()));

        if(laserpoint)
        {
            qDebug()<<"Ho selezionato il punto laser: ";
            clearSelection();
            laserpoint->setSelected(true);
            emit laserSelected();
        }

        else if(reflector)
        {
            qDebug()<<"Ho selezionato un riflettore: ";
            clearSelection();
            reflector->setSelected(true);
            emit reflectorSelected();
        }

        else if(binocular)
        {
            qDebug()<<"Ho selezionato un dispositivo ottico: ";
            clearSelection();
            binocular->setSelected(true);
            emit binocularSelected();
        }

        else if(myLabRoom)
        {
            qDebug()<<"Ho selezionato il laboratorio: ";
            clearSelection();
            myLabRoom->setSelected(true);
            emit labroomSelected();
        }
        else
            emit deselected();
    }
        QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button()==Qt::LeftButton)
    {
        FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(itemAt(mouseEvent->scenePos(), QTransform()));
        if(footprint)
        {
            if(footprint->getResizeHandlePressed())
            emit footprintRelease();
        }

        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}
