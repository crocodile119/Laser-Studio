#ifndef ADDCOMBOBOXVALUECOMMAND_H
#define ADDCOMBOBOXVALUECOMMAND_H

#include <QUndoCommand>
#include <QComboBox>
#include "mycombobox.h"

class AddComboBoxValueCommand : public QUndoCommand
{
public:
    AddComboBoxValueCommand(MyComboBox *_comboBox, int _comboIndexOldValue, QUndoCommand *parent = 0);
    virtual ~AddComboBoxValueCommand();

    void undo() override;
    void redo() override;

    private:
    MyComboBox* comboBox;
    int comboIndexOldValue;
    int comboIndexNewValue;
    bool programmatically;
};

QString createComboBoxCommandString(int index);

#endif //ADDOMBOBOXVALUECOMMAND_H
