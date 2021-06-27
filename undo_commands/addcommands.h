#ifndef ADDCOMMANDS_H
#define ADDCOMMANDS_H

#include <QUndoCommand>

#include "../reflector.h"

class AddCommand : public QUndoCommand
{
public:
    AddCommand(target addType, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    Reflector *myReflector;
    QGraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};

#endif
