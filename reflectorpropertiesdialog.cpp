#include <QtGui>
#include <QColorDialog>

#include "reflectorpropertiesdialog.h"

ReflectorPropertiesDialog::ReflectorPropertiesDialog(Reflector *_reflector, QWidget *parent)
    : QDialog(parent), ui(new Ui::ReflectorPropertiesDialog), reflector(_reflector)
{
    ui->setupUi(this);

    ui->xSpinBox->setValue(reflector->x());
    ui->ySpinBox->setValue(reflector->y());
    ui->reflectionSpinBox->setValue(reflector->getMaterialCoeff());
    ui->reflectorKindLabel->setText(reflector->getReflectorKindString());
    ui->positioningSlider->setValue(reflector->getPositioning());
    ui->laserPhaseLabel->setText(QString::number(reflector->getLaserPhase(), 'f', 0));
    ui->actualPositioningLabel->setText(QString::number(reflector->getCorrectPositioning(), 'f', 0));

    qDebug()<<"Fase laser: "<<reflector->getLaserPhase();
    ui->descriptionTextEdit->setText(reflector->getDescription());

    if(this->reflector->getReflectorKind()==MIRROR_TARGET)
    {
        ui->positioningSlider->setVisible(false);
        ui->laserPhaseLabel->setVisible(false);
        ui->actualPositioningLabel->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->positioningLabel->setVisible(false);
    }

    if(this->reflector->getReflectorKind()==GLASS_TARGET)
        ui->label_5->setText(tr("τ"));
    else
        ui->label_5->setText(tr("ρ"));
}

void ReflectorPropertiesDialog::on_positioningSlider_valueChanged(int value)
{
    ui->positioningLabel->setText(QString::number(value));

    if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        ui->actualPositioningLabel->setText(QString::number(value+reflector->getLaserPhase(), 'f', 0));
    else
        ui->actualPositioningLabel->setText(QString::number(2*(value+reflector->getLaserPhase()), 'f', 0));
}

ReflectorPropertiesDialog::~ReflectorPropertiesDialog()
{
    delete ui;
}
