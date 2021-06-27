#include "movecommand.h"
#include <QGraphicsScene>

MoveCommand::MoveCommand(QGraphicsItem *_movedItem, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent), myMovedItem(_movedItem)
    , myOldPos(oldPos), newPos(myMovedItem->pos())
{
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QGraphicsItem *item = moveCommand->myMovedItem;

    if (myMovedItem != item)
        return false;

    newPos = item->pos();
    setText(QObject::tr("Sposto %1")
        .arg(createCommandString(myMovedItem, newPos)));

    return true;
}

void MoveCommand::undo()
{
    myMovedItem->setPos(myOldPos);
    myMovedItem->scene()->update();
    setText(QObject::tr("Sposto %1")
        .arg(createCommandString(myMovedItem, newPos)));

}

void MoveCommand::redo()
{
    myMovedItem->setPos(newPos);
    setText(QObject::tr("Sposto %1")
        .arg(createCommandString(myMovedItem, newPos)));
}

QString createCommandString(QGraphicsItem *movingItem, const QPointF &pos)
{
    Reflector *reflector=qgraphicsitem_cast<Reflector*>(movingItem);
    Binocular *binocular=qgraphicsitem_cast<Binocular*>(movingItem);
    LaserPoint *laserpoint=qgraphicsitem_cast<LaserPoint*>(movingItem);
    FootprintObject *footprint=qgraphicsitem_cast<FootprintObject*>(movingItem);

    QString targetString;
    QString undoViewString;

    if(reflector)
    {   
        target myTarget=reflector->getReflectorKind();

        if(myTarget==WET_TARGET)
            targetString="lamiera baagnata";
        else if(myTarget==LAMBERTIAN_TARGET)
            targetString="superficie lambertiana";
        else if(myTarget==MIRROR_TARGET)
            targetString="superficie speculare";
        else if(myTarget==GLASS_TARGET)
            targetString="superficie di vetro";

       undoViewString=QObject::tr("%1 in (%2, %3)")
            .arg(targetString)
            .arg(pos.x()).arg(pos.y());
    }
    else if(binocular)
    {
        QString binocularString;
        binocularString="strumento ottico";

        undoViewString=QObject::tr("%1 in (%2, %3)")
            .arg(binocularString)
            .arg(pos.x()).arg(pos.y());
    }
    else if(laserpoint)
    {
        QString laserString;
        laserString="laser";

        undoViewString=QObject::tr("%1 in (%2, %3)")
            .arg(laserString)
            .arg(pos.x()).arg(pos.y());
    }
    else if(footprint)
    {
        QString footprintString;
        footprintString="ingombro";

        undoViewString=QObject::tr("%1 in (%2, %3)")
            .arg(footprintString)
            .arg(pos.x()).arg(pos.y());
    }
    return undoViewString;
}
