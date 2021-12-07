#include "graphicsscene.h"
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

    if(mouseEvent->button() == Qt::LeftButton)
    {
        if(movingItem != nullptr)
        {
            Reflector *reflector= qgraphicsitem_cast<Reflector*>(movingItem);
            LaserPoint *laserpoint= qgraphicsitem_cast<LaserPoint*>(movingItem);
            Binocular *binocular= qgraphicsitem_cast<Binocular*>(movingItem);
            BeamInspector* beamInspector= qgraphicsitem_cast<BeamInspector*>(movingItem);
            FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(movingItem);
            LabRoom *myLabRoom= qgraphicsitem_cast<LabRoom*>(movingItem);

            if(laserpoint)
            {
                qDebug()<<"Ho selezionato il punto laser: ";
                clearSelection();
                laserpoint->setSelected(true);
                oldPos=laserpoint->pos();              
                emit graphicItemSelected(movingItem);
            }

            else if(reflector)
            {
                qDebug()<<"Ho selezionato un riflettore: ";
                clearSelection();
                reflector->setSelected(true);
                oldPos=reflector->pos();
                emit graphicItemSelected(movingItem);
            }

            else if(binocular)
            {
                qDebug()<<"Ho selezionato un dispositivo ottico: ";
                clearSelection();
                binocular->setSelected(true);
                oldPos=binocular->pos();
                emit graphicItemSelected(movingItem);
            }

            else if(beamInspector)
            {
                qDebug()<<"Ho selezionato un segnaposto di ispezione: ";
                clearSelection();
                beamInspector->setSelected(true);
                oldPos=beamInspector->pos();              
                emit graphicItemSelected(movingItem);
            }

            else if(footprint)
            {
                qDebug()<<"Ho selezionato un ingombro: ";
                clearSelection();
                oldPos=footprint->pos();
                myGraphicRect=footprint->getRectangle();
                oldRectangle=myGraphicRect.rect();
                footprint->setSelected(true);
                emit graphicItemSelected(movingItem);
            }

            else if(myLabRoom)
            {
                qDebug()<<"Ho selezionato il laboratorio: ";
                clearSelection();
                myLabRoom->setSelected(true);
                emit labroomSelected();
            }
        }
        else
            emit deselected();
    }
    else
    {
        mouseEvent->accept();
        return;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button()==Qt::LeftButton)
    {
        Reflector *reflector= qgraphicsitem_cast<Reflector*>(movingItem);
        LaserPoint *laserpoint= qgraphicsitem_cast<LaserPoint*>(movingItem);
        Binocular *binocular= qgraphicsitem_cast<Binocular*>(movingItem);
        BeamInspector *beamInspector= qgraphicsitem_cast<BeamInspector*>(movingItem);
        FootprintObject *footprint= qgraphicsitem_cast<FootprintObject*>(movingItem);

        if(footprint)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Footprint spostato: ";
            }
            if(footprint->getResizeHandlePressed())
                emit footprintRelease();
        }
        else if(laserpoint)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Laser spostato: ";
            }
        }
        else if(reflector)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Riflettore spostato: ";
            }
        }
        else if(binocular)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Strumento ottico spostato: ";
            }
        }
        else if(beamInspector)
        {
            if(oldPos!= movingItem->pos())
            {
                emit graphicItemMoved(movingItem, oldPos);
                qDebug()<<"Segnaposto di ispezione spostato: ";
            }
        }
        movingItem=nullptr;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
