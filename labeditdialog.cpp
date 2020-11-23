#include <QtGui>
#include <QColorDialog>

#include "labeditdialog.h"

LabEditDialog::LabEditDialog(LabRoom *myLab, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->myLabRoom=myLab;

    QRectF myLabRect=this->myLabRoom->getRoomRect();
    xSpinBox->setValue(myLabRoom->pos().x());
    ySpinBox->setValue(myLabRoom->pos().y());

    widthSpinBox->setValue(myLabRect.width());
    heightSpinBox->setValue(myLabRect.height());
}

void LabEditDialog::on_buttonBox_accepted()
{
    QRectF labRect=QRectF(0, 0, widthSpinBox->value(), heightSpinBox->value());
    QPointF center= QPointF(widthSpinBox->value()/2, heightSpinBox->value()/2);
    labRect.translate(-center);

    QPointF labRectPos=QPointF(xSpinBox->value(),
                               ySpinBox->value());

    myLabRoom->setRoomRect(labRect);
    myLabRoom->setPos(labRectPos);

    QDialog::accept();
}

LabEditDialog::~LabEditDialog()
{
}
