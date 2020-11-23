#ifndef LASERSAFETY_H
#define LASERSAFETY_H
#include "computeemp.h"


using namespace std;
	

class LaserSafety
{
public:
LaserSafety(double, double, double, double, double, double);

const static double PI;

double getWavelength();
void setWavelength(const double);
double getAlpha() const;
void setAlpha(const double);
double getDivergence() const;
void setDivergence(const double);
double getBeamDiameter() const;
void setBeamDiameter(const double);
void computeNOHD();
double getPowerErgForEMP();
void computeLambertianMax();
void computeBeamArea();
double getNOHD() const;
double getLambertianMax() const;
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
void laserUpdate();
void setPulseWidth(const double);
double getPulseWidth() const;
void setPowerErg(const double);
double getPowerErg() const;
double getPowerForExended() const;
void computePowerForExended();

protected:
double Wavelength;
double EMP_Result;
computeEMP MyLaser;
double Alpha;
double PulseWidth;
double PowerErg;
double NOHD;
double lambertianMax;
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
};

#endif // LASERSAFETY_H
