#ifndef DELETEREFLECTORCOMMAND_H
#define DELETEREFLECTORCOMMAND_H

#include <QUndoCommand>

#include "../reflector.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class DeleteReflectorCommand : public QUndoCommand
{
public:
    DeleteReflectorCommand(Reflector *_reflector, ReflectorLink *_reflectorlink, double *scale, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<Reflector *> *_myReflecotrs, QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteReflectorCommand();

    void undo() override;
    void redo() override;

private:
    typedef QPair<LaserPoint *, Reflector *> NodePair;
    NodePair selectedNodePair() const;

    Reflector *reflectorOnScene=nullptr;
    ReflectorLink *reflectorlink=nullptr;
    double *scale;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <Reflector *>*myReflectors;
    QPointF deletePosition;
};

QString createDeleteCommandString(Reflector *item, const QPointF &pos);


#endif // DELETEREFLECTORCOMMAND_H
