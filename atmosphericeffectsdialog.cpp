#include <QtGui>
#include <QColorDialog>

#include "atmosphericeffectsdialog.h"

AtmosphericEffectsDialog::AtmosphericEffectsDialog(QWidget *parent, CentralWidget *_laserWindow, double _wavelength)
    : QDialog(parent), wavelength(_wavelength), laserWindow(_laserWindow)
{
    setupUi(this);

    A=laserWindow->getA_Coefficient();
    V=laserWindow->getMeteoRange();
    atmAttCoeff=3.91/V*powf(550/wavelength, A);
    int V_int =V/1000;
    A_Label->setText(QString::number(A, 'e', 2));
    meteoRangeSlider->setValue(V_int);
    meteoRangeLabel->setText(QString::number(V_int));
    atmosphericCoeffLabel->setText(QString::number(atmAttCoeff, 'e', 2));
}

void AtmosphericEffectsDialog::on_buttonBox_accepted()
{
    laserWindow->setAtmoshericEffectsCoefficient(atmAttCoeff);
    laserWindow->setA_Coefficient(A);
    laserWindow->setMeteoRange(V*1000);
}

AtmosphericEffectsDialog::~AtmosphericEffectsDialog()
{
}

void AtmosphericEffectsDialog::on_meteoRangeSlider_valueChanged(int value)
{
    V=value;
    A=0.06*powf(1000*V, 0.33);

    meteoRangeLabel->setText(QString::number(V));
    A_Label->setText(QString::number(A, 'e', 2));
    atmAttCoeff=3.91/(1000*V)*powf(550.0/(wavelength), A);
    atmosphericCoeffLabel->setText(QString::number(atmAttCoeff, 'e', 2));
}
