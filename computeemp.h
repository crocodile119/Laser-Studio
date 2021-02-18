#ifndef COMPUTEEMP_H
#define COMPUTEEMP_H
#include "empdata.h"
#include "empleatables.h"
#include <string>
#include <QtGui>
#include <array>

using namespace std;



class ComputeEMP
{
public:
    ComputeEMP(double=632, double=20, double=1.5);
	
	const static double ALPHA_MIN;
	const static double ALPHA_MAX;

	void EMP();
    void setWavelength(const double&);
    double getWavelength() const;
    void setPulseWidth(const double&);
    double getPulseWidth() const;
    void setAlpha(const double&);
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

    void selectLeaRow();
    string valuateFormula(const empdata & myEmpData);

    string valuateFormulaTipo(const empdata & myEmpData);
    string valuateFormulaUnit(const empdata & myEmpData);

    QString typeOfSkinDamage(const int &);
    QString typeOfEyeDamage(const int &);
    QString typeOfRadiation(const int &);
    void writeEmpInStructValues();



private:
    QFile fileBinary;
    double wavelength;
    double pulseWidth;
    double alpha;
    string formula;
    string formulaSort;
	double EMP_Result;
	double CA;
    double CB;
    double CC;
    double CE;
	double T1;
	double T2;
	double t_exp;
    string radiation;
    string skinDamage;
    string eyeDamage;
    string notes;

    std::array<empdata, EmpLeaTables::TABLEROW_EMP> empStructValues;
    empdata myEmpData;

    string EMP_Formula;
    int EMP_FormulaSort;
    string EMP_FormulaTipo;
    string EMP_FormulaUnit;
	
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
