#ifndef DELETEFOOTPRINTCOMMAND_H
#define DELETEFOOTPRINTCOMMAND_H

#include <QUndoCommand>

#include "../footprintobject.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class DeleteFootprintCommand : public QUndoCommand
{
public:
    DeleteFootprintCommand(FootprintObject *_footprint, ObjectLink* _objectLink, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList <pair<FootprintObject *, int>>*_myFootprints, QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteFootprintCommand();

    void undo() override;
    void redo() override;

private:

    FootprintObject *footprintOnScene=nullptr;
    ObjectLink *objectLink=nullptr;
    double scale;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<FootprintObject *, int>>*myFootprints;
    QPointF deletePosition;
    int modelIndex;
};

QString createDeleteFootprintCommandString(const QPointF &pos);


#endif // DELETEREFLECTORCOMMAND_H
