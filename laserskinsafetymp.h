#ifndef LASERSKINSAFETYMP_H
#define LASERSKINSAFETYMP_H
#include "laserskinsafety.h"

using namespace std;

class LaserSkinSafetyMP : public LaserSkinSafety
{
public:
LaserSkinSafetyMP(int, double, double, double, double, double, double, double);

void computeNSHD();
void laserSkinUpdate();
void computeMeanPower();
void computeMeanPowerIrradiance();
void computeMeanPowerEMP();
void computeSP_EMP();
double returnMultiPulse_EMP();
string getMeanPowerFormulaSort();
string getMeanPowerFormulaEMP();
void computePulseNumber();
double getEMP_MP();
void setPRF(const int);
int getPRF() const;
double getMeanPower() const;
double getMeanIrradiance() const;
double getPulseNumber() const;
void computeMeanPower_NSHD();
void computeSinglePulse_NSHD();
void equateMeanPowerEMP();
double getMeanPower_EMP_Equate() const;
double getExposureTime() const;
void setExposureTime(const double &);
double getNSHD() const;
double getEMP_1stCondition()const;
double getEMP_2ndCondition()const;

private:
int PRF;
double MeanPower;
double MeanPower_NSHD;
double SinglePulse_NSHD;
double MeanPowerIrradiance;
int PulseNumber;
computeEMP MyMeanPower_SkinLaser; //oggetto relativo all'esposizione al treno di impulsi
double MeanPow_EMP_Result;
double PowerSkinEMP;
double SP_EMP_Result;
string PowerFormulaSort;
string PowerFormulaEMP;
double MeanPow_EMP_Equate;
double NSHD;

};

#endif // LASERSKINSAFETYMP_H
