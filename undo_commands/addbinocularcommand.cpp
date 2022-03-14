#include "addbinocularcommand.h"

AddBinocularCommand::AddBinocularCommand(double _attenuatedDNRO, double _binocularDistance, double *_scale,
                                         int _binSeqNumber, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                                         QList<Binocular *> *_myBinoculars, QPointF _initialPosition,
                                         QUndoCommand *parent):QUndoCommand(parent), attenuatedDNRO(_attenuatedDNRO),
                                         binocularDistance(_binocularDistance), scale(_scale), binSeqNumber(_binSeqNumber),
                                         laserWindow(_laserWindow), laserpoint(_laserpoint), myBinoculars(_myBinoculars),
                                         initialPosition(_initialPosition)
{  
    double wavelength=laserWindow->myDockControls->getWavelength();
    double divergence=laserWindow->myDockControls->getDivergence();

    binocular = new Binocular(_attenuatedDNRO,
                              _binocularDistance,
                              wavelength,
                              divergence,
                              laserWindow->myDockControls->getBeamDiameter());

    binocular->setPos(initialPosition);  
    binocular->setBinocularObjectName(initialPosition);
    binocularLink=addBinocularLink();

    double exendedOpticalDiameter=binocular->getExendedOpticalDiameter();
    bool binocularInZone=laserpoint->shapeEnhacedPathContainsPoint(laserpoint->mapFromScene(binocular->pos()), exendedOpticalDiameter);
    binocular->setInZone(binocularInZone);

    setText(QObject::tr("Aggiungo %1")
        .arg(createAddBinocularCommandString(initialPosition)));
}

void AddBinocularCommand::undo()
{
    laserWindow->graphicsView->scene()->removeItem(binocular);
    laserWindow->graphicsView->scene()->removeItem(binocularLink);
    laserWindow->graphicsView->scene()->update();

    myBinoculars->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene()->items();
    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        Binocular *undoBinoculars = dynamic_cast<Binocular *>(k.next());
        if (undoBinoculars)
        {
        myBinoculars->push_back(undoBinoculars);
        }
    }
}

void AddBinocularCommand::redo()
{
    binocular->setPixScale(*scale);
    laserWindow->graphicsView->scene()->addItem(binocular);
    laserWindow->graphicsView->scene()->clearSelection();

    laserWindow->graphicsView->scene()->addItem(binocularLink);

    binocular->setTextLabel();
    binocular->setStringPosition();
    binocular->setBinSeqNumber(binSeqNumber);
    QString binocularName=binocular->objectName();
    QGraphicsItem *item =laserWindow->graphicsView->scene()->itemAt(initialPosition, QTransform());
    binocularOnScene= qgraphicsitem_cast<Binocular*>(item);

    if(binocularOnScene==nullptr)
    {
        QList<QGraphicsItem*>items=laserWindow->graphicsView->scene()->collidingItems(item);

        QMutableListIterator<QGraphicsItem *> k(items);
        while (k.hasNext())
        {
            binocularOnScene = dynamic_cast<Binocular *>(k.next());
            if(binocularOnScene!=nullptr)
            {
                if(binocularOnScene->objectName()==binocularName)
                break;
            }
        }
    }

    myBinoculars->append(binocularOnScene);

    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene()->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    binocular->setSelected(true);

    binocularOnScene->update();
}

AddBinocularCommand::~AddBinocularCommand()
{
    if(!laserWindow->graphicsView->scene())
        delete binocular;
}

BinocularLink* AddBinocularCommand::addBinocularLink()
{
    BinocularNodePair nodes = selectedBinocularNodePair();
    if (nodes == BinocularNodePair())
        return nullptr;;

    BinocularLink *binocularLink = new BinocularLink(nodes.first, nodes.second);

    return binocularLink;
}

Binocular* AddBinocularCommand::getBinocular()const
{
    return binocular;
}

AddBinocularCommand::BinocularNodePair AddBinocularCommand::selectedBinocularNodePair() const
{
    return BinocularNodePair(laserpoint, binocular);
}

QString createAddBinocularCommandString(const QPointF &pos)
{
    QString binocularString;
    binocularString="strumento ottico";

    return QObject::tr("%1 in (%2, %3)")
        .arg(binocularString)
        .arg(pos.x()).arg(pos.y());
}
