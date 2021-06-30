#include <QtGui>
#include <QColorDialog>

#include "binocularpropertiesdialog.h"

BinocularPropertiesDialog::BinocularPropertiesDialog(Binocular *_binocular, double _wavelength, QWidget *parent)
    : QDialog(parent), ui(new Ui::BinocularPropertiesDialog), binocular(_binocular), wavelength(_wavelength)
{
    ui->setupUi(this);

    ui->xSpinBox->setValue(int(binocular->x()));
    ui->ySpinBox->setValue(int(binocular->y()));

    ampOpticWavelength=((wavelength>=320)&&(wavelength<=4500));

    ui->ampSpinBox->setValue(int(binocular->getMagnification()));
    ui->transmittanceSpinBox->setValue(binocular->getTransmissionCoeff());

    /****************************************************************************
     * visualizzo la trasmittanza e il valore di M nel caso in cui la lunghezza *
     * d'onda sia compresa tra 320 e 4500 nm.                                   *
     ****************************************************************************/

    ui->transmittanceSpinBox->setVisible(ampOpticWavelength);
    ui->ampSpinBox->setVisible(ampOpticWavelength);
    ui->label_5->setVisible(ampOpticWavelength);
    ui->label_6->setVisible(ampOpticWavelength);

    ui->descriptionTextEdit->setText(binocular->getDescription());

    ui->D0_SpinBox->setValue(binocular->get_D0());

    /******************************************************************************
     * visualizzo il valore del diametro della lente nel caso in cui la lunghezza *
     * d'onda sia compresa tra 400 e 1400 nm.                                     *
     ******************************************************************************/
    thermicWavelength=((wavelength>=400)&&(wavelength<=1400));
    ui->D0_SpinBox->setVisible(thermicWavelength);
    ui->label_4->setVisible(thermicWavelength);
}

