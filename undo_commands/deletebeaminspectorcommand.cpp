#include "deletebeaminspectorcommand.h"

DeleteBeamInspectorCommand::DeleteBeamInspectorCommand(BeamInspector *_beamInspector, InspectorLink *_inspectorLink, double _scale,
                  CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<pair<BeamInspector *, int> > *_myBeamInspectors, InspectorsListModel *_inspectorsModel,
                  QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), beamInspectorOnScene(_beamInspector), inspectorLink(_inspectorLink), scale(_scale), laserWindow(_laserWindow), laserpoint(_laserpoint),
            myBeamInspectors(_myBeamInspectors), inspectorsModel(_inspectorsModel), deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteInspectorCommandString(deletePosition)));

}

DeleteBeamInspectorCommand::~DeleteBeamInspectorCommand()
{

}

void DeleteBeamInspectorCommand::undo()
{
    laserWindow->graphicsView->scene->addItem(inspectorLink);
    laserWindow->graphicsView->scene->addItem(beamInspectorOnScene);
    beamInspectorOnScene->setPos(deletePosition);
    laserWindow->graphicsView->scene->clearSelection();
    laserWindow->graphicsView->scene->update();

    int modelIndex=inspectorsModel->addElement(*beamInspectorOnScene);
    myBeamInspectors->append(make_pair(beamInspectorOnScene, modelIndex));
    beamInspectorOnScene->setInspectorSeqNumber(modelIndex);

    inspectorsModel->myDataHasChanged();

    laserpoint->setSelected(false);

    laserWindow->graphicsView->scene->clearSelection();
    //imposto la NOHD del punto laser
    laserpoint->setOpticalDiameter(laserWindow->myDockControls->getOpticalDistance());
    beamInspectorOnScene->setSelected(true);

    beamInspectorOnScene->update();
}

void DeleteBeamInspectorCommand::redo()
{
    laserWindow->graphicsView->scene->removeItem(inspectorLink);
    laserWindow->graphicsView->scene->removeItem(beamInspectorOnScene);

    myBeamInspectors->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene->items();

    int index;

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        BeamInspector *undoBeamInspectors = dynamic_cast<BeamInspector*>(k.next());
        if (undoBeamInspectors)
        {
            index=undoBeamInspectors->getInspectorSeqNumber();
            myBeamInspectors->push_back(make_pair(undoBeamInspectors, index));
        }
    }
        inspectorsModel->setElementList(*myBeamInspectors);
        inspectorsModel->myDataHasChanged();
}

QString createDeleteInspectorCommandString(const QPointF &pos)
{
    QString inspectorString;
    inspectorString="Segnaposto di ispezione";

    return QObject::tr("%1 in (%2, %3)")
        .arg(inspectorString)
        .arg(pos.x()).arg(pos.y());
}