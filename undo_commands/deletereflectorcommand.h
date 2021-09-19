#ifndef DELETEREFLECTORCOMMAND_H
#define DELETEREFLECTORCOMMAND_H

#include <QUndoCommand>

#include "../reflector.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class DeleteReflectorCommand : public QUndoCommand
{
public:
    DeleteReflectorCommand(Reflector *_reflector, Link *_link, double _scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<pair<Reflector *, int> > *_myReflecotrs,
                  QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteReflectorCommand();

    void undo() override;
    void redo() override;

private:
    typedef QPair<LaserPoint *, Reflector *> NodePair;
    NodePair selectedNodePair() const;

    Reflector *reflectorOnScene=nullptr;
    Link *link=nullptr;
    double scale;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<Reflector *, int>>*myReflectors;
    QPointF deletePosition;
};

QString createDeleteCommandString(Reflector *item, const QPointF &pos);


#endif // DELETEREFLECTORCOMMAND_H
