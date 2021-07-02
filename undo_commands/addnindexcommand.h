#ifndef ADDNINDEXCOMMAND_H
#define ADDNINDEXCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../reflector.h"


class AddNIndexCommand : public QUndoCommand
{
public:
    AddNIndexCommand(Reflector *_reflector, double _n_index, QUndoCommand *parent = nullptr);

    ~AddNIndexCommand();

    void undo() override;
    void redo() override;

private:
    Reflector *reflector;
    double n_index;
    double old_n_index;


};

QString createNIndexCommandString(const double n_index);

#endif
