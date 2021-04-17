#include <cmath>
#include <string>
#include "lasersafety.h"


using namespace std;

const double LaserSafety::PI = 3.141592653589793;
const double LaserSafety::PULSE_ENERGY=1.0e-03;
const double LaserSafety::PUPIL_DIAMETER=7.0;
const double LaserSafety::DIVERGENCE=1.5;
const double LaserSafety::PULSE_WIDTH=1.0e-06;

LaserSafety::LaserSafety(double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                        double _pulseWidth, double _alpha):myLaser(_wavelength, _pulseWidth, _alpha)
{   
    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setPulseWidth(_pulseWidth);
    setAlpha(_alpha);

    myLaser.EMP();
    myLaser.getEMP();
    formula=myLaser.getFormulaEMP();
    formulaSort=myLaser.getFormulaSort();
    CA=myLaser.getCA();
    CB=myLaser.getCB();
    CC=myLaser.getCC();
    CE=myLaser.getCE();
    T1=myLaser.getT1();
    T2=myLaser.getT2();

    gamma=myLaser.getGamma();
    radiation=myLaser.getRadiation();
    skinDamage=myLaser.getSkinDamage();
    eyeDamage=myLaser.getEyeDamage();
    notes=myLaser.getPhotochemicalNote();

    computeNOHD();
    computeLambertianMax();
    computePowerForExended();
}

void LaserSafety::laserUpdate()
{
    myLaser.EMP();
    EMP_Result=getEMP();
    computePowerForExended();
    computeNOHD();
    computeLambertianMax();
}

double LaserSafety::getWavelength() const
{
    return wavelength;
}

void LaserSafety::setWavelength(const double & _wavelength)
{
    if(_wavelength==wavelength)
        return;

    myLaser.setWavelength(_wavelength);
    wavelength=_wavelength;
}

void LaserSafety::setPowerErg(const double &_powerErg)
{
    if(_powerErg==powerErg)
        return;

    powerErg=_powerErg;
}

double LaserSafety::getPowerErg() const
{
    return powerErg;
}

double LaserSafety::getAlpha() const
{
    return alpha;
}

void LaserSafety::setAlpha(const double & _alpha)
{
    if(_alpha==alpha)
        return;

    myLaser.setAlpha(_alpha);
    alpha=_alpha;
}

double LaserSafety::getDivergence() const
{
    return divergence;
}

void LaserSafety::setDivergence(const double & _divergence)
{
    if(_divergence==divergence)
        return;

    divergence=_divergence;
}

double LaserSafety::getBeamDiameter() const
{
    return beamDiameter;
}

void LaserSafety::setBeamDiameter(const double& _beamDiameter)
{
    if(_beamDiameter==beamDiameter)
        return;

    beamDiameter=_beamDiameter;
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
    beamArea=PI*pow(beamDiameter*1.0e-03, 2)/4;
}

double LaserSafety::getBeamArea() const
{
    return beamArea;
}

void LaserSafety::computeNOHD()
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

double LaserSafety::getPowerErgForEMP()
{
    double powerErgForEMP;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
        powerErgForEMP=powerErg/pulseWidth;
    else
        powerErgForEMP=powerErg;

    return powerErgForEMP;
}

/*Calcolo la distanza di sicurezza ottica per effetto della riflessione su un riflettore
 * lambertiano. Il calcolo fa riferimento al caso di riflessione puntiforme con direzione
 * normale al diffusore e coeficiente di diffusione unitario. */

void LaserSafety::computeLambertianMax()
{
    double ForLambertianMax;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
        ForLambertianMax=powerErg/pulseWidth;
    else
        ForLambertianMax=powerErg;

    double underroot;

    underroot=(ForLambertianMax)/(PI*EMP_Result);
    lambertianMax=(sqrt(underroot));
}

double LaserSafety::getCA() const
{
    return myLaser.getCA();
}

double LaserSafety::getCB() const
{
    return myLaser.getCB();
}

double LaserSafety::getCC() const
{
    return myLaser.getCC();
}

double LaserSafety::getCE() const
{
    return myLaser.getCE();
}

double LaserSafety::getT1() const
{
    return myLaser.getT1();
}

double LaserSafety::getT2() const
{
    return myLaser.getT2();
}

string LaserSafety::getRadiation() const
{
    return myLaser.getRadiation();
}

string LaserSafety::getSkinDamage() const
{
    return myLaser.getSkinDamage();
}

string LaserSafety::getEyeDamage() const
{
    return myLaser.getEyeDamage();
}

string LaserSafety::getPhotochemicalNote() const
{
    return myLaser.getPhotochemicalNote();
}

double LaserSafety::getGamma() const
{
    return myLaser.getGamma();
}

double LaserSafety::getEMP()
{
    return myLaser.getEMP();
}

string LaserSafety::getFormulaEMP() const
{    
    qDebug()<<"formula: "<<QString::fromStdString(myLaser.getFormulaEMP());
    return myLaser.getFormulaEMP();
}

string LaserSafety::getFormulaSort() const
{
    return myLaser.getFormulaSort();
}

void LaserSafety::EMP()
{
    myLaser.EMP();
}

void LaserSafety::setPulseWidth(const double & _pulseWidth)
{
    if(_pulseWidth==pulseWidth)
        return;

    myLaser.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}

double LaserSafety::getPulseWidth() const
{
    return pulseWidth;
}

double LaserSafety::getPowerForExended() const
{
    return powerForExended;
}

void LaserSafety::computePowerForExended()
{
    powerForExended=powerErg/pulseWidth;
}
