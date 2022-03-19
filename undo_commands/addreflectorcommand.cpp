#include "addreflectorcommand.h"

AddReflectorCommand::AddReflectorCommand(double _attenuatedDNRO, double _attenuatedDNRC, double _reflectorDistance, double *_scale,
                                         int _seqNumber, target _myTarget, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                                         QList<Reflector *> *_myReflectors, QPointF _initialPosition, QUndoCommand *parent)
                      : QUndoCommand(parent), attenuatedDNRO(_attenuatedDNRO), attenuatedDNRC(_attenuatedDNRC), reflectorDistance(_reflectorDistance),
                        scale(_scale), seqNumber(_seqNumber), myTarget(_myTarget), laserWindow(_laserWindow), laserpoint(_laserpoint),
                        myReflectors(_myReflectors), initialPosition(_initialPosition)
{
    double divergence = laserWindow->myDockControls->getDivergence();
    double beamDiameter = laserWindow->myDockControls->getBeamDiameter();
    double EMP = laserWindow->myDockControls->getEMP();
    double powerErgForEMP = laserWindow->myDockControls->getPowerErgForEMP();
    double lambertianMax=laserWindow->myDockControls->getLambertianMax();

    reflector = new Reflector(_attenuatedDNRO,
                              divergence,
                              _reflectorDistance,
                              beamDiameter,
                              EMP,
                              powerErgForEMP,
                              lambertianMax,
                              myTarget);

    reflector->setPixmap();
    reflector->setPos(initialPosition);
    reflector->setSkinDistance(attenuatedDNRC);
    reflector->setReflectorColor();
    reflector->setBackgroundColor(QColor(247, 247, 247, 170));
    reflector->setTextLabel();
    reflector->setStringDetails();
    reflector->setSeqNumber(seqNumber);
    reflectorlink=addReflectorLink();

    setText(QObject::tr("Aggiungo %1")
        .arg(createAddRefletorCommandString(reflector, initialPosition)));
}

void AddReflectorCommand::undo()
{
    laserWindow->graphicsView->scene()->removeItem(reflector);
    laserWindow->graphicsView->scene()->removeItem(reflectorlink);
    laserWindow->graphicsView->scene()->update();

    myReflectors->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene()->items();

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

void AddReflectorCommand::redo()
{
    reflector->setPixScale(*scale);
    laserWindow->graphicsView->scene()->addItem(reflector);
    laserWindow->graphicsView->scene()->clearSelection();

    laserWindow->graphicsView->scene()->addItem(reflectorlink);

    QGraphicsItem *item =laserWindow->graphicsView->scene()->itemAt(initialPosition, QTransform());
    reflectorOnScene= qgraphicsitem_cast<Reflector*>(item);

    QString reflectorName=reflector->objectName();

    if(reflectorOnScene==nullptr)
    {
        QList<QGraphicsItem*>items=laserWindow->graphicsView->scene()->collidingItems(item);

        QMutableListIterator<QGraphicsItem *> k(items);
        while (k.hasNext())
        {
            reflectorOnScene = dynamic_cast<Reflector *>(k.next());
            if(reflectorOnScene!=nullptr)
            {
                if(reflectorOnScene->objectName()==reflectorName)
                break;
            }
        }
    }

    myReflectors->append(reflectorOnScene);


    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene()->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    reflector->setSelected(true);

    reflectorOnScene->update();
}

AddReflectorCommand::~AddReflectorCommand()
{
    if(!laserWindow->graphicsView->scene())
        delete reflector;
}

ReflectorLink* AddReflectorCommand::addReflectorLink()
{
    ReflectorNodePair reflectorNodes = selectedReflectorNodePair();
    if (reflectorNodes == ReflectorNodePair())
        return nullptr;

    ReflectorLink *reflectorlink = new ReflectorLink(reflectorNodes.first, reflectorNodes.second);

    return reflectorlink;
}

Reflector* AddReflectorCommand::getReflector()const
{
    return reflector;
}

AddReflectorCommand::ReflectorNodePair AddReflectorCommand::selectedReflectorNodePair() const
{
    return ReflectorNodePair(laserpoint, reflector);
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
