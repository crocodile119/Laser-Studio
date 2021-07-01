#include "addmeteocommand.h"
#include <cmath>

AddMeteoCommand::AddMeteoCommand(CentralWidget *_laserWindow,  double _wavelength, double _A, double _V, double _atmAttCoeff,
                 QUndoCommand *parent):QUndoCommand(parent), laserWindow(_laserWindow), wavelength(_wavelength),
                  A(_A), V(_V), atmAttCoeff(_atmAttCoeff)
{
    old_A=laserWindow->getAtmoshericEffectsCoefficient();
    old_V=laserWindow->getMeteoRange();
    old_wavelength=laserWindow->myDockControls->getWavelength();
    old_atmAttCoeff=3.91/(old_V)*powf(550.0/(old_wavelength), old_A);
}

void AddMeteoCommand::undo()
{
    laserWindow->setAtmoshericEffectsCoefficient(old_atmAttCoeff);
    laserWindow->setA_Coefficient(old_A);
    laserWindow->setMeteoRange(old_V);

    setText(QObject::tr("Imposto la visibilità atmosferica a %1 [m]")
            .arg(QString::number(V)));
}

void AddMeteoCommand::redo()
{
    laserWindow->setAtmoshericEffectsCoefficient(atmAttCoeff);
    laserWindow->setA_Coefficient(A);
    laserWindow->setMeteoRange(V);

    setText(QObject::tr("Imposto la visibilità atmosferica a %1 [m]")
            .arg(QString::number(V)));
}

AddMeteoCommand::~AddMeteoCommand()
{

}

