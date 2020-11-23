#include <cmath>
#include <string>
#include "laserskinsafety.h"


using namespace std;

const double LaserSkinSafety::PI = 3.141592653589793;
		
LaserSkinSafety::LaserSkinSafety(double _BeamDiameter, double _PowerErg,  double _Divergence, double _Wavelength,
                        double _PulseWidth, double _Alpha):MySkinLaser(_Wavelength, _PulseWidth, _Alpha)
{
BeamDiameter=_BeamDiameter; 
PowerErg=_PowerErg;
Divergence=_Divergence;
Wavelength=_Wavelength;
PulseWidth=_PulseWidth;

MySkinLaser.setWavelength(_Wavelength);
MySkinLaser.setAlpha(_Alpha);
MySkinLaser.setPulseDuration(getPulseWidth());
MySkinLaser.EMP();
MySkinLaser.adaptForSkinEMP();
MySkinLaser.getEMP();
Formula=MySkinLaser.getFormulaEMP();
FormulaSort=MySkinLaser.getFormulaSort();
CA=MySkinLaser.getCA();
CB=MySkinLaser.getCB();
CC=MySkinLaser.getCC();
CE=MySkinLaser.getCE();
T1=MySkinLaser.getT1();
T2=MySkinLaser.getT2();
Gamma=MySkinLaser.getGamma();
Radiation=MySkinLaser.getRadiation();
SkinDamage=MySkinLaser.getSkinDamage();
EyeDamage=MySkinLaser.getEyeDamage();
Note=MySkinLaser.getPhotochemicalNote();

computePowerNSHD();
computePowerForExended();

}

void LaserSkinSafety::laserSkinUpdate()
{
MySkinLaser.setWavelength(Wavelength);
MySkinLaser.setAlpha(Alpha);
MySkinLaser.setPulseDuration(getPulseWidth());

MySkinLaser.EMP();
MySkinLaser.adaptForSkinEMP();
EMP_Result=getEMP();
computePowerForExended();
computePowerNSHD();
computeErgNSHD();
}

double LaserSkinSafety::getWavelength()
{
return Wavelength;
}

void LaserSkinSafety::setWavelength(const double _Wavelength)
{
Wavelength=_Wavelength;
}

void LaserSkinSafety::setPowerErg(const double _PowerErg)
{
PowerErg=_PowerErg;
}

double LaserSkinSafety::getPowerErg() const
{
return PowerErg;
}

double LaserSkinSafety::getAlpha() const
{
return Alpha;
}

void LaserSkinSafety::setAlpha(const double _Alpha)
{
Alpha=_Alpha;
}

double LaserSkinSafety::getDivergence() const
{
return Divergence;
}

void LaserSkinSafety::setDivergence(const double _Divergence)
{
Divergence=_Divergence;
}

double LaserSkinSafety::getBeamDiameter() const
{
return BeamDiameter;
}

void LaserSkinSafety::setBeamDiameter(const double _BeamDiameter)
{
BeamDiameter=_BeamDiameter;
}

double LaserSkinSafety::getPowerNSHD() const
{
    return PowerNSHD;
}

double LaserSkinSafety::getErgNSHD() const
{
    return ErgNSHD;
}

void LaserSkinSafety::computeBeamArea()
{
	BeamArea=PI*pow(BeamDiameter,2)/4;
}

double LaserSkinSafety::getBeamArea() const
{
	return BeamArea;
}

double LaserSkinSafety::computePowerNSHD()
{
    double ForNSHD;
    FormulaSort=getFormulaSort();
        if(FormulaSort=="E")
            ForNSHD=PowerErg;
                else
            ForNSHD=PowerErg*exposureTime;

            double underroot;

            underroot=(4*ForNSHD)/(PI*EMP_Result);
            PowerNSHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
            if (PowerNSHD<0)
                PowerNSHD=0;

    return PowerNSHD;
}

double LaserSkinSafety::computeErgNSHD()
{
    double ForNSHD;
	FormulaSort=getFormulaSort();
	
	if(FormulaSort=="E")
        ForNSHD=PowerErg/PulseWidth;
	else
        ForNSHD=PowerErg;

    double underroot;
		
    underroot=(4*ForNSHD)/(PI*EMP_Result);
    ErgNSHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
        if (ErgNSHD<0)
            ErgNSHD=0;

    return ErgNSHD;
}

double LaserSkinSafety::getCA() const
{
    return MySkinLaser.getCA();
}

double LaserSkinSafety::getCB() const
{
    return MySkinLaser.getCB();
}

double LaserSkinSafety::getCC() const
{
    return MySkinLaser.getCC();
}

double LaserSkinSafety::getCE() const
{
    return MySkinLaser.getCE();
}

double LaserSkinSafety::getT1() const
{
    return MySkinLaser.getT1();
}

double LaserSkinSafety::getT2() const
{
    return MySkinLaser.getT2();
}

string LaserSkinSafety::getRadiation() const
{
    return MySkinLaser.getRadiation();
}

string LaserSkinSafety::getSkinDamage() const
{
    return MySkinLaser.getSkinDamage();
}

string LaserSkinSafety::getEyeDamage() const
{
    return MySkinLaser.computeEMP::getEyeDamage();
}

string LaserSkinSafety::getPhotochemicalNote() const
{
    return MySkinLaser.getPhotochemicalNote();
}

double LaserSkinSafety::getGamma() const
{
    return MySkinLaser.getGamma();
}

double LaserSkinSafety::getEMP()
{
    return MySkinLaser.getEMP();
}

string LaserSkinSafety::getFormulaEMP() const
{
    return MySkinLaser.getFormulaEMP();
}

string LaserSkinSafety::getFormulaSort() const
{
    return MySkinLaser.computeEMP::getFormulaSort();
}

void LaserSkinSafety::EMP()
{
    MySkinLaser.EMP();
}

void LaserSkinSafety::setPulseWidth(const double _PulseWidth)
{
	PulseWidth=_PulseWidth;
}

double LaserSkinSafety::getPulseWidth() const
{
	return PulseWidth;
}

double LaserSkinSafety::getPowerForExended() const
{
	return PowerForExended;
}

void LaserSkinSafety::computePowerForExended()
{
	PowerForExended=PowerErg/PulseWidth;
}
