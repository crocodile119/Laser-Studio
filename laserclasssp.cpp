#include "laserclasssp.h"

LaserClassSP::LaserClassSP(double _beamDiameter, double _powerErg,  double _divergence,
                           double _wavelength, double _pulseWidth, double _alpha):LaserClassCW(_beamDiameter, _powerErg, _divergence,
                                                                        _wavelength, _pulseWidth, _alpha)
{
    powerErgEq=new double[n_lea];
    powerErg_Cond_1=new double[n_lea];
    powerErg_Cond_3=new double[n_lea];
    classValutation=new bool[n_class];

    updateAll();
}

void LaserClassSP::updateAll()
{
    //Calcolo i valori di LEA
    myLaserClass.calculate();
    computeBeamArea();
    classUpdate(PULSED, pulseWidth, powerErg);
}

void LaserClassSP::setPulseWidth(const double _pulseWidth)
{
    if(_pulseWidth==pulseWidth)
        return;

    myLaserClass.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}

double LaserClassSP::getPulseWidth() const
{
    return pulseWidth;
}
