#ifndef ADDMETEOCOMMAND_H
#define ADDMETEOCOMMAND_H

#include <QUndoCommand>
#include "../centralwidget.h"
#include "../environmentlistmodel.h"

class AddMeteoCommand : public QUndoCommand
{
public:
    AddMeteoCommand(CentralWidget *_laserWindow, double _wavelength,  double _A, double _V,
                    double _atmAttCoeff,  EnvironmentListModel *_environmentModel,
                    QUndoCommand *parent = nullptr);

    ~AddMeteoCommand();

    void undo() override;
    void redo() override;

private:
CentralWidget *laserWindow;
double wavelength;
double A;
double V;
double atmAttCoeff;
EnvironmentListModel *environmentModel;

double old_A;
double old_V;
double old_atmAttCoeff;

double old_wavelength;
};


#endif
