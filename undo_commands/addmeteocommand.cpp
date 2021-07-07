#include "addmeteocommand.h"
#include <cmath>

AddMeteoCommand::AddMeteoCommand(CentralWidget *_laserWindow,  double _wavelength, double _A, double _V,
                 double _atmAttCoeff, EnvironmentListModel *_environmentModel,
                 QUndoCommand *parent):QUndoCommand(parent), laserWindow(_laserWindow), wavelength(_wavelength),
                  A(_A), V(_V), atmAttCoeff(_atmAttCoeff), environmentModel(_environmentModel)
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
    environmentModel->setMeteoVisibility(laserWindow->getMeteoRange());
    environmentModel->myDataHasChanged();

    setText(QObject::tr("Imposto la visibilità atmosferica a %1 [m]")
            .arg(QString::number(V)));
}

void AddMeteoCommand::redo()
{
    laserWindow->setAtmoshericEffectsCoefficient(atmAttCoeff);
    laserWindow->setA_Coefficient(A);
    laserWindow->setMeteoRange(V);
    environmentModel->setMeteoVisibility(laserWindow->getMeteoRange());
    environmentModel->myDataHasChanged();

    setText(QObject::tr("Imposto la visibilità atmosferica a %1 [m]")
            .arg(QString::number(V)));
}

AddMeteoCommand::~AddMeteoCommand()
{

}

