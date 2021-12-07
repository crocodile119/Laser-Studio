#ifndef DELETEBEAMINSPECTORCOMMAND_H
#define DELETEBEAMINSPECTORCOMMAND_H

#include <QUndoCommand>

#include "../beaminspector.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class DeleteBeamInspectorCommand : public QUndoCommand
{
public:
    DeleteBeamInspectorCommand(BeamInspector *_beamInspector, InspectorLink *_inspectorLink, double *_scale,
                  CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<BeamInspector *> *_myBeamInspector,
                  QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteBeamInspectorCommand();

    void undo() override;
    void redo() override;

private:
    typedef QPair<LaserPoint *, BeamInspector *> InspectorNodePair;
    InspectorNodePair selectedInspectorNodePair() const;

    BeamInspector *beamInspectorOnScene=nullptr;
    InspectorLink *inspectorLink=nullptr;
    double* scale;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <BeamInspector *>*myBeamInspectors;
    QPointF deletePosition;
};

QString createDeleteInspectorCommandString(const QPointF &pos);


#endif // DELETEBINOCULARCOMMAND_H
