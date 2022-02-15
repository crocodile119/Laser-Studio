#include <QtGui>
#include <QColorDialog>

#include "atmosphericeffectsdialog.h"

AtmosphericEffectsDialog::AtmosphericEffectsDialog(QWidget *parent, CentralWidget *_laserWindow, double _wavelength, bool _dark)
    : QDialog(parent), ui(new Ui::AtmosphericEffectsDialog), wavelength(_wavelength), laserWindow(_laserWindow), dark(_dark)
{
    ui->setupUi(this);

    /*V è la distanza di visibilità espressa in metri
    la slider riporta la distanza di visibilità in km V/1000 */

    setStyleSheet();
    A=laserWindow->getA_Coefficient();
    V=laserWindow->getMeteoRange();
    atmAttCoeff=3.91/V*std::pow(550/wavelength, A);
    int V_km =V/1000;
    ui->A_Label->setText(QString::number(A, 'e', 2));
    ui->meteoRangeSlider->setValue(V_km);
    ui->meteoRangeLabel->setText(QString::number(V_km));
    ui->atmosphericCoeffLabel->setText(QString::number(atmAttCoeff, 'e', 2));
}

AtmosphericEffectsDialog::~AtmosphericEffectsDialog()
{
    delete ui;
}

void AtmosphericEffectsDialog::on_meteoRangeSlider_valueChanged(int value)
{
    int V_km=value;
    V=1000*V_km;
    A=0.06*std::pow(V, 0.33);

    ui->meteoRangeLabel->setText(QString::number(V_km));
    ui->A_Label->setText(QString::number(A, 'e', 2));
    atmAttCoeff=3.91/(V)*std::pow(550.0/(wavelength), A);
    ui->atmosphericCoeffLabel->setText(QString::number(atmAttCoeff, 'e', 2));
}

double AtmosphericEffectsDialog::getAtmAttCoeff()const
{
    return atmAttCoeff;
}

double AtmosphericEffectsDialog::getA()const
{
    return A;
}

double AtmosphericEffectsDialog::getV()const
{
    return V;
}

void AtmosphericEffectsDialog::setStyleSheet()
{
    if(dark)
    {
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #b5922d}"
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
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
    else
    {
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #00c800}"
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
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
}
