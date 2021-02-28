#ifndef LASERCLASSMP_H
#define LASERCLASSMP_H
#include "laserclasssp.h"

using namespace std;

class LaserClassMP: public LaserClassSP
{
public:
    LaserClassMP(double, double, double, double, double, double, double);
    void meanClassUpdate(const double&, const double&);
    void tiClassUpdate(const double&, const double&);
    void c5ClassUpdate();
    void computeTi();
    void computeMeanPower();
    void computePulseNumber();
    string valutateCounting();
    double getTi()const;
    void computeC5();
    void setPRF(const double &_prf);
    double getPRF()const;
    void setTimeBase();
    double* powerErgUnit(int*, const double &, const double &);
    double* computeMeanLEA_Corrected(int*);
    double* computeLEA_ThermalCorrection(double*);

    void setWavelength(const double&);
    void setPulseWidth(const double&);
    void setAlpha(const double&);
    void setAlpha_max(const double &);
    double getMeanPower() const;
    double getC5Coefficient() const;
    double getPulseNumber() const;


/***************************************************************
 * funzioni membro per il calcolo relativo all'emissione media *
 * **************************************************************/
    double* getMeanLEA()const;
    string* getMeanLEA_Formula()const;
    string* getMeanLEA_FormulaUnit()const;
    string* getMeanLEA_FormulaTipo()const;
    double* getMeanPowerErgEq()const;
    bool* getMeanClassValutation()const;
    double getMeanDistCond_1()const;
    double getMeanDistCond_3()const;
    double getMeanApCond_1()const;
    double getMeanApCond_3()const;
    double getMeanBeamAtStop_Cond_1()const;
    double getMeanBeamAtStop_Cond_3()const;
    double getMeanCouplingFactor_Cond_1()const;
    double getMeanCouplingFactor_Cond_3()const;
    double* getMeanPowerErg_Cond_1()const;
    double* getMeanPowerErg_Cond_3()const;
    string getMeanLaserClassAssigned()const;
    double *getMeanLEA_Corrected()const;

    int *getMeanLEA_FormulaSort()const;

    double* getTiLEA()const;
    string* getTiLEA_Formula()const;
    string* getTiLEA_FormulaUnit()const;
    string* getTiLEA_FormulaTipo()const;
    double* getTiPowerErgEq()const;
    bool* getTiClassValutation()const;
    double getTiDistCond_1()const;
    double getTiDistCond_3()const;
    double getTiApCond_1()const;
    double getTiApCond_3()const;
    double getTiBeamAtStop_Cond_1()const;
    double getTiBeamAtStop_Cond_3()const;
    double getTiCouplingFactor_Cond_1()const;
    double getTiCouplingFactor_Cond_3()const;
    double* getTiPowerErg_Cond_1()const;
    double* getTiPowerErg_Cond_3()const;
    string getTiLaserClassAssigned()const;
    double *getTiLEA_Corrected()const;
    void setTe();
    double getTe()const;

    int *getTiLEA_FormulaSort()const;

    string getC5LaserClassAssigned()const;
    double *getLEA_Corrected()const;

    bool* valuateSystemClass(bool*, bool*, bool* c5ClassValutation=nullptr);

    void highFrequencyValuation();
    laserClass getSystemClassValutation()const;
    void updateAll();

protected:
    double prf;
    double Ti;
    double meanPower;
    int pulseNumber;
    double alpha_max;

   /***********************************************************
    * Te è il tempo di esposizione da considerare nel calcolo *
    * del numero di impulsi (il minore tra la timeBase e T2). *
    ***********************************************************/
    double Te;
    ComputeLEA myMeanLaserClass;
    ComputeLEA myTiLaserClass;
    double* meanLEA_Value;
    string* meanLEA_formula;
    string* meanLEA_formulaTipo;
    string* meanLEA_formulaUnit;
    int* meanLEA_formulaSort;
    double* meanPowerErgEq;
    bool* meanClassValutation;
    double C5;
    double meanDistCond_1;
    double meanDistCond_3;
    double meanApCond_1;
    double meanApCond_3;
    double meanBeamAtStop_Cond_1;
    double meanBeamAtStop_Cond_3;
    double meanCouplingFactor_Cond_1;
    double meanCouplingFactor_Cond_3;
    double *meanPowerErg_Cond_1;
    double *meanPowerErg_Cond_3;
    string meanLaserClassAssigned;
    double *meanLEA_Corrected;

    string c5LaserClassAssigned;
    bool* c5ClassValutation;

    double* tiLEA_Value;
    string* tiLEA_formula;
    string* tiLEA_formulaTipo;
    string* tiLEA_formulaUnit;
    int* tiLEA_formulaSort;
    double* tiPowerErgEq;
    bool* tiClassValutation;
    double tiDistCond_1;
    double tiDistCond_3;
    double tiApCond_1;
    double tiApCond_3;
    double tiBeamAtStop_Cond_1;
    double tiBeamAtStop_Cond_3;
    double tiCouplingFactor_Cond_1;
    double tiCouplingFactor_Cond_3;
    double *tiPowerErg_Cond_1;
    double *tiPowerErg_Cond_3;
    string tiLaserClassAssigned;
    double *tiLEA_Corrected;
    double *LEA_Corrected;
    bool* systemClassValutation;
    laserClass systemLaserClassAssigned;
};


#endif // LASERCLASSMP_H
