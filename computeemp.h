#ifndef COMPUTEEMP_H
#define COMPUTEEMP_H
#include "empsdata.h"
#include <string>
#include <QtGui>

using namespace std;



class computeEMP
{
public:
    computeEMP(double=632, double=20, double=1.5);
	
	const static double ALPHA_MIN;
	const static double ALPHA_MAX;
    static int fileReadCount;

	void EMP();
    void setWavelength(const double);
    double getWavelength() const;
    void setPulseDuration(const double);
    double getPulseDuration() const;
	void setAlpha(const double);
	double getAlpha() const;
	double getCA() const;
    double getCB() const;
    double getCC() const;
    double getCE() const;
	double getT1() const;
	double getT2() const;
	string getRadiation() const;
	string getSkinDamage() const;
	string getEyeDamage() const;
	string getPhotochemicalNote() const;
	double getGamma() const;
	void valutateFormula();
	void EMP_Value();
	void ComputeParameters();
	double getEMP() const;
	string getFormulaEMP() const;
	string getFormulaSort() const;
	void PhotoEffects();
	void ThermoEffects();
	void EMPPhotoThermo();
	void BioEffects();
	void outputLine();
    void adaptForSkinEMP();


private:
    QFile fileBinary;
    double Wavelength;
    double PulseDuration;
	double Alpha;
    empdata LimitValues;
	string Formula;
    string FormulaSort;
	double EMP_Result;
	double CA;
    double CB;
    double CC;
    double CE;
	double T1;
	double T2;
	double t_exp;
	string Radiation;
	string SkinDamage;
	string EyeDamage;
    string Note;
	
	//Photochemical effects vs thermal effects assasment variables
	//Photochemical effects
	double _PhotoEMP;
	string _PhotoFormulaSort;
	string _PhotoFormula;
	double _PhotoEMPH;
	double _PhotoGamma;
	//Thermal effects
	double _ThermoEMP;
	string _ThermoFormulaSort;
	string _ThermoFormula;
	double _ThermoEMPH;
	
};
#endif // COMPUTEEMP_H
