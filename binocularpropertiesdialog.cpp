#include <QtGui>
#include <QColorDialog>

#include "binocularpropertiesdialog.h"

BinocularPropertiesDialog::BinocularPropertiesDialog(Binocular *_binocular, double _wavelength, bool _dark, QWidget *parent)
    : QDialog(parent), ui(new Ui::BinocularPropertiesDialog), binocular(_binocular), wavelength(_wavelength),dark(_dark)
{
    ui->setupUi(this);

    setStyleSheet();

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

void BinocularPropertiesDialog::setStyleSheet()
{
    if(dark)
    {
        ui->label->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_2->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_3->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-right: 10px}"));
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_6->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
    else
    {
        ui->label->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_2->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_3->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-right: 10px}"));
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_6->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
}
