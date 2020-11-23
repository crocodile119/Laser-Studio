#include <cmath>
#include <string>
#include "lasersafetycw.h"


using namespace std;

LaserSafetyCW::LaserSafetyCW(double _BeamDiameter, double _PowerErg,  double _Divergence, double _Wavelength, 
						double _ExposureTime, double _Alpha):LaserSafety(_BeamDiameter, _PowerErg, _Divergence, _Wavelength, 
						_ExposureTime, _Alpha)
{
			
BeamDiameter=_BeamDiameter; 
PowerErg=_PowerErg;
Divergence=_Divergence;
Wavelength=_Wavelength;
Alpha=_Alpha;
timeEditable=false;

setExposureTime();
MyLaser.setWavelength(Wavelength);
MyLaser.setAlpha(Alpha);
MyLaser.setPulseDuration(getExposureTime());
MyLaser.EMP();
MyLaser.getEMP();

computeNOHD();
computeLambertianMax();
}

void LaserSafetyCW::computeNOHD()
{
		double underroot;
		
        underroot=(4*getPowerErgForEMP())/(PI*EMP_Result);
		NOHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
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
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
        powerErgForEMP=PowerErg;
            else
        powerErgForEMP=PowerErg*exposureTime;

    return powerErgForEMP;
}

/*Calcolo la distanza di sicurezza ottica per effetto della riflessione su un riflettore
 * lambertiano. Il calcolo fa riferimento al caso di riflessione puntiforme con direzione
 * normale al diffusore e coeficiente di diffusione unitario. */

void LaserSafetyCW::computeLambertianMax()
{
    double ForLambertianMax;
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
        ForLambertianMax=PowerErg;
            else
        ForLambertianMax=PowerErg*exposureTime;

        double underroot;

        underroot=(ForLambertianMax)/(PI*EMP_Result);
        lambertianMax=(sqrt(underroot));
}

void LaserSafetyCW::laserUpdate()
{  
MyLaser.setWavelength(Wavelength);
MyLaser.setAlpha(Alpha);
MyLaser.setPulseDuration(getExposureTime());
MyLaser.EMP();
EMP_Result=getEMP();
computeNOHD();
computeLambertianMax();
}

void LaserSafetyCW::setExposureTime()
{
    if(!timeEditable)
    {
        if ((Wavelength>=400) and (Wavelength<=700)){
        exposureTime=0.25;}
        else if((Wavelength>=180)and (Wavelength<400)){
         exposureTime=30000;}
         else{
                exposureTime=10;}
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
