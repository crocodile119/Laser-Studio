#ifndef LASERSKINSAFETY_H
#define LASERSKINSAFETY_H
#include "computeemp.h"


using namespace std;
	
class LaserSkinSafety
{
public:
LaserSkinSafety(double, double, double, double, double, double);

const static double PI;

double getWavelength()const;
void setWavelength(const double &);
double getAlpha() const;
void setAlpha(const double &);
double getDivergence() const;
void setDivergence(const double &);
double getBeamDiameter() const;
void setBeamDiameter(const double &);
double computePowerNSHD();
double computeErgNSHD();
void computeBeamArea();
double getPowerNSHD() const;
double getErgNSHD() const;
double getBeamArea() const;
double getCA() const;
double getCB() const;
double getCC() const;
double getCE() const;
double getT1() const;
double getT2() const;
void EMP();
string getRadiation() const;
string getSkinDamage() const;
string getEyeDamage() const;
string getPhotochemicalNote() const;
double getGamma() const;
double getEMP();
string getFormulaEMP() const;
string getFormulaSort() const;
void laserSkinUpdate();
void setPulseWidth(const double &);
double getPulseWidth() const;
void setPowerErg(const double &);
double getPowerErg() const;
double getPowerForExended() const;
void computePowerForExended();

protected:
double wavelength;
double EMP_Result;
ComputeEMP mySkinLaser;
double alpha;
double pulseWidth;
double powerErg;
double powerNSHD;
double ergNSHD;
double divergence;
double beamDiameter;
double laserEMP;
double beamArea;
string formula;
double CA;
double CB;
double CC;
double CE;
double T1;
double T2;
double t_exp;
double gamma;
string radiation;
string skinDamage;
string eyeDamage;
string notes;
string formulaSort;
double powerForExended;
double exposureTime;
};

#endif // LASERSkinSAFETY_H
