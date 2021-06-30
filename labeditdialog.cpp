#include <QtGui>
#include <QColorDialog>

#include "labeditdialog.h"

LabEditDialog::LabEditDialog(LabRoom *myLab, QWidget *parent)
    : QDialog(parent), ui(new Ui::LabEditDialog)
{
    ui->setupUi(this);

    this->myLabRoom=myLab;

    QRectF myLabRect=this->myLabRoom->getRoomRect();
    ui->xSpinBox->setValue(myLabRoom->pos().x());
    ui->ySpinBox->setValue(myLabRoom->pos().y());

    ui->widthSpinBox->setValue(myLabRect.width());
    ui->heightSpinBox->setValue(myLabRect.height());
}

LabEditDialog::~LabEditDialog()
{
    delete ui;
}
