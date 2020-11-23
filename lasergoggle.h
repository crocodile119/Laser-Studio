#ifndef LASERGOGGLE_H
#define LASERGOGGLE_H

#include <vector>
#include <utility>
#include <functional>
#include <string>

using namespace std;

class LaserGoggle
{
public:
    static const int timeBaseLowWavelength;
    static const int timeBaseWavelength;
    static const double PI;
    LaserGoggle(int, double, double, double);
    LaserGoggle(int, double, double, double, int);
    double* selectData(const double &, const double &);
    vector< pair <int,double> > buildDataVector(const double[], const int[]);
    vector< pair <int,double> > getDataVector();
    int scaleNumber(vector< pair <int,double> > dataVector, double);
    double pulseTrainCorrectionK();
    double pulseTrainCorrectionKi();
    double frequencyCorrection();
    double laserIrrRad(double powerErg);
    double meanPower();
    int numberOfPulse();
    int* buildScaleNumbers();
    LaserGoggle* getLaserGoggleForD();
    double laserIrrRadCorrected(double);
    void prepareGoggleMark();
    string goggleMark(int, double, double, double, double);
    string goggleMark();
    //string goggleMarkTrain(int, double, double, double, double);

//getters && setters
    int getWavelength();
    void setWavelength(const double);
    double getPulseWidth();
    void setPulseWidth(const double);
    double getPowerErg();
    void setPowerErg(const double);
    double getBeamDiameter();
    void setBeamDiameter(const double);
    double getFrequency();
    void setFrequency(const double);
    int getScaleNumber()const;
    int getNumberOfPulse()const;
    double getCoefficient_ki()const;
    double getCoefficient_k()const;
    double get_ni_max()const;
    string getKindOfGoggle();
    string getCodeUnit();
    string outputSort();
    void printData();
    void printScaleNumbers(int[]);
    string printGoggleCode();
    void printVector( vector< pair <int,double> >);

private:
    double *expositionData;
    vector<pair<int, double>> dataVector;
    double myExpositionValue;
    int n;
    double thermicExpositionValue;
    bool pulseTrain;
    double powerErg;
    double wavelength;
    double pulseWidth;
    double beamDiameter;

    double irrRad;
    double myMeanPower;
    int numberOfPulses;
    string myGoggleCode;
    enum laserOperation{CONTINUOSWAVE, IMPULSATO, IMPULSIRIPETUTI};
    int frequency;
    double ki;
    double k;
    int myScaleNumber;
    int *LB_Scale;
    double Ti;
    double ni_max;
    string codeUnit;
    string pulseCode;
    string laserOutputSort;
    string goggleCode;
};

#endif // LASERGOGGLE_H
