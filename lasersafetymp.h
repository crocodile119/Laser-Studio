#ifndef LASERSAFETYMP_H
#define LASERSAFETYMP_H
#include "lasersafetycw.h"

using namespace std;

class LaserSafetyMP : public LaserSafetyCW
{
public:
LaserSafetyMP(int, double, double, double, double, double, double);

void computeNOHD();
void computeLambertianMax();
void laserUpdate();
void computeMeanPower();
void computeMeanPowerIrradiance();
void computeCP();
void computePulseNumber();
void computeMeanPowerEMP();
void computeSP_CP_EMP();
double returnMultiPulse_EMP();
double getPowerErgForEMP();
string getMeanPowerFormulaSort();
string getMeanPowerFormulaEMP();
double getCA_MP() const;
double getCB_MP() const;
double getCC_MP() const;
double getCE_MP() const;
double getT1_MP() const;
double getT2_MP() const;
double getGamma_MP() const;
string getRadiation_MP() const;
string getSkinDamage_MP() const;
string getEyeDamage_MP() const;
string getPhotochemicalNote_MP() const;
double getEMP_MP();
void setPRF(const int);
int getPRF() const;
double getMeanPower() const;
double getMeanIrradiance() const;
double getThermalEMP() const;
double getCPCoefficient() const;
double getPulseNumber() const;
double getEMP_ForOD()const;
void computeMeanPower_NOHD();
void computeSinglePulse_NOHD();
void computePulseTrain_NOHD();
void computeEMP_ForOD();

void computeMeanPowerLambertianMax();
void computeSinglePulseLambertianMax();
void computePulseTrainLambertianMax();

double getEMP_1stCondition()const;
double getEMP_2ndCondition()const;
double getEMP_3rdCondition()const;

string getFormula_3rdCondition();
string getFormulaSort_3rdCondition();

void equateMeanPowerEMP();
void computeTmin();
double getTmin() const;
string valutateCounting();
double getMeanPower_EMP_Equate() const;

private:
int PRF;
double MeanPower;
double MeanPower_NOHD;
double SinglePulse_NOHD;
double PulseTrain_NOHD;

double MeanPowerLambertianMax;
double SinglePulseLambertianMax;
double PulseTrainLambertianMax;

double MeanPowerIrradiance;
int PulseNumber;
double CP;
computeEMP MyMeanPower_Laser;
computeEMP MyTmin_Laser;
//oggetto relativo all'esposizione al treno di impulsi
double MeanPow_EMP_Result;
double PowerEMP;
double CP_EMP_Result;
double SP_EMP_Result;
double EMP_ForOD;
string PowerFormulaSort;
string PowerFormulaEMP;
double MeanPow_EMP_Equate;
double Tmin;
string Formula_Tmin;
string FormulaSort_Tmin;
};

#endif // LASERSAFETYMP_H
