#include "lasermovecommand.h"
#include "../laserpoint.h"

#include <QGraphicsScene>

LaserMoveCommand::LaserMoveCommand(LaserPoint *laserpoint, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent), myLaserPoint(laserpoint)
    , myOldPos(oldPos), newPos(laserpoint->pos())
{
}

bool LaserMoveCommand::mergeWith(const QUndoCommand *command)
{
    const LaserMoveCommand *moveCommand = static_cast<const LaserMoveCommand *>(command);
    LaserPoint *item = moveCommand->myLaserPoint;

    if (myLaserPoint != item)
        return false;

    newPos = item->pos();
    setText(QObject::tr("Sposta %1")
        .arg(createCommandString(newPos)));

    return true;
}

void LaserMoveCommand::undo()
{
    myLaserPoint->setPos(myOldPos);
    myLaserPoint->scene()->update();
    setText(QObject::tr("Sposta %1")
        .arg(createCommandString(newPos)));
}

void LaserMoveCommand::redo()
{
    myLaserPoint->setPos(newPos);
    setText(QObject::tr("Sposta %1")
        .arg(createCommandString(newPos)));
}

QString createCommandString(const QPointF &pos)
{
    return QObject::tr("%1 in (%2, %3)")
        .arg("Laser" )
        .arg(pos.x()).arg(pos.y());
}
