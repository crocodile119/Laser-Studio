#include <cmath>
#include <string>
#include "laserclasscw.h"

const double LaserClassCW::PI = 3.141592653589793;

LaserClassCW::LaserClassCW(double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                           double _pulseWidth, double _alpha):myLaserClass( _wavelength,_pulseWidth, _alpha)
{
    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);

    setTimeBase();
    timeBase=getTimeBase();

    powerErgEq=new double[n_lea];
    powerErg_Cond_1=new double[n_lea];
    powerErg_Cond_3=new double[n_lea];
    classValutation=new bool[n_class];

    internalWaist=false;
    updateAll();
}

void LaserClassCW::classUpdate(laserOperation myLaserOperation, const double & time, const double& _powerErg)
    {
    //In base a ciacuno dei LEA delle varie classi trasformo l'unità di misura dell'uscita laser
    powerErgEq=leaPowerErgUnit(myLaserOperation, myLaserClass.getLEA_FormulaSort(), _powerErg, time);
    //del laser calcolo le distanze e le aperture relative alle condizioni 1 e 3

    /****************************************************
     * prelevo le distanza calcolata per la condizione 1*
     * **************************************************/
     distCond_1=myLaserClass.getDistanceCond_1();
     apCond_1=myLaserClass.getApertureStopCond_1();

    /*******************************************************************************
     * Per il calcolo del diametro del diametro del fascio e del fattore di        *
     * accoppiamento per la condizione 1 è necessario verificare quand'è che       *
     * risulta applicabile (lunghezza d'onda >=302.5 e <=4000).                    *                                      *
     * *****************************************************************************/

    if((wavelength>=302.5)and(wavelength<=4000)){
    /**************************************************************************
     * Calcolo il diametro del fascio alla stessa distanza della condizione 1 *                                                *
     **************************************************************************/
        beamAtStop_Cond_1=valuateBeamDiameterAtStop(distCond_1, divergence);
    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/

        couplingFactor_Cond_1=valuateCouplingFactor(apCond_1, beamAtStop_Cond_1);
        }
        else
            {
             couplingFactor_Cond_1=1.0;
             beamAtStop_Cond_1=std::nan("N.A.");
            }

    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/
    if(internalWaist)
        distCond_3=0.0;
    else
        distCond_3=myLaserClass.getDistanceCond_3();

    beamAtStop_Cond_3=valuateBeamDiameterAtStop(distCond_3, divergence);

    apCond_3=myLaserClass.getApertureStopCond_3();
    couplingFactor_Cond_3=valuateCouplingFactor(apCond_3, beamAtStop_Cond_3);

    /******************************************************************
     * correggo i valori relativi all'uscita del laser per i fattori  *
     * di accoppiamento corrispondenti                                *
     ******************************************************************/
    powerErg_Cond_1=computePowerErgCond_1(powerErgEq, couplingFactor_Cond_1);
    powerErg_Cond_3=computePowerErgCond_3(powerErgEq, couplingFactor_Cond_3);

    /***********************************
     * valuto la classe corrispondente *
     ***********************************/
    classValutation=valuateLEA_forClass(powerErg_Cond_1, powerErg_Cond_3,
                                       myLaserClass.getLEA());

    /******************************
     * Ricavo la classe calcolata *
     ******************************/
    laserClassAssigned=valuateClass(classValutation);
}

void LaserClassCW::setTimeBase()
{
        if ((wavelength>=400) and (wavelength<=700))
            {
            timeBase=0.25;
            }
            else if((wavelength>=180)and (wavelength<400))
                {
                timeBase=30000;
                }
                    else
                    {
                    timeBase=100;
                    }
        myLaserClass.setPulseWidth(timeBase);
}

double LaserClassCW::getTimeBase()const
{
    return timeBase;
}

bool LaserClassCW::isInternalWaist()
{
    return internalWaist;
}

void LaserClassCW::setInternalWaist(bool _internalWaist)
{
    internalWaist=_internalWaist;
}

void LaserClassCW::computeBeamArea()
{
    beamArea=PI*pow(beamDiameter*1.0e-03, 2)/4;
}

double LaserClassCW::getBeamArea() const
{
    return beamArea;
}

double* LaserClassCW::leaPowerErgUnit(laserOperation myOperation, int* _LEA_formulaSort, const double &_powerErg, const double &time)
{
    double* myPowerErgEq=new double[n_lea];
    if(myOperation==CW)
    {
        for(int i=0; i<n_lea; i++)
        {
        if(_LEA_formulaSort[i]==1)
            myPowerErgEq[i]=_powerErg/beamArea;
        else if(_LEA_formulaSort[i]==2)
            myPowerErgEq[i]=_powerErg*time/beamArea;
        else if(_LEA_formulaSort[i]==3)
            myPowerErgEq[i]=_powerErg;
        else if(_LEA_formulaSort[i]==4)
            myPowerErgEq[i]=_powerErg*time;
        }
    }
    else
    {
        for(int i=0; i<n_lea; i++)
        {
        if(_LEA_formulaSort[i]==1)
            myPowerErgEq[i]=_powerErg/(time*beamArea);
        else if(_LEA_formulaSort[i]==2)
            myPowerErgEq[i]=_powerErg/beamArea;
        else if(_LEA_formulaSort[i]==3)
            myPowerErgEq[i]=_powerErg/time;
        else if(_LEA_formulaSort[i]==4)
            myPowerErgEq[i]=_powerErg;
        }
    }
    return myPowerErgEq;
}

double LaserClassCW::valuateBeamDiameterAtStop(double &condDistance,
                                             double &divergence)
{
    double diameterAtStop=beamDiameter+condDistance*divergence*1.0e-003;
    return diameterAtStop;
}

double LaserClassCW::valuateCouplingFactor(double &apertureDiameter,
                                         double &beamDiameterAtStop)
{
    double couplingFactor;
    double diametersPower;

    if((wavelength<4000)and(wavelength>302.5))
        {
        diametersPower=pow(apertureDiameter/beamDiameterAtStop, 2);
        couplingFactor= 1-std::exp(-diametersPower);
        }
    else
        couplingFactor=1;

    return couplingFactor;
}

bool* LaserClassCW::valuateLEA_forClass(double* _powerErg_Cond_1, double* _powerErg_Cond_3,
                                   double* _LEA_Value)
{
    double* myPowerErg_Cond_1=_powerErg_Cond_1;
    double* myPowerErg_Cond_3=_powerErg_Cond_3;
    double* myLEA_Value=_LEA_Value;
    bool* myClassValutation=new bool[n_class];
    /************
     * Classe 1 *
     ************/

    for(int i=0; i<n_class; i++)
    {myClassValutation[i]=false;}

    if((wavelength<302.5)and(wavelength>4000))
       {
        if(myPowerErg_Cond_3[0]<=myLEA_Value[0])
            {
            myClassValutation[0]=true;
            return myClassValutation;
            }
        }
    else if((wavelength>=302.5)and(wavelength<=4000))
       {
        if((myPowerErg_Cond_1[0]<=myLEA_Value[0])and(myPowerErg_Cond_3[0]<=myLEA_Value[0]))
            {
            myClassValutation[0]=true;
            return myClassValutation;
            }
       }

    /*************
     * Classe 1M *
     *************/
    if(!myClassValutation[0]){
    if((wavelength>=302.5)and(wavelength<=4000)){
        if((myPowerErg_Cond_1[0]>myLEA_Value[0])and(myPowerErg_Cond_1[0]<=myLEA_Value[0])and(myPowerErg_Cond_1[3]<myLEA_Value[3]))
           {
                myClassValutation[1]=true;
                return myClassValutation;
           }
        }
    }

    /************
     * Classe 2 *
     ************/

    if((!myClassValutation[0])and(!myClassValutation[1]))
    {
    if((wavelength>=400)and(wavelength<=700))
        {
            if((myPowerErg_Cond_1[1]<=myLEA_Value[1])and(myPowerErg_Cond_3[1]<=myLEA_Value[1]))
            {
                myClassValutation[2]=true;
                return myClassValutation;
            }
        }
    }

    /*************
     * Classe 2M *
     *************/
    if((!myClassValutation[0])and(!myClassValutation[1])and(!myClassValutation[2]))
    {
        if((wavelength>=400)and(wavelength<=700))
                {
                if((myPowerErg_Cond_1[1]<=myLEA_Value[1])and(myPowerErg_Cond_1[3]<myLEA_Value[3]))
                {
                    myClassValutation[3]=true;
                    return myClassValutation;
                }
        }
    }

    /*************
     * Classe 3R *
     *************/

    if((!myClassValutation[0])and(!myClassValutation[1])and(!myClassValutation[2])and(!myClassValutation[3]))
    {
                if((myPowerErg_Cond_1[2]<=myLEA_Value[2])and(myPowerErg_Cond_3[2]<=myLEA_Value[2]))
                {
                    myClassValutation[4]=true;
                    return myClassValutation;
                }
    }

    /*************
     * Classe 3B *
     *************/

    if((!myClassValutation[0])and(!myClassValutation[1])and(!myClassValutation[3])
            and(!myClassValutation[4]))
    {
                if((myPowerErg_Cond_1[3]<=myLEA_Value[3])and(myPowerErg_Cond_3[3]<=myLEA_Value[3]))
                {
                    myClassValutation[5]=true;
                    return myClassValutation;
                }
    }
    if((!myClassValutation[0])and(!myClassValutation[1])and(!myClassValutation[3])
            and(!myClassValutation[4])and(!myClassValutation[5]))
                {
                    myClassValutation[6]=true;
                }
   return myClassValutation;
}

LaserClassCW::laserClass LaserClassCW::valuateClass(bool* myClassValuation)
{
    laserClass _laserClassAssigned;
    if(myClassValuation[0])
        _laserClassAssigned=CLASSE_1;
    else if(myClassValuation[1])
        _laserClassAssigned=CLASSE_1M;
    else if(myClassValuation[2])
        _laserClassAssigned=CLASSE_2;
    else if(myClassValuation[3])
        _laserClassAssigned=CLASSE_2M;
    else if(myClassValuation[4])
        _laserClassAssigned=CLASSE_3R;
    else if(myClassValuation[5])
        _laserClassAssigned=CLASSE_3B;
    else if(myClassValuation[6])
        _laserClassAssigned=CLASSE_4;
    else
        _laserClassAssigned=NON_CLASSIFICATO;

    return _laserClassAssigned;
}

double* LaserClassCW::computePowerErgCond_1(double* _powerErgEq_1, const double &_couplingFactor_Cond_1)
{
    double* _myPowerErg=new double[n_lea];
    for(int i=0; i<n_lea; i++)
    {
            if((i==1)and((wavelength<400)or(wavelength>700)))
                _myPowerErg[i]=std::nan("N.A.");
        else
                _myPowerErg[i]=_couplingFactor_Cond_1*_powerErgEq_1[i];
    }
    return _myPowerErg;
}

double* LaserClassCW::computePowerErgCond_3(double* _powerErgEq_3, const double &_couplingFactor_Cond_3)
{
    double* _myPowerErg=new double[n_lea];
    for(int i=0; i<n_lea; i++)
    {
            if((i==1)and((wavelength<400)or(wavelength>700)))
                _myPowerErg[i]=std::nan("N.A.");
        else
                _myPowerErg[i]=_couplingFactor_Cond_3*_powerErgEq_3[i];

    }
    return _myPowerErg;
}

ComputeLEA LaserClassCW::getLEA_Data()
{
    return myLaserClass;
}

LaserClassCW::laserClass LaserClassCW::getLaserClass() const
{
    return laserClassAssigned;
}

double LaserClassCW::getWavelength()const
{
    return wavelength;
}

void LaserClassCW::setWavelength(const double& _wavelength)
{
    if(_wavelength==wavelength)
        return;

    myLaserClass.setWavelength(_wavelength);
    wavelength=_wavelength;
}

void LaserClassCW::setPowerErg(const double& _powerErg)
{
    if(_powerErg==powerErg)
        return;

    powerErg=_powerErg;
}

double LaserClassCW::getPowerErg() const
{
    return powerErg;
}

double LaserClassCW::getAlpha() const
{
    return alpha;
}

void LaserClassCW::setAlpha(const double& _alpha)
{    
    if(_alpha==alpha)
        return;

    myLaserClass.setAlpha(_alpha);
    alpha=_alpha;
}

double LaserClassCW::getDivergence() const
{
    return divergence;
}

void LaserClassCW::setDivergence(const double& _divergence)
{
    if(_divergence==divergence)
        return;

    divergence=_divergence;
}

double LaserClassCW::getBeamDiameter() const
{
return beamDiameter;
}

void LaserClassCW::setBeamDiameter(const double& _beamDiameter)
{
    if(_beamDiameter==beamDiameter)
        return;

    beamDiameter=_beamDiameter;
}

double* LaserClassCW::getLEA()const
{
    return myLaserClass.getLEA();
}

string* LaserClassCW::getLEA_Formula()const
{
    return myLaserClass.getLEA_Formula();
}

string* LaserClassCW::getLEA_FormulaTipo()const
{
    return myLaserClass.getLEA_FormulaTipo();
}

string* LaserClassCW::getLEA_FormulaUnit()const
{
    return myLaserClass.getLEA_FormulaUnit();
}

int* LaserClassCW::getLEA_FormulaSort()const
{
    return myLaserClass.getLEA_FormulaSort();
}

double* LaserClassCW::getPowerErg_Cond_1()const
{
    return powerErg_Cond_1;
}

double* LaserClassCW::getPowerErg_Cond_3()const
{
    return powerErg_Cond_3;
}

double LaserClassCW::getC1() const
{
    return myLaserClass.getC1();
}

double LaserClassCW::getC2() const
{
    return myLaserClass.getC2();
}

double LaserClassCW::getC3() const
{
    return myLaserClass.getC3();
}

double LaserClassCW::getC4() const
{
    return myLaserClass.getC4();
}

double LaserClassCW::getC5() const
{
    return myLaserClass.getC5();
}

double LaserClassCW::getC6() const
{
    return myLaserClass.getC6();
}

double LaserClassCW::getC7() const
{
    return myLaserClass.getC7();
}

double LaserClassCW::getT1() const
{
    return myLaserClass.getT1();
}

double LaserClassCW::getT2() const
{
    return myLaserClass.getT2();
}

double LaserClassCW::getGamma() const
{
    return myLaserClass.getGamma();
}

string LaserClassCW::getRadiation() const
{
    return myLaserClass.getRadiation();
}

string LaserClassCW::getSkinDamage() const
{
    return myLaserClass.getSkinDamage();
}

string LaserClassCW::getEyeDamage() const
{
    return myLaserClass.getEyeDamage();
}

double LaserClassCW::getCouplingFactor_Cond_1()const
{
    return couplingFactor_Cond_1;
}

double LaserClassCW::getCouplingFactor_Cond_3()const
{
    return couplingFactor_Cond_3;
}

double LaserClassCW::getDistCond_1()const
{
   return distCond_1;
}

double LaserClassCW::getDistCond_3()const
{
   return distCond_3;
}

double LaserClassCW::getApCond_1()const
{
    return apCond_1;
}

double LaserClassCW::getApCond_3()const
{
    return apCond_3;
}

double LaserClassCW::getBeamAtStop_Cond_1()const
{
    return beamAtStop_Cond_1;
}

double LaserClassCW::getBeamAtStop_Cond_3()const
{
    return beamAtStop_Cond_3;
}

void LaserClassCW::updateAll()
{
    //Calcolo i valori di LEA
    myLaserClass.calculate();
    computeBeamArea();
    classUpdate(CW, timeBase, powerErg);
}
