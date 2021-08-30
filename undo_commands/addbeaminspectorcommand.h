#ifndef ADDBEAMINSPECTORCOMMAND_H
#define ADDBEAMINSPECTORCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../beaminspector.h"
#include "../laserpoint.h"
#include "../inspectorslistmodel.h"
#include "centralwidget.h"



class AddBeamInspectorCommand : public QUndoCommand
{
public:
    AddBeamInspectorCommand(double _inspectorDistance, double _scale,
               int _inspectorSeqNumber, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
               QList<pair<BeamInspector *, int> > *_myBeamInspectors, InspectorsListModel *_inspectorsModel,
               QPointF _initialPosition, QUndoCommand *parent = nullptr);

    ~AddBeamInspectorCommand();

    void undo() override;
    void redo() override;
    InspectorLink* addInspectorLink();
    BeamInspector* getBeamInspector() const;

private:
    typedef QPair<LaserPoint *, BeamInspector *> InspectorNodePair;
    InspectorNodePair selectedInspectorNodePair() const;

    double inspectorDistance;
    double scale;
    int inspectorSeqNumber;
    BeamInspector *beamInspector=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<BeamInspector *, int>>*myBeamInspectors;
    InspectorsListModel *inspectorsModel;
    QPointF initialPosition;
    InspectorLink *inspectorLink=nullptr;
    BeamInspector *beamInspectorOnScene=nullptr;
};

QString createAddInspectorCommandString(const QPointF&);

#endif
