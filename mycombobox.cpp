#include "mycombobox.h"

MyComboBox::MyComboBox(QWidget *parent):QComboBox(parent)
{
    programmatically=false;
}

void MyComboBox::setProgrammaticallyOperation(bool _operation)
{
    programmatically=_operation;
}

bool MyComboBox::isProgrammaticallyOperation()
{
    return programmatically;
}
