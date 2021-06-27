
#ifndef LASERMOVECOMMANDS_H
#define LASERMOVECOMMANDS_H

#include <QUndoCommand>

#include "../laserpoint.h"

class LaserMoveCommand : public QUndoCommand
{
public:
    enum { Id = 10 };

    LaserMoveCommand(LaserPoint *laserpoint, const QPointF &oldPos,
                QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override { return Id; }

private:
    LaserPoint *myLaserPoint;
    QPointF myOldPos;
    QPointF newPos;
};

QString createCommandString(const QPointF &point);

#endif
