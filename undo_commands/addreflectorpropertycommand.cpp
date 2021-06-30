#include "addreflectorpropertycommand.h"

AddReflectorPropertyCommand::AddReflectorPropertyCommand(Reflector* _reflector, QPointF _position, double _reflection, int _positioning,
                         QString _descriptionTextEdit, QUndoCommand *parent)
                      : QUndoCommand(parent), reflector(_reflector), position(_position), reflection(_reflection), positioning(_positioning),
                                              descriptionTextEdit(_descriptionTextEdit)
{
    oldPosition=reflector->pos();
    oldReflection=reflector->getMaterialCoeff();
    oldPositioning=reflector->getPositioning();
    oldDescriptionTextEdit=reflector->getDescription();


    setText(QObject::tr("Modifico le proprietà del punto laser"));
}

void AddReflectorPropertyCommand::undo()
{
    reflector->setPos(oldPosition);
    reflector->setMaterialCoeff(oldReflection);
    reflector->setPositioning(oldPositioning);
    reflector->reflectorOperation();
    reflector->setStringDetails();
    reflector->setDescription(oldDescriptionTextEdit);
    reflector->laserParametersChanged();
}

void AddReflectorPropertyCommand::redo()
{
    reflector->setPos(position);
    reflector->setMaterialCoeff(reflection);
    reflector->setPositioning(positioning);
    reflector->reflectorOperation();
    reflector->setStringDetails();
    reflector->setDescription(descriptionTextEdit);
    reflector->laserParametersChanged();
}

AddReflectorPropertyCommand::~AddReflectorPropertyCommand()
{

}

