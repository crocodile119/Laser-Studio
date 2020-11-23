#include <cmath>
#include <string>
#include "lasersafety.h"


using namespace std;

const double LaserSafety::PI = 3.141592653589793;
		
LaserSafety::LaserSafety(double _BeamDiameter, double _PowerErg,  double _Divergence, double _Wavelength, 
						double _PulseWidth, double _Alpha):MyLaser(_Wavelength, _PulseWidth, _Alpha)
{
BeamDiameter=_BeamDiameter; 
PowerErg=_PowerErg;
Divergence=_Divergence;
Wavelength=_Wavelength;
PulseWidth=_PulseWidth;

MyLaser.setWavelength(Wavelength);
MyLaser.setAlpha(_Alpha);
MyLaser.setPulseDuration(PulseWidth);
MyLaser.EMP();
MyLaser.getEMP();
Formula=MyLaser.getFormulaEMP();
FormulaSort=MyLaser.getFormulaSort();
CA=MyLaser.getCA();
CB=MyLaser.getCB();
CC=MyLaser.getCC();
CE=MyLaser.getCE();
T1=MyLaser.getT1();
T2=MyLaser.getT2();
Gamma=MyLaser.getGamma();
Radiation=MyLaser.getRadiation();
SkinDamage=MyLaser.getSkinDamage();
EyeDamage=MyLaser.getEyeDamage();
Note=MyLaser.getPhotochemicalNote();

computeNOHD();
computeLambertianMax();
computePowerForExended();

}

void LaserSafety::laserUpdate()
{
MyLaser.setWavelength(Wavelength);
MyLaser.setAlpha(Alpha);
MyLaser.setPulseDuration(getPulseWidth());
MyLaser.EMP();
EMP_Result=getEMP();
computePowerForExended();
computeNOHD();
computeLambertianMax();
}

double LaserSafety::getWavelength()
{
return Wavelength;
}

void LaserSafety::setWavelength(const double _Wavelength)
{
Wavelength=_Wavelength;
}

void LaserSafety::setPowerErg(const double _PowerErg)
{
PowerErg=_PowerErg;
}

double LaserSafety::getPowerErg() const
{
return PowerErg;
}

double LaserSafety::getAlpha() const
{
return Alpha;
}

void LaserSafety::setAlpha(const double _Alpha)
{
Alpha=_Alpha;
}

double LaserSafety::getDivergence() const
{
return Divergence;
}

void LaserSafety::setDivergence(const double _Divergence)
{
Divergence=_Divergence;
}

double LaserSafety::getBeamDiameter() const
{
return BeamDiameter;
}

void LaserSafety::setBeamDiameter(const double _BeamDiameter)
{
BeamDiameter=_BeamDiameter;
}

double LaserSafety::getNOHD() const
{
	return NOHD;
}

double LaserSafety::getLambertianMax() const
{
    return lambertianMax;
}

void LaserSafety::computeBeamArea()
{
    BeamArea=PI*pow(BeamDiameter*1.0e-03, 2)/4;
}

double LaserSafety::getBeamArea() const
{
	return BeamArea;
}

void LaserSafety::computeNOHD()
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

double LaserSafety::getPowerErgForEMP()
{
    double powerErgForEMP;
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
        powerErgForEMP=PowerErg/PulseWidth;
    else
        powerErgForEMP=PowerErg;

    return powerErgForEMP;
}

/*Calcolo la distanza di sicurezza ottica per effetto della riflessione su un riflettore
 * lambertiano. Il calcolo fa riferimento al caso di riflessione puntiforme con direzione
 * normale al diffusore e coeficiente di diffusione unitario. */

void LaserSafety::computeLambertianMax()
{
    double ForLambertianMax;
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
        ForLambertianMax=PowerErg/PulseWidth;
    else
        ForLambertianMax=PowerErg;

    double underroot;

    underroot=(ForLambertianMax)/(PI*EMP_Result);
    lambertianMax=(sqrt(underroot));
}

double LaserSafety::getCA() const
{
	return MyLaser.getCA();
}

double LaserSafety::getCB() const
{
	return MyLaser.getCB();
}

double LaserSafety::getCC() const
{
	return MyLaser.getCC();
}

double LaserSafety::getCE() const
{
	return MyLaser.getCE();
}

double LaserSafety::getT1() const
{
	return MyLaser.getT1();
}

double LaserSafety::getT2() const
{
	return MyLaser.getT2();
}

string LaserSafety::getRadiation() const
{
	return MyLaser.getRadiation();
}

string LaserSafety::getSkinDamage() const
{
	return MyLaser.getSkinDamage();
}

string LaserSafety::getEyeDamage() const
{
	return MyLaser.computeEMP::getEyeDamage();
}

string LaserSafety::getPhotochemicalNote() const
{
	return MyLaser.getPhotochemicalNote();
}

double LaserSafety::getGamma() const
{
	return MyLaser.getGamma();
}

double LaserSafety::getEMP()
{
	return MyLaser.getEMP();
}

string LaserSafety::getFormulaEMP() const
{    
    qDebug()<<"formula: "<<QString::fromStdString(MyLaser.getFormulaEMP());
	return MyLaser.getFormulaEMP();
}

string LaserSafety::getFormulaSort() const
{
	return MyLaser.computeEMP::getFormulaSort();
}

void LaserSafety::EMP()
{
	MyLaser.EMP();
}

void LaserSafety::setPulseWidth(const double _PulseWidth)
{
	PulseWidth=_PulseWidth;
}

double LaserSafety::getPulseWidth() const
{
	return PulseWidth;
}

double LaserSafety::getPowerForExended() const
{
	return PowerForExended;
}

void LaserSafety::computePowerForExended()
{
	PowerForExended=PowerErg/PulseWidth;
}
