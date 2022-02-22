#ifndef MOVECOMMANDS_H
#define MOVECOMMANDS_H

#include <QUndoCommand>

#include "../reflector.h"
#include "../binocular.h"
#include "../beaminspector.h"
#include "../laserpoint.h"
#include "../labroom.h"
#include "../footprintobject.h"
#include "../safetysignitem.h"

class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 20 };

    MoveCommand(QGraphicsItem *itemMoved, const QPointF &oldPos, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override { return Id; }

private:
    QGraphicsItem *myMovedItem;
    QPointF myOldPos;
    QPointF newPos;
};

QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif
