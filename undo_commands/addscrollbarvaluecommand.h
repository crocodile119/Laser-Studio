#ifndef ADDSCROLLBARVALUECOMMAND_H
#define ADDSCROLLBARVALUECOMMAND_H

#include <QUndoCommand>
#include <QScrollBar>

class AddScrollBarValueCommand : public QUndoCommand
{
public:
    enum movement{SLIDER_MOVED, TRIGGER_ACTION};
    const int Id=2;
    AddScrollBarValueCommand(QScrollBar *_myScrollBar, int _scrollBarValueOld, QString _title, movement _movementType,
                        QUndoCommand *parent = 0);
    virtual ~AddScrollBarValueCommand();

    int id() const override { return Id; }
    bool mergeWith(const QUndoCommand *command) override;
    void undo() override;
    void redo() override;
    QString displayNumber(const int&);

    private:
    QScrollBar* myScrollBar;
    int scrollBarOldValue;
    QString title;
    movement movementType;
    int scrollBarNewValue;
};

#endif //ADDSCROLLBARVALUECOMMAND_H
