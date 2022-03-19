#include "addnindexcommand.h"

AddNIndexCommand::AddNIndexCommand(Reflector *_reflector, double _n_index, QUndoCommand *parent)
                      : QUndoCommand(parent), reflector(_reflector), n_index(_n_index)
{  
   old_n_index=reflector->getRefration_n();
}

void AddNIndexCommand::undo()
{
    reflector->setRefraction_n(old_n_index);
    reflector->setStringDetails();
    setText(createNIndexCommandString(n_index));
}

void AddNIndexCommand::redo()
{
    reflector->setRefraction_n(n_index);
    reflector->setStringDetails();
    setText(createNIndexCommandString(n_index));
}

AddNIndexCommand::~AddNIndexCommand()
{

}

QString createNIndexCommandString(const double n_index)
{
    QString n_indexString;
    n_indexString="Cambio l'indice di rifrazione del riflettore di vetro:";

    return QObject::tr("%1: %2")
        .arg(n_indexString)
        .arg(n_index);
}
