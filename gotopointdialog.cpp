#include <QtGui>
#include <QColorDialog>

#include "gotopointdialog.h"
#include "ui_gotopointdialog.h"

GoToPointDialog::GoToPointDialog(QWidget *parent, QPointF position)
    : QDialog(parent)
{
    setupUi(this);
    center=position;

    xPosition=center.x();
    yPosition=center.y();

    xSpinBox->setValue(xPosition);
    ySpinBox->setValue(yPosition);
}


QPointF GoToPointDialog::getViewCenter()
{
    return center;
}

void GoToPointDialog::setViewCenter(const QPointF _center)
{
    center=_center;
}

void GoToPointDialog::on_xSpinBox_valueChanged(int arg1)
{
    xPosition=static_cast<double>(arg1);
}

void GoToPointDialog::on_ySpinBox_valueChanged(int arg1)
{
    yPosition=static_cast<double>(arg1);
}

void GoToPointDialog::on_buttonBox_accepted()
{
    center=QPointF(xPosition, yPosition);
}
