#include "addfootprintcommand.h"

AddFootprintCommand::AddFootprintCommand(double _attenuatedDNRO, double _scale, int _footprintSeqNumber, CentralWidget *_laserWindow,
                    LaserPoint *_laserpoint, QList<pair<FootprintObject *, int> > *_myFootprints, QPointF _initialPosition,
                    QUndoCommand *parent)
                    : QUndoCommand(parent), attenuatedDNRO(_attenuatedDNRO), scale(_scale), footprintSeqNumber(_footprintSeqNumber),
                      laserWindow(_laserWindow), laserpoint(_laserpoint), myFootprints(_myFootprints), initialPosition(_initialPosition)
{
    footprint= new FootprintObject(scale);

    footprint->setPos(initialPosition);
    footprint->rectangle().setRect(-20, -20, 40, 40);

    footprint->setDNRO_Diameter(attenuatedDNRO);
    footprint->setLaserBeamPath(laserpoint->mapToItem(footprint, laserpoint->shapePath()));

    footprint->setItemScale(scale);
    laserWindow->graphicsView->scene->addItem(footprint);

    objectLink =addObjectLink();

    footprint->updateTipString();
    footprint->setFootprintSeqNumber(footprintSeqNumber);
    footprint->setLaserPosition();
    myFootprints->append(make_pair(footprint, footprintSeqNumber));

    footprintSeqNumber=myFootprints->count();


    setText(QObject::tr("Aggiungo %1")
        .arg(createAddFootprintCommandString(initialPosition)));
}

void AddFootprintCommand::undo()
{
    laserWindow->graphicsView->scene->removeItem(footprint);
    laserWindow->graphicsView->scene->removeItem(objectLink);
    laserWindow->graphicsView->scene->update();

    myFootprints->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    int index;

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        FootprintObject *undoFootprints = dynamic_cast<FootprintObject *>(k.next());
        if (undoFootprints)
        {
        index=undoFootprints->getFootprintSeqNumber();
        myFootprints->push_back(make_pair(undoFootprints, index));
        }
    }
    laserWindow->graphicsView->scene->update();
}

void AddFootprintCommand::redo()
{
    laserWindow->graphicsView->scene->addItem(footprint);
    laserWindow->graphicsView->scene->clearSelection();

    laserWindow->graphicsView->scene->addItem(objectLink);

    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(initialPosition, QTransform());
    footprintOnScene= qgraphicsitem_cast<FootprintObject*>(item);

    int index=footprintOnScene->getFootprintSeqNumber();
    myFootprints->append(make_pair(footprintOnScene, index));


    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    footprint->setSelected(true);

    footprintOnScene->update();
}

AddFootprintCommand::~AddFootprintCommand()
{
    if(!laserWindow->graphicsView->scene)
        delete footprint;
}

ObjectLink *AddFootprintCommand::addObjectLink()
{
    ObjectNodePair objectNodes = selectedObjectNodePair();
    if (objectNodes == ObjectNodePair())
        return nullptr;

    ObjectLink *objectlink = new ObjectLink(objectNodes.first, objectNodes.second);

    return objectlink;
}

FootprintObject* AddFootprintCommand::getFootprint()const
{
    return footprint;
}

AddFootprintCommand::ObjectNodePair AddFootprintCommand::selectedObjectNodePair() const
{
    return ObjectNodePair(laserpoint, footprint);
}

QString createAddFootprintCommandString(const QPointF &pos)
{
    QString footprintString="ingombro";

    return QObject::tr("%1 in (%2, %3)")
        .arg(footprintString)
        .arg(pos.x()).arg(pos.y());
}
