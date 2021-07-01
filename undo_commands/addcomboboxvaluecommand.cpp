
#include "addcomboboxvaluecommand.h"
#include <QDebug>

/* Command Stack peril controllo dial con label per visualizzazione input */
AddComboBoxValueCommand::AddComboBoxValueCommand(MyComboBox *_comboBox, int _comboIndexOldValue, QUndoCommand *parent):QUndoCommand(parent),
    comboBox(_comboBox), comboIndexOldValue(_comboIndexOldValue)

{
    comboIndexNewValue=comboBox->currentIndex();
    programmatically=false;
}

void AddComboBoxValueCommand::redo()
{
    if(programmatically)
    {
        comboBox->setProgrammaticallyOperation(true);
        comboBox->setCurrentIndex(comboIndexNewValue);
        comboBox->setProgrammaticallyOperation(false);
    }
    else
        comboBox->setCurrentIndex(comboIndexNewValue);

    setText(QString("Cambio tipo di laser λ[nm]: %1")
           .arg(createComboBoxCommandString(comboIndexNewValue)));

    programmatically=true;
}

void AddComboBoxValueCommand::undo()
{
    comboBox->setProgrammaticallyOperation(true);
    comboBox->setCurrentIndex(comboIndexOldValue);
    comboBox->setProgrammaticallyOperation(false);
    comboBox->blockSignals(false);
    setText(QString("Cambio tipo di laser λ[nm]: %1")
           .arg(createComboBoxCommandString(comboIndexNewValue)));

}

QString createComboBoxCommandString(int index)
{
    QString wavelengthString;
    if(index==0)
        wavelengthString="380 (UV)";
    else if(index==1)
        wavelengthString="633 (VIS)";
    else if(index==2)
        wavelengthString="1064 (IRA-NIR)";
    else if(index==3)
        wavelengthString="1583 (IRB-SWIR)";
    else if(index==4)
        wavelengthString="3391 (IRC-MWIR)";
    else if(index==5)
        wavelengthString="10600 (IRC-LWIR)";
    else if(index==6)
        wavelengthString="571699 (IRC-FIR";

    return QObject::tr("%1")
        .arg(wavelengthString);
}

AddComboBoxValueCommand::~AddComboBoxValueCommand()
{

}
