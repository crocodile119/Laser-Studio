#include "addreflectorcommand.h"

AddReflectorCommand::AddReflectorCommand(double _attenuatedDNRO, double _attenuatedDNRC, double _reflectorDistance, double _scale,
                                         int _seqNumber, target _myTarget, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                                         QList <pair<Reflector *, int>>*_myReflectors, ReflectorsListModel *_reflectorsModel,
                                         QPointF _initialPosition, QUndoCommand *parent)
                      : QUndoCommand(parent), attenuatedDNRO(_attenuatedDNRO), attenuatedDNRC(_attenuatedDNRC), reflectorDistance(_reflectorDistance),
                        scale(_scale), seqNumber(_seqNumber), myTarget(_myTarget), laserWindow(_laserWindow), laserpoint(_laserpoint),
                        myReflectors(_myReflectors), reflectorsModel(_reflectorsModel), initialPosition(_initialPosition)
{
    reflector = new Reflector(_attenuatedDNRO,
                              laserWindow->myDockControls->getDivergence(),
                              _reflectorDistance,
                              laserWindow->myDockControls->getBeamDiameter(),
                              laserWindow->myDockControls->getEMP(),
                              laserWindow->myDockControls->getPowerErgForEMP(),
                              laserWindow->myDockControls->getLambertianMax(),
                              myTarget);

    reflector->setPixmap();
    reflector->setPixScale(scale);

    reflector->setSkinDistance(attenuatedDNRC);
    reflector->setReflectorColor();
    reflector->setBackgroundColor(QColor(247, 247, 247, 170));
    reflector->setPos(initialPosition);
    reflector->setTextLabel();
    reflector->setStringDetails();
    reflector->setSeqNumber(seqNumber);
    link=addLink();

    setText(QObject::tr("Aggiungo %1")
        .arg(createAddRefletorCommandString(reflector, initialPosition)));
}

void AddReflectorCommand::undo()
{
    laserWindow->graphicsView->scene->removeItem(reflector);
    laserWindow->graphicsView->scene->removeItem(link);
    laserWindow->graphicsView->scene->update();

    myReflectors->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    int index;

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        Reflector *undoReflectors = dynamic_cast<Reflector *>(k.next());
        if (undoReflectors)
        {
        index=undoReflectors->getSeqNumber();
        myReflectors->push_back(make_pair(undoReflectors, index));
        }
    }
    reflectorsModel->setElementList(*myReflectors);
    reflectorsModel->myDataHasChanged();
}

void AddReflectorCommand::redo()
{
    laserWindow->graphicsView->scene->addItem(reflector);
    laserWindow->graphicsView->scene->clearSelection();

    laserWindow->graphicsView->scene->addItem(link);

    QGraphicsItem *item =laserWindow->graphicsView->scene->itemAt(initialPosition, QTransform());
    reflectorOnScene= qgraphicsitem_cast<Reflector*>(item);

    int modelIndex=reflectorsModel->addElement(*reflectorOnScene);
    myReflectors->append(make_pair(reflectorOnScene, modelIndex));
    reflectorOnScene->setSeqNumber(modelIndex);

    reflectorsModel->myDataHasChanged();

    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    reflector->setSelected(true);

    reflectorOnScene->update();
}

AddReflectorCommand::~AddReflectorCommand()
{
    if(!laserWindow->graphicsView->scene)
        delete reflector;
}

Link* AddReflectorCommand::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return nullptr;;

    Link *link = new Link(nodes.first, nodes.second);

    return link;
}

Reflector* AddReflectorCommand::getReflector()const
{
    return reflector;
}

AddReflectorCommand::NodePair AddReflectorCommand::selectedNodePair() const
{
    return NodePair(laserpoint, reflector);
}

QString createAddRefletorCommandString(Reflector *item, const QPointF &pos)
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
