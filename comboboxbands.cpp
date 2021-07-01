#include "comboboxbands.h"
#include <QDebug>

ComboBoxBands::ComboBoxBands(QWidget *parent)
    : QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(tr("gridLayout"));

    comboBox = new MyComboBox(this);
    comboBox->setObjectName(tr("comboBox"));
    comboBox->setMinimumSize(QSize(0, 25));

    gridLayout->addWidget(comboBox, 0, 0, 1, 1);

    comboBox->addItem(tr("UV"));
    comboBox->addItem(tr("VIS"));
    comboBox->addItem(tr("IRA-NIR"));
    comboBox->addItem(tr("IRB-SWIR"));
    comboBox->addItem(tr("IRC-MWIR"));
    comboBox->addItem(tr("IRC-LWIR"));
    comboBox->addItem(tr("IRC-FIR"));
    comboBox->setCurrentIndex(1);

    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxBands_currentIndexChanged(int)));
}

void ComboBoxBands::on_comboBoxBands_currentIndexChanged(int index)
{
    if(!comboBox->isProgrammaticallyOperation())
    {
        QUndoCommand *comboCommand = new AddComboBoxValueCommand(comboBox, comboBoxOldValue);

        undoStack->push(comboCommand);
        qDebug()<<"Dammi un cenno che ci sei.";
        comboBoxOldValue=index;
    }
    emit currentIndexChanged(index);
}

void ComboBoxBands::setCurrentIndex(const int& index)
{
    comboBox->setCurrentIndex(index);
}

int ComboBoxBands::currentIndex()const
{
    return comboBox->currentIndex();
}

void ComboBoxBands::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
}

void ComboBoxBands::setComboBoxInitialValue()
{
    comboBoxOldValue=comboBox->currentIndex();
}
