#include <QtGui>
#include <QColorDialog>

#include "reflectorpropertiesdialog.h"

ReflectorPropertiesDialog::ReflectorPropertiesDialog(Reflector *reflector, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->reflector = reflector;
    xSpinBox->setValue(reflector->x());
    ySpinBox->setValue(reflector->y());
    reflectionSpinBox->setValue(reflector->getMaterialCoeff());
    reflectorKindLabel->setText(reflector->getReflectorKindString());
    positioningSlider->setValue(reflector->getPositioning());
    laserPhaseLabel->setText(QString::number(reflector->getLaserPhase(), 'f', 0));
    actualPositioningLabel->setText(QString::number(reflector->getCorrectPositioning(), 'f', 0));

    qDebug()<<"Fase laser: "<<reflector->getLaserPhase();
    descriptionTextEdit->setText(reflector->getDescription());

    if(this->reflector->getReflectorKind()==MIRROR_TARGET)
    {
        positioningSlider->setVisible(false);
        laserPhaseLabel->setVisible(false);
        actualPositioningLabel->setVisible(false);
        label_4->setVisible(false);
        label_7->setVisible(false);
        label_8->setVisible(false);
        positioningLabel->setVisible(false);
    }

    if(this->reflector->getReflectorKind()==GLASS_TARGET)
    label_5->setText(tr("τ"));
    else
    label_5->setText(tr("ρ"));
}

void ReflectorPropertiesDialog::on_buttonBox_accepted()
{
    reflector->setPos(xSpinBox->value(), ySpinBox->value());
    reflector->setMaterialCoeff(reflectionSpinBox->value());
    reflector->setPositioning(positioningLabel->text().toInt());
    reflector->reflectorOperation();
    reflector->setStringDetails();
    reflector->setDescription(descriptionTextEdit->toPlainText());
    reflector->laserParametersChanged();
    QDialog::accept();
}

void ReflectorPropertiesDialog::on_positioningSlider_valueChanged(int value)
{
     positioningLabel->setText(QString::number(value));
     actualPositioningLabel->setText(QString::number(2*(value+reflector->getLaserPhase()), 'f', 0));
}
