#include <QtGui>
#include <QColorDialog>

#include "footprintdialog.h"

FootprintDialog::FootprintDialog(FootprintObject *_footprint, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    myFootprint=_footprint;

    auto myFootprintRectangle=myFootprint->getRectangle();
    QRectF myRect=myFootprintRectangle.rect();
    QPointF myFootprintCenter= myRect.topLeft();
    QPointF center =myFootprint->mapToScene(myFootprintCenter);
    xSpinBox->setValue(center.x());
    ySpinBox->setValue(center.y());

    widthSpinBox->setValue(myRect.width());
    heightSpinBox->setValue(myRect.height());
    descriptionTextEdit->setText(myFootprint->getDescription());
}

void FootprintDialog::on_buttonBox_accepted()
{
    QRectF myFootprintRect=QRectF(0, 0, widthSpinBox->value(), heightSpinBox->value());
    //QPointF center= QPointF(widthSpinBox->value()/2, heightSpinBox->value()/2);
    //myFootprintRect.translate(-center);

    auto myRectangle=myFootprint->getRectangle();
    myRectangle.setRect(myFootprintRect);

    myFootprint->setRectangle(myRectangle);
    myFootprint->setPos(xSpinBox->value(), ySpinBox->value());
    myFootprint->setDescription(descriptionTextEdit->toPlainText());
    myFootprint->updateTipString();
    QDialog::accept();
}

FootprintDialog::~FootprintDialog()
{
}
