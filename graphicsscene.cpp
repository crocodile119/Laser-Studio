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


GraphicsScene::GraphicsScene(QObject *parent):QGraphicsScene(parent)
{

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF mousePos(mouseEvent->buttonDownScenePos(Qt::LeftButton).x(),
                     mouseEvent->buttonDownScenePos(Qt::LeftButton).y());

    const QList<QGraphicsItem*>itemList=items(mousePos);
    movingItem=itemList.isEmpty() ? nullptr : itemList.first();

    if(movingItem != nullptr && mouseEvent->button() == Qt::LeftButton)
    {
        oldPos=movingItem->pos();
        Reflector *reflector= qgraphicsitem_cast<Reflector*>(movingItem);
        LaserPoint *laserpoint= qgraphicsitem_cast<LaserPoint*>(movingItem);
        Binocular *binocular= qgraphicsitem_cast<Binocular*>(movingItem);
        FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(movingItem);
        LabRoom *myLabRoom= qgraphicsitem_cast<LabRoom*>(movingItem);

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
            oldPos=reflector->pos();
            emit reflectorSelected();
        }

        else if(binocular)
        {
            qDebug()<<"Ho selezionato un dispositivo ottico: ";
            clearSelection();
            binocular->setSelected(true);
            oldPos=binocular->pos();
            emit binocularSelected();
        }

        else if(footprint)
        {
            qDebug()<<"Ho selezionato il punto laser: ";
            clearSelection();
            footprint->setSelected(true);
            oldPos=footprint->pos();
        }

        else if(myLabRoom)
        {
            qDebug()<<"Ho selezionato il laboratorio: ";
            clearSelection();
            myLabRoom->setSelected(true);
            oldPos=myLabRoom->pos();
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
        FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(movingItem);
        if(footprint)
        {
            if(footprint->getResizeHandlePressed())
            emit footprintRelease();

            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Footprint spostato: ";
            }
        }

        LaserPoint *laserpoint=qgraphicsitem_cast<LaserPoint*>(movingItem);
        if(laserpoint)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Laser spostato: ";
            }
        }

        Reflector *reflector=qgraphicsitem_cast<Reflector*>(movingItem);
        if(reflector)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Riflettore spostato: ";
            }
        }

        Binocular *binocular=qgraphicsitem_cast<Binocular*>(movingItem);
        if(binocular)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Strumento ottico spostato: ";
            }
        }

        LabRoom *myLabRoom=qgraphicsitem_cast<LabRoom*>(movingItem);
        if(myLabRoom)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Laboratorio spostato: ";
            }
        }
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}
