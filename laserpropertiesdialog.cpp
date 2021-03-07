#include <QtGui>
#include <QColorDialog>

#include "laserpropertiesdialog.h"

LaserPropertiesDialog::LaserPropertiesDialog(LaserPoint *laserpoint, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->laserpoint = laserpoint;
    xSpinBox->setValue(int(laserpoint->x()));
    ySpinBox->setValue(int(laserpoint->y()));
    areaSpinBox->setValue(laserpoint->getAperture());
    installationComboBox->setCurrentIndex(laserpoint->getInstallationIndex());
    pillowLabel->setText(QString::number(laserpoint->getPillow()));

    int checkState;
    if(laserpoint->isFilterOn())
        checkState=2;
    else
        checkState=0;

    filterCheckBox->setCheckState(Qt::CheckState(checkState));
    transmittanceSpinBox->setValue(laserpoint->getTransmittance());
    transmittanceSpinBox->setEnabled(filterCheckBox->checkState());

}

void LaserPropertiesDialog::on_buttonBox_accepted()
{
    laserpoint->setPos(xSpinBox->value(), ySpinBox->value());
    laserpoint->setInstallationIndex(installationComboBox->currentIndex());
    laserpoint->setAperture(areaSpinBox->value());
    laserpoint->setFilterOn(filterCheckBox->checkState());
    laserpoint->setTrasmittance(transmittanceSpinBox->value());
    laserpoint->computePillowAreas();
    laserpoint->update();

    QDialog::accept();
}

void LaserPropertiesDialog::on_filterCheckBox_stateChanged(int arg1)
{
    transmittanceSpinBox->setEnabled(arg1);
}

void LaserPropertiesDialog::on_installationComboBox_currentIndexChanged(int index)
{
    laserpoint->setInstallationIndex(index);
    laserpoint->computePillowAreas();
    pillowLabel->setText(QString::number(laserpoint->getPillow()));
}
