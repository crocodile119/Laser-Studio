#include "addbeaminspectorcommand.h"

AddBeamInspectorCommand::AddBeamInspectorCommand(double _inspectorDistance, double *_scale, int _inspectorSeqNumber,
                                         CentralWidget *_laserWindow, LaserPoint *_laserpoint, QList<BeamInspector *> *_myBeamInspectors,
                                         QPointF _initialPosition, double _attenuatedDNRO, QUndoCommand *parent)
                                         : QUndoCommand(parent), inspectorDistance(_inspectorDistance), scale(_scale), inspectorSeqNumber(_inspectorSeqNumber),
                                         laserWindow(_laserWindow), laserpoint(_laserpoint), myBeamInspectors(_myBeamInspectors),
                                         initialPosition(_initialPosition), attenuatedDNRO(_attenuatedDNRO)
{  
    double _wavelength=laserWindow->myDockControls->getWavelength();
    double _divergence=laserWindow->myDockControls->getDivergence();
    double _beamDiameter=laserWindow->myDockControls->getBeamDiameter();

    beamInspector = new BeamInspector(_inspectorDistance, _wavelength, _divergence, _beamDiameter, _attenuatedDNRO);

    beamInspector->setPos(initialPosition);
    beamInspector->setBeamInspectorObjectName(initialPosition);

    inspectorLink=addInspectorLink();

    setText(QObject::tr("Aggiungo %1")
        .arg(createAddInspectorCommandString(initialPosition)));
}

void AddBeamInspectorCommand::undo()
{
    laserWindow->graphicsView->scene()->removeItem(beamInspector);
    laserWindow->graphicsView->scene()->removeItem(inspectorLink);
    laserWindow->graphicsView->scene()->update();

    myBeamInspectors->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene()->items();

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        BeamInspector *undoBeamInspectors = dynamic_cast<BeamInspector *>(k.next());
        if (undoBeamInspectors)
        {
            myBeamInspectors->push_back(undoBeamInspectors);
        }
    }
}

void AddBeamInspectorCommand::redo()
{
    beamInspector->setPixScale(*scale);
    laserWindow->graphicsView->scene()->addItem(beamInspector);
    laserWindow->graphicsView->scene()->clearSelection();

    laserWindow->graphicsView->scene()->addItem(inspectorLink);

    beamInspector->setStringPosition();
    beamInspector->setInspectorSeqNumber(inspectorSeqNumber);
    QPointF shiftPosition =initialPosition-BeamInspector::positionShift(*scale);
    QGraphicsItem *item =laserWindow->graphicsView->scene()->itemAt(shiftPosition, QTransform());
    beamInspectorOnScene= qgraphicsitem_cast<BeamInspector*>(item);

    QString reflectorName=beamInspector->objectName();

    if(beamInspectorOnScene==nullptr)
    {
        QList<QGraphicsItem*>items=laserWindow->graphicsView->scene()->collidingItems(item);

        QMutableListIterator<QGraphicsItem *> k(items);
        while (k.hasNext())
        {
            beamInspectorOnScene = dynamic_cast<BeamInspector *>(k.next());
            if(beamInspectorOnScene!=nullptr)
            {
                if(beamInspectorOnScene->objectName()==reflectorName)
                break;
            }
        }
    }

    myBeamInspectors->append(beamInspectorOnScene);

    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene()->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());

    beamInspector->setSelected(true);
    beamInspectorOnScene->update();
}

AddBeamInspectorCommand::~AddBeamInspectorCommand()
{
    if(!laserWindow->graphicsView->scene())
        delete beamInspector;
}

InspectorLink* AddBeamInspectorCommand::addInspectorLink()
{
    InspectorNodePair nodes = selectedInspectorNodePair();
    if (nodes == InspectorNodePair())
        return nullptr;;

    InspectorLink *inspectorLink = new InspectorLink(nodes.first, nodes.second);

    return inspectorLink;
}

BeamInspector* AddBeamInspectorCommand::getBeamInspector()const
{
    return beamInspector;
}

AddBeamInspectorCommand::InspectorNodePair AddBeamInspectorCommand::selectedInspectorNodePair() const
{
    return InspectorNodePair(laserpoint, beamInspector);
}

QString createAddInspectorCommandString(const QPointF &pos)
{
    QString inspectorString;
    inspectorString="segnaposto di ispezione";

    return QObject::tr("%1 in (%2, %3)")
        .arg(inspectorString)
        .arg(pos.x()).arg(pos.y());
}
