#include "deletebinocularcommand.h"

DeleteBinocularCommand::DeleteBinocularCommand(Binocular *_binocularOnScene, BinocularLink *_binocularLink, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                             QList <pair<Binocular *, int>>*_myBinoculars, BinocularsListModel *_binocularsModel,
                             QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), binocularOnScene(_binocularOnScene), binocularLink(_binocularLink), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myBinoculars(_myBinoculars), binocularsModel(_binocularsModel), deletePosition(_deletePosition)
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

    int modelIndex=binocularsModel->addElement(*binocularOnScene);
    myBinoculars->append(make_pair(binocularOnScene, modelIndex));
    binocularOnScene->setBinSeqNumber(modelIndex);

    binocularsModel->myDataHasChanged();

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

    int index;

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        Binocular *undoBinoculars = dynamic_cast<Binocular*>(k.next());
        if (undoBinoculars)
        {
            index=undoBinoculars->getBinSeqNumber();
            myBinoculars->push_back(make_pair(undoBinoculars, index));
        }
    }
        binocularsModel->setElementList(*myBinoculars);
        binocularsModel->myDataHasChanged();
}

QString createDeleteBinocularCommandString(const QPointF &pos)
{
    QString binocularString;
    binocularString="Strumento ottico";

    return QObject::tr("%1 in (%2, %3)")
        .arg(binocularString)
        .arg(pos.x()).arg(pos.y());
}
