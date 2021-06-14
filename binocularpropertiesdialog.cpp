#include <QtGui>
#include <QColorDialog>

#include "binocularpropertiesdialog.h"

BinocularPropertiesDialog::BinocularPropertiesDialog(Binocular *_binocular, double _wavelength, QWidget *parent)
    : QDialog(parent), binocular(_binocular), wavelength(_wavelength)
{
    setupUi(this);

    xSpinBox->setValue(int(binocular->x()));
    ySpinBox->setValue(int(binocular->y()));

    ampOpticWavelenght=((wavelength>=320)&&(wavelength<=4500));

    ampSpinBox->setValue(int(binocular->getMagnification()));
    transmittanceSpinBox->setValue(binocular->getTransmissionCoeff());

    /****************************************************************************
     * visualizzo la trasmittanza e il valore di M nel caso in cui la lunghezza *
     * d'onda sia compresa tra 320 e 4500 nm.                                   *
     ****************************************************************************/

    transmittanceSpinBox->setVisible(ampOpticWavelenght);
    ampSpinBox->setVisible(ampOpticWavelenght);
    label_5->setVisible(ampOpticWavelenght);
    label_6->setVisible(ampOpticWavelenght);

    descriptionTextEdit->setText(binocular->getDescription());

    D0_SpinBox->setValue(binocular->get_D0());

    /******************************************************************************
     * visualizzo il valore del diametro della lente nel caso in cui la lunghezza *
     * d'onda sia compresa tra 400 e 1400 nm.                                     *
     ******************************************************************************/
    thermicWavelength=((wavelength>=400)&&(wavelength<=1400));
    D0_SpinBox->setVisible(thermicWavelength);
    label_4->setVisible(thermicWavelength);
}

void BinocularPropertiesDialog::on_buttonBox_accepted()
{
    binocular->setPos(xSpinBox->value(), ySpinBox->value());

    if(ampOpticWavelenght)
    {
    binocular->setMagnification(ampSpinBox->value());
    binocular->setTransmissionCoeff(transmittanceSpinBox->value());
    }


    binocular->setDescription(descriptionTextEdit->toPlainText());

    if(thermicWavelength)
        binocular->set_D0(D0_SpinBox->value());

    binocular->setStringPosition();
    binocular->update();
    QDialog::accept();
}
