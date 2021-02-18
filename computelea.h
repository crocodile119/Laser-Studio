#ifndef COMPUTELEA_H
#define COMPUTELEA_H
#include "leadata.h"
#include "empleatables.h"
#include <string>
#include <QtGui>

using namespace std;

class ComputeLEA
{
public:
    ComputeLEA(double=633, double=1.0e-004, double=1.5);
	
    const static double ALPHA_MIN;

    void selectLea_1M_Row(const std::array<leadata, EmpLeaTables::TABLEROW_1_1M> &);
    void selectLea_3R_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3R> &);
    void selectLea_3B_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3B> &);

	void LEA();
    void calculate();   
    void writeLeaInStructValues(classData);
    void setWavelength(const double&);
    double getWavelength() const;
    void setPulseWidth(const double&);
    double getPulseWidth() const;
    void setAlpha(const double&);
    double getAlpha() const;
    double getAlpha_max() const;

	double getC1() const;
    double getC2() const;
    double getC3() const;
    double getC4() const;
    double getC5() const;
    double getC6() const;
    double getC7() const;
	double getT1() const;
	double getT2() const;
	string getRadiation() const;
	string getSkinDamage() const;
    string getEyeDamage() const;
	double getGamma() const;
    void computeAlpha_max();
    string valuateFormula(leadata &);

    double LEA_Value(leadata &);
    void computeParameters();
    double compute_t(leadata &);
    void computePhotoGamma();
    string valuateFormulaTipo(leadata &);
    string valuateFormulaUnit(leadata &);
    void applyLEA_CorrectionFactor(const double&);
    void extractSelctedRow();
    QString create_LEA_Table(classData);
    QString typeOfEyeDamage(const int&);
    QString typeOfRadiation(const int&);
    QString typeOfSkinDamage(const int&);

    double *getLEA()const;
    string *getLEA_Formula()const;
    string *getLEA_FormulaTipo()const;
    string *getLEA_FormulaUnit()const;
    int *getLEA_FormulaSort()const;

    double valuate_LEA_2M_Value();
    string valuate_LEA_2M_Formula();
    string valuate_LEA_2M_FormulaUnit();
    string valuate_LEA_2M_FormulaTipo();

    void bioEffects();

    void valuateApertureCondition_3();
    void valuateCondition_1();
    void valuateCondition_3();

    double getApertureStopCond_1()const;
    double getApertureStopCond_3()const;
    double getDistanceCond_1()const;
    double getDistanceCond_3()const;
    double getApertureThirdCondition()const;

private:
    double wavelength;
    double pulseWidth;
    double alpha;
    double alpha_max;
    int n_lea=4;

    std::array<leadata, EmpLeaTables::TABLEROW_1_1M> leaStructValues_1M;
    std::array<leadata, EmpLeaTables::TABLEROW_3R>leaStructValues_3R;
    std::array<leadata, EmpLeaTables::TABLEROW_3B>leaStructValues_3B;

    leadata myLeaData;

    double *LEA_Result;
    string *LEA_Formula;
    int *LEA_FormulaSort;
    string *LEA_FormulaTipo;
    string *LEA_FormulaUnit;

    leadata myLeaData_1M;
    leadata myLeaData_3R;
    leadata myLeaData_3B;

	double C1;
    double C2;
    double C3;
    double C4;
    double C5;
    double C6;
    double C7;
	double T1;
    double T2;
    double photoGamma;
    string radiation;
    string skinDamage;
    string eyeDamage;
    string note;

    double apertureStopCond_1;
    double distanceCond_1;
    double apertureStopCond_3;
    double distanceCond_3;
    double apertureThirdCondition;
    double diameterAtStopCond_1;
    double diameterAtStopCond_3;
    double couplingFactor_1;
    double couplingFactor_2;
};
#endif // COMPUTELEA_H
