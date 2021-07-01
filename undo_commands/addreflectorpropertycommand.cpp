#include "addreflectorpropertycommand.h"
#include <QDebug>

AddReflectorPropertyCommand::AddReflectorPropertyCommand(Reflector* _reflector, QPointF _position, double _reflection, int _positioning,
                         QString _descriptionTextEdit, QUndoCommand *parent)
                      : QUndoCommand(parent), reflector(_reflector), position(_position), reflection(_reflection), positioning(_positioning),
                                              descriptionTextEdit(_descriptionTextEdit)
{
    oldPosition=reflector->pos();
    oldReflection=reflector->getMaterialCoeff();
    oldPositioning=reflector->getPositioning();
    oldDescriptionTextEdit=reflector->getDescription();
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
        setText(undoReflectorPropertyString(position, positioning, descriptionTextEdit, oldPosition,
                                              oldPositioning, oldDescriptionTextEdit));
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
    setText(redoReflectorPropertyString(reflector, oldPosition,
                                          oldPositioning, oldDescriptionTextEdit));
}

AddReflectorPropertyCommand::~AddReflectorPropertyCommand()
{

}

QString redoReflectorPropertyString(Reflector *reflector, QPointF oldPosition, int oldPositioning,
                                    QString oldDescription)
{
    QPointF position=reflector->pos();
    int positioning=reflector->getPositioning();
    QString description=reflector->getDescription();

    QString positionString;
    QString positioningString;
    QString descriptionString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldPositioning!=positioning)
        positioningString=QString(" angolo di posizionamento %1 gradi")
                .arg(positioning);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((position==oldPosition)&&(oldPositioning==positioning)&&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il riflettore: %1%2%3%4")
        .arg(positionString)
        .arg(positioningString)
        .arg(descriptionString)
        .arg(actionString);
}

QString undoReflectorPropertyString(QPointF position, int positioning,
                                    QString description, QPointF oldPosition, int oldPositioning,
                                    QString oldDescription)
{
    QString positionString;
    QString positioningString;
    QString descriptionString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldPositioning!=positioning)
        positioningString=QString(" angolo di posizionamento %1 gradi")
                .arg(positioning);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((position==oldPosition)&&(oldPositioning==positioning)&&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il riflettore: %1%2%3%4")
        .arg(positionString)
        .arg(positioningString)
        .arg(descriptionString)
        .arg(actionString);
}
