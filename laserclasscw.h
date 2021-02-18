#ifndef LASERCLASSCW_H
#define LASERCLASSCW_H
#include "computelea.h"


using namespace std;

class LaserClassCW
{
public:
LaserClassCW(double, double, double, double, double, double);

enum laserOperation{CW, PULSED, MULTIPULSED};
enum laserClass{CLASSE_1, CLASSE_1M, CLASSE_2, CLASSE_2M, CLASSE_3R, CLASSE_3B, CLASSE_4, NON_CLASSIFICATO};

    const static double PI;
    double getWavelength()const;
    void setWavelength(const double&);
    double getAlpha() const;
    void setAlpha(const double&);
    double getDivergence() const;
    void setDivergence(const double&);
    double getBeamDiameter() const;
    void setBeamDiameter(const double&);
    void setTimeBase();
    double getTimeBase()const;
    ComputeLEA getLEA_Data();

    void computeBeamArea();
    double getBeamArea() const;
    double getC1() const;
    double getC2() const;
    double getC3() const;
    double getC4() const;
    double getC5() const;
    double getC6() const;
    double getC7() const;
    double getT1() const;
    double getT2() const;
    double getGamma() const;

    string getRadiation() const;
    string getSkinDamage() const;
    string getEyeDamage() const;
    string getPhotochemicalNote() const;

    double* getLEA()const;
    string* getLEA_Formula()const;
    string* getLEA_FormulaUnit()const;
    string* getLEA_FormulaTipo()const;
    int *getLEA_FormulaSort()const;

    void setPowerErg(const double&);
    double getPowerErg() const;
    double valuateBeamDiameterAtStop(double&, double&);
    double valuateCouplingFactor(double&, double&);
    void valuateClass();
    void classUpdate(laserOperation, const double&, const double &);

    double* leaPowerErgUnit(laserOperation, int*, const double &, const double &);
    bool* valuateLEA_forClass(double*, double*, double*);
    laserClass valuateClass(bool*);

    void computeBeamAtStop(double&, double&);
    void computeCouplingFactors(double&, double&);
    void computePowerErgConditions();

    double* computePowerErgCond_1(double*, const double &);
    double* computePowerErgCond_3(double*, const double &);

    double* getPowerErg_Cond_1()const;
    double* getPowerErg_Cond_3()const;

    double getCouplingFactor_Cond_1()const;
    double getCouplingFactor_Cond_3()const;

    double getDistCond_1()const;
    double getDistCond_3()const;

    double getApCond_1()const;
    double getApCond_3()const;

    double getBeamAtStop_Cond_1()const;
    double getBeamAtStop_Cond_3()const;

    bool isInternalWaist();
    void setInternalWaist(bool);

    laserClass getLaserClass()const;
    void updateAll();

protected:
    int n_lea=4;
    int n_class=7;
    double wavelength;
    ComputeLEA myLaserClass;
    double alpha;
    double powerErg;
    double divergence;
    double beamDiameter;
    double beamArea;
    bool internalWaist;

    double* LEA_Value;
    string* LEA_formula;
    string* LEA_formulaTipo;
    string* LEA_formulaUnit;
    int* LEA_formulaSort;
    double* powerErgEq;
    bool* classValutation;

    double distanceCond_1;
    double distanceCond_3;
    double apertureStopCond_1;
    double apertureStopCond_3;

    double beamAtStop_Cond_1;
    double beamAtStop_Cond_3;

    double couplingFactor_Cond_1;
    double couplingFactor_Cond_3;

    double distCond_1;
    double distCond_3;

    double apCond_1;
    double apCond_3;

    double *powerErg_Cond_1;
    double *powerErg_Cond_3;

    laserClass laserClassAssigned;

    double C1;
    double C2;
    double C3;
    double C4;
    double C5;
    double C6;
    double C7;
    double T1;
    double T2;
    double t_exp;
    double Gamma;
    string Radiation;
    string SkinDamage;
    string EyeDamage;
    string Note;
    string FormulaSort;
    double timeBase;
};


#endif // LASERCLASSCW_H
