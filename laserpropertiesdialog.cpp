#include <QtGui>
#include <QColorDialog>

#include "laserpropertiesdialog.h"

LaserPropertiesDialog::LaserPropertiesDialog(LaserPoint *laserpoint, QWidget *parent)
    : QDialog(parent), laserpoint(laserpoint)
{
    setupUi(this);

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
    laserpoint->computePillowAreas();
    laserpoint->setFilterOn(filterCheckBox->checkState());
    laserpoint->setTrasmittance(transmittanceSpinBox->value());
    laserpoint->update();

    QDialog::accept();
}

void LaserPropertiesDialog::on_filterCheckBox_stateChanged(int arg1)
{
    transmittanceSpinBox->setEnabled(arg1);
}

void LaserPropertiesDialog::on_installationComboBox_currentIndexChanged(int index)
{
    pillowLabel->setText(QString::number(computePillowAreas(index)));
}

double LaserPropertiesDialog::computePillowAreas(const int & index)
{
    double pillow;
    switch(index)
    {
       case 0 :
       pillow= 0.11465;
       break;

       case 1 :
       pillow= 0.286624;
       break;

       case 2 :
       pillow= 0.573248;
       break;

       case 3 :
       pillow= 1.714795;
       break;

       default:
       pillow= 1.714795;
       break;
    }
    return pillow;
}
