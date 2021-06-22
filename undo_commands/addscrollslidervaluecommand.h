#ifndef ADDSCROLLSLIDERVALUECOMMAND_H
#define ADDSCROLLSLIDERVALUECOMMAND_H

#include <QUndoCommand>
#include <QScrollBar>
#include <QSlider>

class AddScrollSliderValueCommand : public QUndoCommand
{
public:
    enum movement{SLIDER_MOVED, TRIGGER_ACTION};
    enum command{SLIDER, SCROLL_BAR};
    const int Id=3;
    AddScrollSliderValueCommand(QSlider *_mySlider, QScrollBar* _myScrollBar, int _sliderlOldValue, int _ScrollBarOld,
                           QString _title, movement _movementType, command _commandType, QUndoCommand *parent = 0);
    virtual ~AddScrollSliderValueCommand();

    int id() const override { return Id; }
    bool mergeWith(const QUndoCommand *command) override;
    void undo() override;
    void redo() override;
    QString scientificNumber(const double&, const int&);

    private:
    QSlider* mySlider;
    QScrollBar* myScrollBar;
    int sliderOldValue;
    int scrollBarOldValue;
    QString title;
    movement movementType;
    command commandType;
    int sliderNewValue;
    int scrollBarNewValue;
};
#endif //ADDSCROLLSLIDERVALUECOMMAND_H
