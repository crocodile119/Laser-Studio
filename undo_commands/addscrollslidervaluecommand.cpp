#include "addscrollslidervaluecommand.h"
#include <QDebug>
#include <cmath>

/* Command Stack peril controllo composito slider scrollBar */
AddScrollSliderValueCommand::AddScrollSliderValueCommand(QSlider *_mySlider, QScrollBar *_myScrollBar, int _sliderOldValue, int _ScrollBarOldValue,
                                               QString _title, movement _movementType, command _commandType,
                                       QUndoCommand *parent):QUndoCommand(parent),
    mySlider(_mySlider), myScrollBar(_myScrollBar), sliderOldValue(_sliderOldValue), scrollBarOldValue(_ScrollBarOldValue), title(_title),
    movementType(_movementType), commandType(_commandType)

{
    sliderNewValue=mySlider->sliderPosition();
    scrollBarNewValue=myScrollBar->sliderPosition();
}

bool AddScrollSliderValueCommand::mergeWith(const QUndoCommand *command)
{
    const AddScrollSliderValueCommand *addScrollSliderValueCommand = static_cast<const AddScrollSliderValueCommand *>(command);

    QSlider *slider = addScrollSliderValueCommand->mySlider;
    QScrollBar *scrollBar= addScrollSliderValueCommand->myScrollBar;
    movement myMovement = addScrollSliderValueCommand->movementType;

    if(myMovement==movement::TRIGGER_ACTION)
        return false;

    sliderNewValue = slider->sliderPosition();
    scrollBarNewValue = scrollBar->sliderPosition();
    setText(QString("%1: %2")
            .arg(title)
            .arg(scientificNumber((double)sliderNewValue/100, scrollBarNewValue)));

    return true;
}

void AddScrollSliderValueCommand::redo()
{
    mySlider->setSliderPosition(sliderNewValue);
    myScrollBar->setSliderPosition(scrollBarNewValue);
    setText(QString("%1: %2")
           .arg(title)
           .arg(scientificNumber((double)sliderNewValue/100, scrollBarNewValue)));
}

void AddScrollSliderValueCommand::undo()
{
    myScrollBar->setSliderPosition(scrollBarOldValue);
    mySlider->setSliderPosition(sliderOldValue);

    setText(QString("%1: %2")
           .arg(title)
           .arg(scientificNumber((double)sliderNewValue/100, scrollBarNewValue)));
}

QString AddScrollSliderValueCommand::scientificNumber(const double & mantissa, const int & exponent)
{
    double scientificNumber=mantissa*std::pow(10, -exponent);
    return QString::number(scientificNumber,'e',2);
}

AddScrollSliderValueCommand::~AddScrollSliderValueCommand()
{

}
