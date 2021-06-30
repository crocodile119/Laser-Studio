#include "combotypecontrol.h"
#include <cmath>
#include <QDebug>

ComboTypeControl::ComboTypeControl(QWidget *parent)
    : QWidget(parent)
{

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(tr("gridLayout"));


    comboBox = new QComboBox(this);

    comboBox->addItem(tr("UV"));
    comboBox->addItem(tr("VIS"));
    comboBox->addItem(tr("IRA-NIR"));
    comboBox->addItem(tr("IRB-SWIR"));
    comboBox->addItem(tr("IRC-MWIR"));
    comboBox->addItem(tr("IRC-LWIR"));
    comboBox->addItem(tr("IRC-FIR"));

    comboBox->setObjectName(tr("comboBOx"));
    comboBox->setMinimumSize(QSize(0, 20));

    gridLayout->addWidget(comboBox, 0, 1, 1, 1);

    comboBox->setCurrentIndex(1);


 }
ComboTypeControl::~ComboTypeControl()
{

}

void ComboTypeControl::on_comboBox_currentIndexChanged(int index)
{
    comboBoxIndex= index;

        QUndoCommand *scrollCommand //= new AddScrollBarValueCommand(scrollBar, scrollBarOldValue, titleLabel->text(),
                                                              AddScrollBarValueCommand::movement::TRIGGER_ACTION);
        undoStack->push(scrollCommand);
}

void ComboTypeControl::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(on_undoStack_indexChanged()));
}

void ComboTypeControl::setScrollBarInitialValue()
{
    scrollBarOldValue=scrollBar->value();
}

void ComboTypeControl::on_undoStack_indexChanged()
{
    scrollBarOldValue=scrollBar->sliderPosition();
}
