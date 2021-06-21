#include "addcontrolvaluecommand.h"
#include <QDebug>
#include <cmath>

AddControlValueCommand::AddControlValueCommand(QDial *_myDial, QScrollBar *_myScrollBar, int _dialOldValue, int _ScrollBarOldValue,
                                               QString _title, movement _movementType, command _commandType,
                                       QUndoCommand *parent):QUndoCommand(parent),
    myDial(_myDial), myScrollBar(_myScrollBar), dialOldValue(_dialOldValue), scrollBarOldValue(_ScrollBarOldValue), title(_title),
    movementType(_movementType), commandType(_commandType)

{
    dialNewValue=myDial->sliderPosition();
    scrollBarNewValue=myScrollBar->sliderPosition();
}

bool AddControlValueCommand::mergeWith(const QUndoCommand *command)
{
    const AddControlValueCommand *addControlValueCommand = static_cast<const AddControlValueCommand *>(command);

    QDial *dial = addControlValueCommand->myDial;
    QScrollBar *scrollBar= addControlValueCommand->myScrollBar;
    movement myMovement = addControlValueCommand->movementType;

    if(myMovement==movement::TRIGGER_ACTION)
        return false;

    dialNewValue = dial->sliderPosition();
    scrollBarNewValue = scrollBar->sliderPosition();
    setText(QString("%1: %2")
            .arg(title)
            .arg(scientificNumber((double)dialNewValue/100, scrollBarNewValue)));

    return true;
}

void AddControlValueCommand::redo()
{
    myDial->setSliderPosition(dialNewValue);
    myScrollBar->setSliderPosition(scrollBarNewValue);
    setText(QString("%1: %2")
           .arg(title)
           .arg(scientificNumber((double)dialNewValue/100, scrollBarNewValue)));
}

void AddControlValueCommand::undo()
{
    myScrollBar->setSliderPosition(scrollBarOldValue);
    myDial->setSliderPosition(dialOldValue);

    setText(QString("%1: %2")
           .arg(title)
           .arg(scientificNumber((double)dialNewValue/100, scrollBarNewValue)));
    qDebug()<<"da undo dialOldValue: "<<dialOldValue;
    qDebug()<<"da undo scrollBarOldValue"<<scrollBarOldValue;
    qDebug()<<"da undo dialNewValue: "<<dialNewValue;
    qDebug()<<"da undo scrollBarNewValue"<<scrollBarNewValue;
}

QString AddControlValueCommand::scientificNumber(const double & mantissa, const int & exponent)
{
    double scientificNumber=mantissa*powf(10, -exponent);
    return QString::number(scientificNumber,'e',2);
}

AddControlValueCommand::~AddControlValueCommand()
{

}
