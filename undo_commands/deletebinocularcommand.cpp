#include "deletebinocularcommand.h"

DeleteBinocularCommand::DeleteBinocularCommand(Binocular *_binocularOnScene, BinocularLink *_binocularLink, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                             QList <Binocular *>*_myBinoculars, QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), binocularOnScene(_binocularOnScene), binocularLink(_binocularLink), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myBinoculars(_myBinoculars), deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteBinocularCommandString(deletePosition)));

}

DeleteBinocularCommand::~DeleteBinocularCommand()
{

}

void DeleteBinocularCommand::undo()
{
    laserWindow->graphicsView->scene->addItem(binocularLink);
    laserWindow->graphicsView->scene->addItem(binocularOnScene);
    binocularOnScene->setPos(deletePosition);
    laserWindow->graphicsView->scene->clearSelection();
    laserWindow->graphicsView->scene->update();

    myBinoculars->append(binocularOnScene);

    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    binocularOnScene->setSelected(true);

    binocularOnScene->update();
}

void DeleteBinocularCommand::redo()
{
    laserWindow->graphicsView->scene->removeItem(binocularLink);
    laserWindow->graphicsView->scene->removeItem(binocularOnScene);

    myBinoculars->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        Binocular *undoBinoculars = dynamic_cast<Binocular*>(k.next());
        if (undoBinoculars)
        {
            myBinoculars->push_back(undoBinoculars);
        }
    }
}

QString createDeleteBinocularCommandString(const QPointF &pos)
{
    QString binocularString;
    binocularString="Strumento ottico";

    return QObject::tr("%1 in (%2, %3)")
        .arg(binocularString)
        .arg(pos.x()).arg(pos.y());
}
