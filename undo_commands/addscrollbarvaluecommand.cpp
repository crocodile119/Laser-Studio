#include "addscrollbarvaluecommand.h"
#include <QDebug>
#include <cmath>

/* Command Stack peril controllo dial con label per visualizzazione input */
AddScrollBarValueCommand::AddScrollBarValueCommand(QScrollBar *_myScrollBar, int _scrollBarOldValue, QString _title,
                                         movement _movementType, QUndoCommand *parent):QUndoCommand(parent),
    myScrollBar(_myScrollBar), scrollBarOldValue(_scrollBarOldValue), title(_title), movementType(_movementType)

{
    scrollBarNewValue=myScrollBar->sliderPosition();
}

bool AddScrollBarValueCommand::mergeWith(const QUndoCommand *command)
{
    const AddScrollBarValueCommand *addScrollBarValueCommand = static_cast<const AddScrollBarValueCommand *>(command);

    QScrollBar *scrollBar = addScrollBarValueCommand->myScrollBar;
    movement myMovement = addScrollBarValueCommand->movementType;

    if(myMovement==movement::TRIGGER_ACTION)
        return false;

    scrollBarNewValue = scrollBar->sliderPosition();
    setText(QString("%1: %2")
            .arg(title)
            .arg(displayNumber((double)scrollBarNewValue)));

    return true;
}

void AddScrollBarValueCommand::redo()
{
    myScrollBar->setSliderPosition(scrollBarNewValue);
    setText(QString("%1: %2")
           .arg(title)
           .arg(displayNumber((double)scrollBarNewValue)));
}

void AddScrollBarValueCommand::undo()
{
    myScrollBar->setSliderPosition(scrollBarOldValue);

    setText(QString("%1: %2")
           .arg(title)
           .arg(displayNumber((double)scrollBarNewValue)));
}

QString AddScrollBarValueCommand::displayNumber(const int & _scrollBarValue)
{
    return QString::number(_scrollBarValue);
}

AddScrollBarValueCommand::~AddScrollBarValueCommand()
{

}
