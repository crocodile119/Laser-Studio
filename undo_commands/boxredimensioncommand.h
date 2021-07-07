#ifndef BOXREDIMENSIONCOMMANDS_H
#define BOXREDIMENSIONCOMMANDS_H

#include <QUndoCommand>
#include "../footprintobject.h"

class BoxRedimensionCommand : public QUndoCommand
{
public:
    BoxRedimensionCommand(FootprintObject *footprint, QRectF _old_rect, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    FootprintObject *footprint;
    QRectF rect;
    QRectF old_rect;
};

QString createBoxEditCommandString(QRectF rect);

#endif
