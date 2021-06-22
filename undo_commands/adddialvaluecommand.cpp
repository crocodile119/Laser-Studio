#include "adddialvaluecommand.h"
#include <QDebug>
#include <cmath>

/* Command Stack peril controllo dial con label per visualizzazione input */
AddDialValueCommand::AddDialValueCommand(QDial *_myDial, int _dialOldValue, QString _title,
                                         movement _movementType, QUndoCommand *parent):QUndoCommand(parent),
    myDial(_myDial), dialOldValue(_dialOldValue), title(_title), movementType(_movementType)

{
    dialNewValue=myDial->sliderPosition();
}

bool AddDialValueCommand::mergeWith(const QUndoCommand *command)
{
    const AddDialValueCommand *addDialValueCommand = static_cast<const AddDialValueCommand *>(command);

    QDial *dial = addDialValueCommand->myDial;
    movement myMovement = addDialValueCommand->movementType;

    if(myMovement==movement::TRIGGER_ACTION)
        return false;

    dialNewValue = dial->sliderPosition();
    setText(QString("%1: %2")
            .arg(title)
            .arg(displayNumber((double)dialNewValue)));

    return true;
}

void AddDialValueCommand::redo()
{
    myDial->setSliderPosition(dialNewValue);
    setText(QString("%1: %2")
           .arg(title)
           .arg(displayNumber((double)dialNewValue)));
}

void AddDialValueCommand::undo()
{
    myDial->setSliderPosition(dialOldValue);

    setText(QString("%1: %2")
           .arg(title)
           .arg(displayNumber((double)dialNewValue)));
}

QString AddDialValueCommand::displayNumber(const int & _dialValue)
{
    return QString::number(_dialValue);
}

AddDialValueCommand::~AddDialValueCommand()
{

}
