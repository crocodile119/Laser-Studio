#include <QtGui>
#include <QColorDialog>

#include "atmosphericeffectsdialog.h"

AtmosphericEffectsDialog::AtmosphericEffectsDialog(QWidget *parent, CentralWidget *_laserWindow, double _wavelength)
    : QDialog(parent), ui(new Ui::AtmosphericEffectsDialog), wavelength(_wavelength), laserWindow(_laserWindow)
{
    ui->setupUi(this);

    /*V è la distanza di visibilità espressa in metri
    la slider riporta la distanza di visibilità in km V/1000 */

    A=laserWindow->getA_Coefficient();
    V=laserWindow->getMeteoRange();
    atmAttCoeff=3.91/V*powf(550/wavelength, A);
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
    A=0.06*powf(V, 0.33);

    ui->meteoRangeLabel->setText(QString::number(V_km));
    ui->A_Label->setText(QString::number(A, 'e', 2));
    atmAttCoeff=3.91/(V)*powf(550.0/(wavelength), A);
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
