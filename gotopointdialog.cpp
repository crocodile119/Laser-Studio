#include <QtGui>
#include <QColorDialog>

#include "gotopointdialog.h"
#include "ui_gotopointdialog.h"

GoToPointDialog::GoToPointDialog(QWidget *parent,  bool _dark)
    : QDialog(parent), dark(_dark), xPosition(0), yPosition(0)
{
    setupUi(this);
    setStyleSheet();
}

QPointF GoToPointDialog::getViewCenter()
{
    return center;
}

void GoToPointDialog::setViewCenter()
{
    center=QPointF(xPosition, yPosition);
}

void GoToPointDialog::on_xSpinBox_valueChanged(int arg1)
{
    xPosition=static_cast<double>(arg1);
    setViewCenter();
}

void GoToPointDialog::on_ySpinBox_valueChanged(int arg1)
{
    yPosition=static_cast<double>(arg1);
    setViewCenter();
}

void GoToPointDialog::setStyleSheet()
{
    if(dark)
    {
        label->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        label_2->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
    else
    {
        label->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        label_2->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
}
