#ifndef ADDDIALVALUECOMMAND_H
#define ADDDIALVALUECOMMAND_H

#include <QUndoCommand>
#include <QDial>

class AddDialValueCommand : public QUndoCommand
{
public:
    enum movement{SLIDER_MOVED, TRIGGER_ACTION};
    const int Id=2;
    AddDialValueCommand(QDial *_myDial, int _dialValueOld, QString _title, movement _movementType,
                        QUndoCommand *parent = 0);
    virtual ~AddDialValueCommand();

    int id() const override { return Id; }
    bool mergeWith(const QUndoCommand *command) override;
    void undo() override;
    void redo() override;
    QString displayNumber(const int&);

    private:
    QDial* myDial;
    int dialOldValue;
    QString title;
    movement movementType;
    int dialNewValue;
};

#endif //ADDDIALVALUECOMMAND_H
