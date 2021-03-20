#include <cmath>
#include <string>
#include "lasersafetycw.h"

using namespace std;

const double LaserSafetyCW::POWER=1.0;

LaserSafetyCW::LaserSafetyCW(double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                        double _exposureTime, double _alpha):LaserSafety(_beamDiameter, _powerErg, _divergence,
                        _wavelength, _exposureTime, _alpha)
{
    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setAlpha(_alpha);

    timeEditable=false;

    setExposureTime();
    myLaser.EMP();
    myLaser.getEMP();

    computeNOHD();
    computeLambertianMax();
}

void LaserSafetyCW::computeNOHD()
{
    double underroot;
		
    underroot=(4*getPowerErgForEMP())/(PI*EMP_Result);
    NOHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
    if (NOHD<0)
        NOHD=0;
}

/*****************************************************************************************************************
 * Calcolo il valore dell'energia emessa dal laser a partire dalla potenza se l'EMP è espresso in Esposizione    *
 * radiante lascio invariata la potenza se l'EMP è espresso in irradianza                                        *
 * ***************************************************************************************************************/

double LaserSafetyCW::getPowerErgForEMP()
{
    double powerErgForEMP;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
        powerErgForEMP=powerErg;
            else
        powerErgForEMP=powerErg*exposureTime;

    return powerErgForEMP;
}

/*Calcolo la distanza di sicurezza ottica per effetto della riflessione su un riflettore
 * lambertiano. Il calcolo fa riferimento al caso di riflessione puntiforme con direzione
 * normale al diffusore e coeficiente di diffusione unitario. */

void LaserSafetyCW::computeLambertianMax()
{
    double ForLambertianMax;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
        ForLambertianMax=powerErg;
            else
        ForLambertianMax=powerErg*exposureTime;

    double underroot;

    underroot=(ForLambertianMax)/(PI*EMP_Result);
    lambertianMax=(sqrt(underroot));
}

void LaserSafetyCW::laserUpdate()
{  
    myLaser.EMP();
    EMP_Result=getEMP();
    computeNOHD();
    computeLambertianMax();
}

void LaserSafetyCW::setExposureTime()
{
    if(!timeEditable)
    {
        if ((wavelength>=400) and (wavelength<=700))
        {
        exposureTime=0.25;
        }
        else if((wavelength>=180)and (wavelength<400))
        {
        exposureTime=30000;
        }
        else
        {
        exposureTime=10;
        }
    }
}

double LaserSafetyCW::getExposureTime() const
{
    return exposureTime;
}

void LaserSafetyCW::setExposureTimeEditable(bool _timeEditable)
{
    timeEditable=_timeEditable;

}

bool LaserSafetyCW::isTimeEditable()
{
    return timeEditable;
}

void LaserSafetyCW::setEditedExposureTime(const double &_exposureTime)
{
    exposureTime=_exposureTime;
}
