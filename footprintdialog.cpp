#include <QtGui>
#include <QColorDialog>

#include "footprintdialog.h"

FootprintDialog::FootprintDialog(FootprintObject *_footprint, QWidget *parent)
    : QDialog(parent), ui(new Ui::FootprintDialog), myFootprint(_footprint)
{
    ui->setupUi(this);

    auto myFootprintRectangle=myFootprint->getRectangle();
    QRectF myRect=myFootprintRectangle.rect();
    QPointF myFootprintCenter= myRect.topLeft();
    QPointF center =myFootprint->mapToScene(myFootprintCenter);
    ui->xSpinBox->setValue(center.x());
    ui->ySpinBox->setValue(center.y());

    ui->widthSpinBox->setValue(myRect.width());
    ui->heightSpinBox->setValue(myRect.height());
    ui->descriptionTextEdit->setText(myFootprint->getDescription());
}

FootprintDialog::~FootprintDialog()
{
}
