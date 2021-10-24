#include <cmath>
#include <string>
#include "laserskinsafety.h"

using namespace std;

const double LaserSkinSafety::PI = 3.141592653589793;
const double LaserSkinSafety::PUPIL_DIAMETER=7.0;
const double LaserSkinSafety::POWER_ENERGY=1.0e-01;
const double LaserSkinSafety::DIVERGENCE=1.5;
const double LaserSkinSafety::EXPOSURE_TIME=5;
		
LaserSkinSafety::LaserSkinSafety(double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                        double _pulseWidth, double _alpha):mySkinLaser(_wavelength, _pulseWidth, _alpha)
{
    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setPulseWidth(_pulseWidth);
    setAlpha(_alpha);

    mySkinLaser.EMP();
    mySkinLaser.adaptForSkinEMP();
    mySkinLaser.getEMP();
    formula=mySkinLaser.getFormulaEMP();
    formulaSort=mySkinLaser.getFormulaSort();
    CA=mySkinLaser.getCA();
    CB=mySkinLaser.getCB();
    CC=mySkinLaser.getCC();
    CE=mySkinLaser.getCE();
    T1=mySkinLaser.getT1();
    T2=mySkinLaser.getT2();
    gamma=mySkinLaser.getGamma();
    radiation=mySkinLaser.getRadiation();
    skinDamage=mySkinLaser.getSkinDamage();
    eyeDamage=mySkinLaser.getEyeDamage();
    notes=mySkinLaser.getPhotochemicalNote();

    computePowerNSHD();
    computePowerForExended();
}

void LaserSkinSafety::laserSkinUpdate()
{
    mySkinLaser.EMP();
    mySkinLaser.adaptForSkinEMP();
    EMP_Result=getEMP();
    computePowerForExended();
    computePowerNSHD();
    computeErgNSHD();
}

double LaserSkinSafety::getWavelength()const
{
    return wavelength;
}

void LaserSkinSafety::setWavelength(const double & _wavelength)
{
    if(_wavelength==wavelength)
        return;

    mySkinLaser.setWavelength(_wavelength);
    wavelength=_wavelength;
}

void LaserSkinSafety::setPowerErg(const double & _powerErg)
{
    if(_powerErg==powerErg)
        return;

    powerErg=_powerErg;
}

double LaserSkinSafety::getPowerErg() const
{
    return powerErg;
}

double LaserSkinSafety::getAlpha() const
{
    return alpha;
}

void LaserSkinSafety::setAlpha(const double & _alpha)
{
    if(_alpha==alpha)
        return;

    mySkinLaser.setAlpha(_alpha);
    alpha=_alpha;
}

double LaserSkinSafety::getDivergence() const
{
    return divergence;
}

void LaserSkinSafety::setDivergence(const double & _divergence)
{
    if(_divergence==divergence)
        return;

    divergence=_divergence;
}

double LaserSkinSafety::getBeamDiameter() const
{
return beamDiameter;
}

void LaserSkinSafety::setBeamDiameter(const double & _beamDiameter)
{
    if(_beamDiameter==beamDiameter)
        return;

    beamDiameter=_beamDiameter;
}


double LaserSkinSafety::getErgNSHD() const
{
    return ergNSHD;
}

double LaserSkinSafety::getPowerNSHD() const
{
    return powerNSHD;
}

void LaserSkinSafety::computeBeamArea()
{
    beamArea=PI*std::pow(beamDiameter,2)/4;
}

double LaserSkinSafety::getBeamArea() const
{
    return beamArea;
}

double LaserSkinSafety::computePowerNSHD()
{
    double ForNSHD;
    formulaSort=getFormulaSort();
    if(formulaSort=="E")
        ForNSHD=powerErg;
    else
        ForNSHD=powerErg*exposureTime;

    double underroot;

    underroot=(4*ForNSHD)/(PI*EMP_Result);
    powerNSHD=(std::sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
    if(powerNSHD<0)
        powerNSHD=0;

    return powerNSHD;
}

double LaserSkinSafety::computeErgNSHD()
{
    double ForNSHD;
    formulaSort=getFormulaSort();
	
    if(formulaSort=="E")
        ForNSHD=powerErg/pulseWidth;
	else
        ForNSHD=powerErg;

    double underroot;
		
    underroot=(4*ForNSHD)/(PI*EMP_Result);
    ergNSHD=(std::sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
    if (ergNSHD<0)
        ergNSHD=0;

    return ergNSHD;
}

double LaserSkinSafety::getCA() const
{
    return mySkinLaser.getCA();
}

double LaserSkinSafety::getCB() const
{
    return mySkinLaser.getCB();
}

double LaserSkinSafety::getCC() const
{
    return mySkinLaser.getCC();
}

double LaserSkinSafety::getCE() const
{
    return mySkinLaser.getCE();
}

double LaserSkinSafety::getT1() const
{
    return mySkinLaser.getT1();
}

double LaserSkinSafety::getT2() const
{
    return mySkinLaser.getT2();
}

string LaserSkinSafety::getRadiation() const
{
    return mySkinLaser.getRadiation();
}

string LaserSkinSafety::getSkinDamage() const
{
    return mySkinLaser.getSkinDamage();
}

string LaserSkinSafety::getEyeDamage() const
{
    return mySkinLaser.getEyeDamage();
}

string LaserSkinSafety::getPhotochemicalNote() const
{
    return mySkinLaser.getPhotochemicalNote();
}

double LaserSkinSafety::getGamma() const
{
    return mySkinLaser.getGamma();
}

double LaserSkinSafety::getEMP()
{
    return mySkinLaser.getEMP();
}

string LaserSkinSafety::getFormulaEMP() const
{
    return mySkinLaser.getFormulaEMP();
}

string LaserSkinSafety::getFormulaSort() const
{
    return mySkinLaser.getFormulaSort();
}

void LaserSkinSafety::EMP()
{
    mySkinLaser.EMP();
}

void LaserSkinSafety::setPulseWidth(const double & _pulseWidth)
{
    if(_pulseWidth==pulseWidth)
        return;

    mySkinLaser.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}

double LaserSkinSafety::getPulseWidth() const
{
    return pulseWidth;
}

double LaserSkinSafety::getPowerForExended() const
{
    return powerForExended;
}

void LaserSkinSafety::computePowerForExended()
{
    powerForExended=powerErg/pulseWidth;
}
