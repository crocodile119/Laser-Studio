#include <QtGui>
#include <QColorDialog>

#include "laserpropertiesdialog.h"

LaserPropertiesDialog::LaserPropertiesDialog(LaserPoint *_laserpoint, bool _dark, QWidget *parent)
    : QDialog(parent), ui(new Ui::LaserPropertiesDialog), laserpoint(_laserpoint), dark(_dark)
{
    ui->setupUi(this);

    ui->xSpinBox->setValue(laserpoint->x());
    ui->ySpinBox->setValue(laserpoint->y());
    ui->areaSpinBox->setValue(laserpoint->getAperture());
    ui->installationComboBox->setCurrentIndex(laserpoint->getInstallationIndex());
    ui->pillowLabel->setText(QString::number(laserpoint->getPillow()));

    setStyleSheet();

    int checkState;
    if(laserpoint->isFilterOn())
        checkState=2;
    else
        checkState=0;

    ui->filterCheckBox->setCheckState(Qt::CheckState(checkState));
    ui->transmittanceSpinBox->setValue(laserpoint->getTransmittance());
    ui->transmittanceSpinBox->setEnabled(ui->filterCheckBox->checkState());
}

void LaserPropertiesDialog::on_filterCheckBox_stateChanged(int arg1)
{
    ui->transmittanceSpinBox->setEnabled(arg1);
}

void LaserPropertiesDialog::on_installationComboBox_currentIndexChanged(int index)
{
    ui->pillowLabel->setText(QString::number(computePillowAreas(index)));
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

void LaserPropertiesDialog::setStyleSheet()
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
                "QLabel {margin-left: 10px}"));
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
                "QLabel {margin-left: 10px}"));
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
LaserPropertiesDialog::~LaserPropertiesDialog()
{
    delete ui;
}
