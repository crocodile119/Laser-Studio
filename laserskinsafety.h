#ifndef LASERSKINSAFETY_H
#define LASERSKINSAFETY_H
#include "computeemp.h"


using namespace std;
	

class LaserSkinSafety
{
public:
LaserSkinSafety(double, double, double, double, double, double);

const static double PI;

double getWavelength();
void setWavelength(const double);
double getAlpha() const;
void setAlpha(const double);
double getDivergence() const;
void setDivergence(const double);
double getBeamDiameter() const;
void setBeamDiameter(const double);
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
void setPulseWidth(const double);
double getPulseWidth() const;
void setPowerErg(const double);
double getPowerErg() const;
double getPowerForExended() const;
void computePowerForExended();

protected:
double Wavelength;
double EMP_Result;
computeEMP MySkinLaser;
double Alpha;
double PulseWidth;
double PowerErg;
double PowerNSHD;
double ErgNSHD;
double Divergence;
double BeamDiameter;
double LaserEMP;
double BeamArea;
string Formula;
double CA;
double CB;
double CC;
double CE;
double T1;
double T2;
double t_exp;
double Gamma;
string Radiation;
string SkinDamage;
string EyeDamage;
string Note;
string FormulaSort;
double PowerForExended;
double exposureTime;
};

#endif // LASERSkinSAFETY_H
