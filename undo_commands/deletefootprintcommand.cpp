#include "deletefootprintcommand.h"

DeleteFootprintCommand::DeleteFootprintCommand(FootprintObject *_footprint, ObjectLink *_objectLink, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                        QList <FootprintObject *>*_myFootprints, QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), footprint(_footprint), objectLink(_objectLink), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myFootprints(_myFootprints), deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteFootprintCommandString(deletePosition)));

}

DeleteFootprintCommand::~DeleteFootprintCommand()
{
    footprintOnScene=new FootprintObject(scale);
    footprintOnScene->setPos(deletePosition);
}

void DeleteFootprintCommand::undo()
{
    objectLink=addObjectLink();
    laserWindow->graphicsView->scene->addItem(objectLink);
    laserWindow->graphicsView->scene->addItem(footprint);

    laserWindow->graphicsView->scene->clearSelection();
    laserWindow->graphicsView->scene->update();
}

void DeleteFootprintCommand::redo()
{
    laserWindow->graphicsView->scene->removeItem(objectLink);
    laserWindow->graphicsView->scene->removeItem(footprint);

    delete objectLink;
    delete footprint;
    footprint=new FootprintObject(scale);
    footprint->setPos(deletePosition);

//E' necessario cancellare la forma dell'ombra dell'ingombro
    QPainterPath nullShadowZonePath;
    laserpoint->setShadowZone(nullShadowZonePath);
    laserpoint->setEhnacedShadowZone(nullShadowZonePath);

    myFootprints->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        FootprintObject *undoFootprints = dynamic_cast<FootprintObject*>(k.next());
        if (undoFootprints)
            myFootprints->push_back(undoFootprints);
    }
}

ObjectLink *DeleteFootprintCommand::addObjectLink()
{
    ObjectNodePair objectNodes = selectedObjectNodePair();
    if (objectNodes == ObjectNodePair())
        return nullptr;

    ObjectLink *objectlink = new ObjectLink(objectNodes.first, objectNodes.second);

    return objectlink;
}

DeleteFootprintCommand::ObjectNodePair DeleteFootprintCommand::selectedObjectNodePair() const
{
    return ObjectNodePair(laserpoint, footprint);
}

QString createDeleteFootprintCommandString(const QPointF &pos)
{
    QString footprintString="ingombro";

    return QObject::tr("%1 in (%2, %3)")
        .arg(footprintString)
        .arg(pos.x()).arg(pos.y());
}
