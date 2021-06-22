#ifndef ADDCONTROLVALUECOMMAND_H
#define ADDCONTROLVALUECOMMAND_H

#include <QUndoCommand>
#include <QDial>
#include <QScrollBar>
#include <QSlider>

class AddControlValueCommand : public QUndoCommand
{
public:
    enum movement{SLIDER_MOVED, TRIGGER_ACTION};
    enum command{DIAL, SCROLL_BAR};
    const int Id=1;
    AddControlValueCommand(QDial *_myDial, QScrollBar* _myScrollBar, int _dialValueOld, int _ScrollBarOld,
                           QString _title, movement _movementType, command _commandType, QUndoCommand *parent = 0);
    virtual ~AddControlValueCommand();

    int id() const override { return Id; }
    bool mergeWith(const QUndoCommand *command) override;
    void undo() override;
    void redo() override;
    QString scientificNumber(const double&, const int&);

    private: 
    QDial* myDial;
    QScrollBar* myScrollBar;
    int dialOldValue;
    int scrollBarOldValue;
    QString title;
    movement movementType;
    command commandType;
    int dialNewValue;
    int scrollBarNewValue;
};

#endif // ADDCONTROLVALUECOMMAND_H