#include "deletereflectorcommand.h"

DeleteReflectorCommand::DeleteReflectorCommand(Reflector *_reflectorOnScene, Link *_link, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                             QList<Reflector *> *_myReflectors, QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), reflectorOnScene(_reflectorOnScene), link(_link), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myReflectors(_myReflectors), deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteCommandString(reflectorOnScene, deletePosition)));

}

DeleteReflectorCommand::~DeleteReflectorCommand()
{

}

void DeleteReflectorCommand::undo()
{
    laserWindow->graphicsView->scene->addItem(link);
    laserWindow->graphicsView->scene->addItem(reflectorOnScene);
    reflectorOnScene->setPos(deletePosition);
    laserWindow->graphicsView->scene->clearSelection();
    laserWindow->graphicsView->scene->update();

    myReflectors->append(reflectorOnScene);

    laserWindow->graphicsView->scene->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    reflectorOnScene->setSelected(true);

    reflectorOnScene->update();
}

void DeleteReflectorCommand::redo()
{
    laserWindow->graphicsView->scene->removeItem(link);
    laserWindow->graphicsView->scene->removeItem(reflectorOnScene);

    myReflectors->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        Reflector *undoReflectors = dynamic_cast<Reflector *>(k.next());
        if (undoReflectors)
        {
            myReflectors->push_back(undoReflectors);
        }
    }
}

QString createDeleteCommandString(Reflector *item, const QPointF &pos)
{
    QString targetString;
    target myTarget=item->getReflectorKind();
    if(myTarget==WET_TARGET)
        targetString="lamiera bagnata";
    else if(myTarget==LAMBERTIAN_TARGET)
        targetString="superficie lambertiana";
    else if(myTarget==MIRROR_TARGET)
        targetString="superficie speculare";
    else if(myTarget==GLASS_TARGET)
        targetString="superficie di vetro";

    return QObject::tr("%1 in (%2, %3)")
        .arg(targetString)
        .arg(pos.x()).arg(pos.y());
}
