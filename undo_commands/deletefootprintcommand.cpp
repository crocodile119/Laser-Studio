#include "deletefootprintcommand.h"

DeleteFootprintCommand::DeleteFootprintCommand(FootprintObject *_footprintOnScene, ObjectLink *_objectLink, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                        QList <pair<FootprintObject *, int>>*_myFootprints, QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), footprintOnScene(_footprintOnScene), objectLink(_objectLink), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myFootprints(_myFootprints), deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteFootprintCommandString(deletePosition)));

}

DeleteFootprintCommand::~DeleteFootprintCommand()
{

}

void DeleteFootprintCommand::undo()
{
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    footprintOnScene->setLaserBeamPath(laserpoint->mapToItem(footprintOnScene, laserpoint->shapePath()));
    laserWindow->graphicsView->scene->addItem(objectLink);
    laserWindow->graphicsView->scene->addItem(footprintOnScene);
    footprintOnScene->setPos(deletePosition);
    laserWindow->graphicsView->scene->clearSelection();
    laserWindow->graphicsView->scene->update();
    //imposto la NOHD del punto laser
    footprintOnScene->setSelected(true);

    footprintOnScene->update();    
}

void DeleteFootprintCommand::redo()
{
    laserWindow->graphicsView->scene->removeItem(objectLink);
    laserWindow->graphicsView->scene->removeItem(footprintOnScene);

    myFootprints->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    int index;

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        FootprintObject *undoFootprints = dynamic_cast<FootprintObject*>(k.next());
        if (undoFootprints)
        {
            index=undoFootprints->getFootprintSeqNumber();
            myFootprints->push_back(make_pair(undoFootprints, index));
        }
    }
}

QString createDeleteFootprintCommandString(const QPointF &pos)
{
    QString footprintString="ingombro";

    return QObject::tr("%1 in (%2, %3)")
        .arg(footprintString)
        .arg(pos.x()).arg(pos.y());
}