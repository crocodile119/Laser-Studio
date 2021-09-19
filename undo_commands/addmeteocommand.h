#ifndef ADDMETEOCOMMAND_H
#define ADDMETEOCOMMAND_H

#include <QUndoCommand>
#include "../centralwidget.h"

class AddMeteoCommand : public QUndoCommand
{
public:
    AddMeteoCommand(CentralWidget *_laserWindow, double _wavelength,  double _A, double _V,
                    double _atmAttCoeff, QUndoCommand *parent = nullptr);

    ~AddMeteoCommand();

    void undo() override;
    void redo() override;

private:
CentralWidget *laserWindow;
double wavelength;
double A;
double V;
double atmAttCoeff;

double old_A;
double old_V;
double old_atmAttCoeff;

double old_wavelength;
};


#endif
