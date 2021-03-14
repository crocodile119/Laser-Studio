#include "laserclassmp.h"

LaserClassMP::LaserClassMP(double _prf, double _beamDiameter, double _powerErg,  double _divergence,
                           double _wavelength, double _pulseWidth, double _alpha)
                           :LaserClassSP(_beamDiameter, _powerErg, _divergence,_wavelength, _pulseWidth, _alpha)
{
    setTimeBase();
    timeBase=getTimeBase();

    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setPulseWidth(_pulseWidth);
    setPRF(_prf);

    meanPowerErgEq=new double[n_lea];
    meanPowerErg_Cond_1=new double[n_lea];
    meanPowerErg_Cond_3=new double[n_lea];
    meanClassValutation=new bool[n_class];
    meanLEA_Corrected=new double[n_lea];
    meanLEA_formulaSort=new int[n_lea];

    tiPowerErgEq=new double[n_lea];
    tiPowerErg_Cond_1=new double[n_lea];
    tiPowerErg_Cond_3=new double[n_lea];
    tiClassValutation=new bool[n_class];
    tiLEA_Value=new double[n_lea];
    tiLEA_Corrected=new double[n_lea];

    updateAll();
}

void LaserClassMP::meanClassUpdate(const double& _timeBase, const double& _meanPower)
    {
    //In base a ciacuno dei LEA delle varie classi trasformo l'unità di misura dell'uscita laser
    meanLEA_formulaSort=getMeanLEA_FormulaSort();

    meanPowerErgEq=powerErgUnit(meanLEA_formulaSort, _meanPower, _timeBase);
    //del laser calcolo le distanze e le aperture relative alle condizioni 1 e 3

    /****************************************************
     * prelevo le distanza calcolata per la condizione 1*
     * **************************************************/
     meanDistCond_1=myMeanLaserClass.getDistanceCond_1();
     meanApCond_1=myMeanLaserClass.getApertureStopCond_1();

    /*******************************************************************************
     * Per il calcolo del diametro del diametro del fascio e del fattore di        *
     * accoppiamento per la condizione 1 è necessario verificare quand'è che       *
     * risulta applicabile (lunghezza d'onda >=302.5 e <=4000).                    *                                      *
     * *****************************************************************************/

    if((wavelength>=302.5)and(wavelength<=4000)){
    /**************************************************************************
     * Calcolo il diametro del fascio alla stessa distanza della condizione 1 *                                                *
     **************************************************************************/
        meanBeamAtStop_Cond_1=valuateBeamDiameterAtStop(meanDistCond_1, divergence);
    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/

        meanCouplingFactor_Cond_1=valuateCouplingFactor(meanApCond_1, meanBeamAtStop_Cond_1);
        }
        else
            {
             meanCouplingFactor_Cond_1=1.0;
             meanBeamAtStop_Cond_1=std::nan("N.A.");
            }

    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/

    if(internalWaist)
        meanDistCond_3=0.0;
    else
        meanDistCond_3=myMeanLaserClass.getDistanceCond_3();

    meanBeamAtStop_Cond_3=valuateBeamDiameterAtStop(meanDistCond_3, divergence);

    meanApCond_3=myMeanLaserClass.getApertureStopCond_3();
    meanCouplingFactor_Cond_3=valuateCouplingFactor(meanApCond_3, meanBeamAtStop_Cond_3);

    /****************************************************************************************************************
     * Correggo in calori di LEA per ottenerne quelli medi.                                                         *
     * Si ricorda a tal proposito che:                                                                              *
     *  - la potenza media in uscita dal laser è pari a Q*prf;                                                      *
     *  - se il valore del LEA in timeBase è espresso in E allora il suo valore medio è pari a E/(prt*pulseWidth);  *
     *  - se il valore del LEA in timeBase è espresso in H allora il suo valore medio è pari a H/(prf*timeBase);    *
     *  - se il valore del LEA in timeBase è espresso in W allora il suo valore medio è proprio a Q*prf;                *
     *  - se il valore del LEA in timeBase è espresso in J allora il suo valore medio è pari Q*prf*pulseWidth.   *
    *****************************************************************************************************************/

    meanLEA_Corrected=computeMeanLEA_Corrected(meanLEA_formulaSort);

    /******************************************************************
     * correggo i valori relativi all'uscita del laser per i fattori  *
     * di accoppiamento corrispondenti                                *
     ******************************************************************/
    /******************************************************************
     * correggo i valori relativi all'uscita del laser per i fattori  *
     * di accoppiamento corrispondenti                                *
     ******************************************************************/
    meanPowerErg_Cond_1=computePowerErgCond_1(meanPowerErgEq, meanCouplingFactor_Cond_1);
    meanPowerErg_Cond_3=computePowerErgCond_3(meanPowerErgEq, meanCouplingFactor_Cond_3);

    /***********************************
     * valuto la classe corrispondente *
     ***********************************/
    meanClassValutation=valuateLEA_forClass(meanPowerErg_Cond_1, meanPowerErg_Cond_3,
                                       meanLEA_Corrected);

    /******************************
     * Ricavo la classe calcolata *
     ******************************/
    meanLaserClassAssigned=valuateClass(meanClassValutation);
}

void LaserClassMP::c5ClassUpdate()
    {

    LEA_Corrected=computeLEA_ThermalCorrection(myLaserClass.getLEA());
    c5ClassValutation=valuateLEA_forClass(powerErg_Cond_1, powerErg_Cond_3,
                                        LEA_Corrected);

    /******************************
     * Ricavo la classe calcolata *
     ******************************/
    c5LaserClassAssigned=valuateClass(c5ClassValutation);
}

void LaserClassMP::tiClassUpdate(const double& _Tmin, const double& _powerErg)
    {
    //In base a ciacuno dei LEA delle varie classi trasformo l'unità di misura dell'uscita laser
    tiPowerErgEq=powerErgUnit(myTiLaserClass.getLEA_FormulaSort(), _powerErg, _Tmin);
    //del laser calcolo le distanze e le aperture relative alle condizioni 1 e 3

    /****************************************************
     * prelevo le distanza calcolata per la condizione 1*
     * **************************************************/
     tiDistCond_1=myTiLaserClass.getDistanceCond_1();
     tiApCond_1=myTiLaserClass.getApertureStopCond_1();

    /*******************************************************************************
     * Per il calcolo del diametro del diametro del fascio e del fattore di        *
     * accoppiamento per la condizione 1 è necessario verificare quand'è che       *
     * risulta applicabile (lunghezza d'onda >=302.5 e <=4000).                    *                                      *
     * *****************************************************************************/

    if((wavelength>=302.5)and(wavelength<=4000)){
    /**************************************************************************
     * Calcolo il diametro del fascio alla stessa distanza della condizione 1 *                                                *
     **************************************************************************/
        tiBeamAtStop_Cond_1=valuateBeamDiameterAtStop(tiDistCond_1, divergence);
    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/

        tiCouplingFactor_Cond_1=valuateCouplingFactor(tiApCond_1, tiBeamAtStop_Cond_1);
        }
        else
            {
             tiCouplingFactor_Cond_1=1.0;
             tiBeamAtStop_Cond_1=std::nan("N.A.");
            }

    /****************************************************************************
     * prelevo l'apertura calcolata per la condizione 1 e calcolo il fattore di *
     * accoppiamento corrispondente                                             *
     ****************************************************************************/

    if(internalWaist)
        tiDistCond_3=0.0;
    else
        tiDistCond_3=myTiLaserClass.getDistanceCond_3();

    tiBeamAtStop_Cond_3=valuateBeamDiameterAtStop(tiDistCond_3, divergence);

    tiApCond_3=myTiLaserClass.getApertureStopCond_3();
    tiCouplingFactor_Cond_3=valuateCouplingFactor(tiApCond_3, tiBeamAtStop_Cond_3);

    /******************************************************************
     * correggo i valori relativi all'uscita del laser per il fattore *
     * di correzione termica C5                                       *
     ******************************************************************/

    tiLEA_Value=getTiLEA();
    tiLEA_Corrected=computeLEA_ThermalCorrection(tiLEA_Value);

    /******************************************************************
     * correggo i valori relativi all'uscita del laser per i fattori  *
     * di accoppiamento corrispondenti                                *
     ******************************************************************/

    tiPowerErg_Cond_1=computePowerErgCond_1(tiPowerErgEq, tiCouplingFactor_Cond_1);
    tiPowerErg_Cond_3=computePowerErgCond_3(tiPowerErgEq, tiCouplingFactor_Cond_3);

    /***********************************
     * valuto la classe corrispondente *
     ***********************************/
    tiClassValutation=valuateLEA_forClass(tiPowerErg_Cond_1, tiPowerErg_Cond_3,
                                       tiLEA_Corrected);

    /******************************
     * Ricavo la classe calcolata *
     ******************************/
    tiLaserClassAssigned=valuateClass(tiClassValutation);
}


void LaserClassMP::computeTi()
{
    /***********************************************************************************************************************
    *Si imposta il valore di Tmin in funzione della lunghezza d'onda      			                                       *
    ************************************************************************************************************************/

    if((wavelength>400) and (wavelength<=1050))
        Ti=5e-06;
    else
    if((wavelength>1050) and (wavelength<=1400))
        Ti=13e-06;
    else
    if((wavelength>1400) and (wavelength<=1500))
        Ti=0.001;
    else
    if((wavelength>1500) and (wavelength<=1800))
        Ti=10;
        else
    if((wavelength>1800) and (wavelength<=2600))
        Ti=0.001;
        else
    if((wavelength>2600) and (wavelength<=1.0e+06))
        Ti=1.0e-07;;
}

void LaserClassMP::computeMeanPower()
{
   meanPower=powerErg*prf;
}

void LaserClassMP::setTe()
{
    T2=getT2();
    Te=std::min(timeBase, T2);
}

double LaserClassMP::getTe()const
{
    return Te;
}

void LaserClassMP::computePulseNumber()
{
    /*La funzione calcola il numero degli impulsi del treno come prodotto del Pulse Repetition Frequency e del tempo di esposizione.*/
    if((wavelength>=400)and(wavelength<=1400))
        {
        if(prf> (1/Ti)){
            pulseNumber=ceil((int)(0.5+(1/Ti)*Te));}//se il conteggio non è regolare il numero di impulsi è pari al rapporto del tempo di esposizione con Te con Ti
            else
            pulseNumber=ceil(prf*Te);//altrimenti è pari al prodotto della PRF con il tempo di esposizione Te.
         }
    else
        {
         pulseNumber=ceil(prf*timeBase);
        }
    /****************************************************************************************************
     *                                              ATTENZIONE                                          *
     * PulseNumber è un intero quindi risulta nullo se il prodotto di PRF*ExposureTime è minore di 1    *
     * **************************************************************************************************/
}

void LaserClassMP::computeC5()
{
    /***********************************************************************************************************************
    *La funzione calcola il coefficiente CP secondo la nota formula Cp=N^(-0-25)                                           *
    ************************************************************************************************************************/
    if((wavelength>=400)and(wavelength<=1400)and(pulseWidth<=0.25))
    {
        if(pulseWidth<=Ti){
            if(timeBase<=0.25)
                C5=1;
                else{
                    if(pulseNumber<=600)
                    C5=1;
                    else{
                        C5=5*std::pow(pulseNumber, -0.25);
                        if(C5<0.4)
                        C5=0.4;
                        }
                    }
                }
        if(pulseWidth>Ti){
            if((alpha<=5)or(alpha>100))
            C5=1;
            else if((alpha>5)and(alpha<=alpha_max))
                {
                if(pulseNumber<=40)
                    C5=std::pow(pulseNumber, -0.25);
                    else
                    C5=0.4;
                }else if(alpha>alpha_max){
                        if(pulseNumber<=625)
                    C5=std::pow(pulseNumber, -0.25);
                    else
                    C5=0.2;
                    }
                }
    }
}

string LaserClassMP::valutateCounting()
{
    /*************************************************************************************************************************
    *Si valuta il numero di impulsi da considerare. Se la PRF è maggiore di Tmin il numero di impulsi in Tmin è maggiore     *
    *di 1 mentre il numero di impulsi da contare è pari a uno. In questo caso il conteggio non è regolare ed il numero di    *
    *impulsi è pari al rapporto tra il tempo di esposizione e Tmin															 *
    **************************************************************************************************************************/
    string _Counting;

    if((wavelength>=400)and(wavelength<=1400))
    {
         if(prf> (1/Ti))
          _Counting="non regolare";
          else
          _Counting="regolare";
     }else
        {
           _Counting="regolare";
        }

    return _Counting;
}

double LaserClassMP::getTi() const
{
    return Ti;
}

void LaserClassMP::setPRF(const double &_prf)
{
    prf=_prf;
}

double LaserClassMP::getPRF()const
{
    return prf;
}

void LaserClassMP::setTimeBase()
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
        myMeanLaserClass.setPulseWidth(timeBase);
}


double* LaserClassMP::powerErgUnit(int* _meanLEA_formulaSort, const double &_powerErg, const double &time)
{
    double* myPowerErgEq=new double[n_lea];
    for(int i=0; i<n_lea; i++)
    {
    if(_meanLEA_formulaSort[i]==1)
        myPowerErgEq[i]=_powerErg/beamArea;
    else if(_meanLEA_formulaSort[i]==2)
        myPowerErgEq[i]=_powerErg*time/beamArea;
    else if(_meanLEA_formulaSort[i]==3)
        myPowerErgEq[i]=_powerErg;
    else if(_meanLEA_formulaSort[i]==4)
        myPowerErgEq[i]=_powerErg*time;
    }
    return myPowerErgEq;
}

double* LaserClassMP::computeMeanLEA_Corrected(int* _meanLEA_formulaSort)
{
    double* meanLEACorrected=new double[n_lea];
    double*  myMeanLEA=getMeanLEA();
    for(int i=0; i<n_lea; i++)
    {
    if(_meanLEA_formulaSort[i]==1)
        meanLEACorrected[i]=myMeanLEA[i]/(prf*pulseWidth);
    else if(_meanLEA_formulaSort[i]==2)
        meanLEACorrected[i]=myMeanLEA[i]/ceil(prf*timeBase);
    else if(_meanLEA_formulaSort[i]==3)
        meanLEACorrected[i]=myMeanLEA[i];
    else if(_meanLEA_formulaSort[i]==4)
        meanLEACorrected[i]=myMeanLEA[i]*pulseWidth;
    }

    return meanLEACorrected;
}

void LaserClassMP::setWavelength(const double& _wavelength)
{
    myMeanLaserClass.setWavelength(_wavelength);
    myTiLaserClass.setWavelength(_wavelength);

    if(_wavelength==wavelength)
        return;

    myLaserClass.setWavelength(_wavelength);
    wavelength=_wavelength;
}


void LaserClassMP::setAlpha(const double& _alpha)
{
    myMeanLaserClass.setAlpha(_alpha);
    myTiLaserClass.setAlpha(_alpha);

    if(_alpha==alpha)
        return;

    myLaserClass.setAlpha(_alpha);
    alpha=_alpha;
}

void LaserClassMP::setPulseWidth(const double& _pulseWidth)
{
    myMeanLaserClass.setPulseWidth(timeBase);
    myTiLaserClass.setPulseWidth(Ti);

    if(_pulseWidth==pulseWidth)
        return;

    myLaserClass.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}


void LaserClassMP::highFrequencyValuation()
{
        myTiLaserClass.setPulseWidth(Ti);
        myTiLaserClass.calculate();
        myTiLaserClass.applyLEA_CorrectionFactor(1/(prf*Ti));
        computeBeamArea();
        tiClassUpdate(Ti, powerErg);
}

double* LaserClassMP::computeLEA_ThermalCorrection(double* _LEA_Value)
{
    double* _LEA_ThermalCorrected=new double[n_lea];
    for(int i=0; i<n_lea; i++)
    {
        _LEA_ThermalCorrected[i]=C5*_LEA_Value[i];
    }
    return _LEA_ThermalCorrected;
}

void LaserClassMP::updateAll()
{
    computeTi();
    computePulseNumber();
    computeC5();
    myLaserClass.calculate();
    classUpdate(PULSED, pulseWidth, powerErg);

    setTimeBase();
    computeBeamArea();
    computeMeanPower();
    myMeanLaserClass.calculate();
    setTe();
    meanClassUpdate(timeBase, meanPower);

    if((wavelength>=400)and(wavelength<=1400))
    {
        if(prf<(1/Ti)){
            setAlpha_max(pulseWidth);
            computeC5();
            c5ClassUpdate();
            systemClassValutation=valuateSystemClass(classValutation, meanClassValutation, c5ClassValutation);
        }
         else{
            setAlpha_max(Ti);
            computeC5();
            highFrequencyValuation();
            systemClassValutation=valuateSystemClass(classValutation, meanClassValutation, tiClassValutation);
        }
    }
    else
    systemClassValutation=valuateSystemClass(classValutation, meanClassValutation);

    systemLaserClassAssigned=valuateClass(systemClassValutation);
}


LaserClassCW::laserClass LaserClassMP::getSystemClassValutation()const
{
    return systemLaserClassAssigned;
}

void LaserClassMP::setAlpha_max(const double &pulse)
{
    if(pulse<6.25e-004)
        alpha_max=5;
        else if((pulse>=6.25e-004)and(pulse<0.25))
            alpha_max=200*pow(pulse, 0.5);
                else if(pulse>0.25)
                alpha_max=100;
}

bool* LaserClassMP::valuateSystemClass(bool* myClassValuation, bool* myMeanClassValuation, bool* myC5ClassValuation)
{
    bool* systemClass=new bool[n_class];
    for(int i=0; i<n_class; i++)
    {systemClass[i]=false;}

    if((wavelength>=400)and(wavelength<=1400)){
    for(int i=0; i<n_class; i++){
        if(myClassValuation[n_class-i-1] or myMeanClassValuation[n_class-i-1] or myC5ClassValuation[n_class-i-1])
            {
            systemClass[n_class-i-1]=true;
            break;
            }
        }
    }
    else{
    for(int i=0; i<n_class; i++){
        if(myClassValuation[n_class-i-1] or myMeanClassValuation[n_class-i-1])
            {
            systemClass[n_class-i-1]=true;
            break;
            }
        }
    }
    return systemClass;
}

double *LaserClassMP::getLEA_Corrected()const
{
    return LEA_Corrected;
}


double* LaserClassMP::getMeanLEA()const
{
    return myMeanLaserClass.getLEA();
}

string* LaserClassMP::getMeanLEA_Formula()const
{
    return myMeanLaserClass.getLEA_Formula();
}

string* LaserClassMP::getMeanLEA_FormulaTipo()const
{
    return myMeanLaserClass.getLEA_FormulaTipo();
}

string* LaserClassMP::getMeanLEA_FormulaUnit()const
{
    return myMeanLaserClass.getLEA_FormulaUnit();
}

int* LaserClassMP::getMeanLEA_FormulaSort()const
{
    return myMeanLaserClass.getLEA_FormulaSort();
}

double* LaserClassMP::getMeanPowerErgEq()const
{
    return meanPowerErgEq;
}

QString* LaserClassMP::getMeanLEA_Expressions()const
{
    QString* leaExpr=new QString[n_lea];

    for(int i=0; i<n_lea; i++)
    {
        leaExpr[i]=QString::fromStdString(getMeanLEA_FormulaTipo()[i])+" = "+
                  QString::fromStdString(getMeanLEA_Formula()[i]) + " = "+
                  QString::number(getMeanLEA()[i],'e', 2) +" "+
                  QString::fromStdString(getMeanLEA_FormulaUnit()[i]);
    }
    return leaExpr;
}

bool* LaserClassMP::getMeanClassValutation()const
{
    return meanClassValutation;
}

double LaserClassMP::getMeanDistCond_1()const
{
    return meanDistCond_1;
}

double LaserClassMP::getMeanDistCond_3()const
{
    return meanDistCond_3;
}

double LaserClassMP::getMeanApCond_1()const
{
    return meanApCond_1;
}

double LaserClassMP::getMeanApCond_3()const
{
    return meanApCond_3;
}

double LaserClassMP::getMeanBeamAtStop_Cond_1()const
{
    return meanBeamAtStop_Cond_1;
}

double LaserClassMP::getMeanBeamAtStop_Cond_3()const
{
    return meanBeamAtStop_Cond_3;
}

double LaserClassMP::getMeanCouplingFactor_Cond_1()const
{
    return meanCouplingFactor_Cond_1;
}

double LaserClassMP::getMeanCouplingFactor_Cond_3()const
{
    return meanCouplingFactor_Cond_3;
}

double* LaserClassMP::getMeanPowerErg_Cond_1()const
{
    return meanPowerErg_Cond_1;
}

double* LaserClassMP::getMeanPowerErg_Cond_3()const
{
    return meanPowerErg_Cond_3;
}

string LaserClassMP::getMeanLaserClassAssigned()const
{
    return meanLaserClassAssigned;
}

double *LaserClassMP::getMeanLEA_Corrected()const
{
    return meanLEA_Corrected;
}

string LaserClassMP::getC5LaserClassAssigned()const
{
    return c5LaserClassAssigned;
}

double* LaserClassMP::getTiLEA()const
{
    return myTiLaserClass.getLEA();
}

string* LaserClassMP::getTiLEA_Formula()const
{
    return myTiLaserClass.getLEA_Formula();
}

string* LaserClassMP::getTiLEA_FormulaTipo()const
{
    return myTiLaserClass.getLEA_FormulaTipo();
}

string* LaserClassMP::getTiLEA_FormulaUnit()const
{
    return myTiLaserClass.getLEA_FormulaUnit();
}

int* LaserClassMP::getTiLEA_FormulaSort()const
{
    return myTiLaserClass.getLEA_FormulaSort();
}

QString* LaserClassMP::getTiLEA_Expressions()const
{
    QString* leaExpr=new QString[n_lea];

    for(int i=0; i<n_lea; i++)
    {
        leaExpr[i]=QString::fromStdString(getTiLEA_FormulaTipo()[i])+" = "+
                QString::fromStdString(getTiLEA_Formula()[i]) +" = "+
                QString::number(getTiLEA()[i], 'e', 2) +" "+
                QString::fromStdString(getTiLEA_FormulaUnit()[i]);
    }
    return leaExpr;
}

QString* LaserClassMP::getThermalLEA_Expressions()const
{
    QString* leaExpr=new QString[n_lea];

    for(int i=0; i<n_lea; i++)
    {

        leaExpr[i]=QString::fromStdString(getLEA_FormulaTipo()[i])+" = "+
                   QString::fromStdString(getLEA_Formula()[i]) +" C<sub>5</sub> = "+
                   QString::number(LEA_Corrected[i], 'e', 2) +" "+
                   QString::fromStdString(getLEA_FormulaUnit()[i]);
    }
    return leaExpr;
}
double* LaserClassMP::getTiPowerErgEq()const
{
    return tiPowerErgEq;
}

bool* LaserClassMP::getTiClassValutation()const
{
    return tiClassValutation;
}

double LaserClassMP::getTiDistCond_1()const
{
    return tiDistCond_1;
}

double LaserClassMP::getTiDistCond_3()const
{
    return tiDistCond_3;
}

double LaserClassMP::getTiApCond_1()const
{
    return tiApCond_1;
}

double LaserClassMP::getTiApCond_3()const
{
    return tiApCond_3;
}

double LaserClassMP::getTiBeamAtStop_Cond_1()const
{
    return tiBeamAtStop_Cond_1;
}

double LaserClassMP::getTiBeamAtStop_Cond_3()const
{
    return tiBeamAtStop_Cond_3;
}

double LaserClassMP::getTiCouplingFactor_Cond_1()const
{
    return tiCouplingFactor_Cond_1;
}

double LaserClassMP::getTiCouplingFactor_Cond_3()const
{
    return tiCouplingFactor_Cond_3;
}

double* LaserClassMP::getTiPowerErg_Cond_1()const
{
    return tiPowerErg_Cond_1;
}

double* LaserClassMP::getTiPowerErg_Cond_3()const
{
    return tiPowerErg_Cond_3;
}

string LaserClassMP::getTiLaserClassAssigned()const
{
    return tiLaserClassAssigned;
}
double *LaserClassMP::getTiLEA_Corrected()const
{
    return tiLEA_Corrected;
}

double LaserClassMP::getMeanPower() const
{
    return meanPower;
}

double LaserClassMP::getC5Coefficient() const
{
    return C5;
}

double LaserClassMP::getPulseNumber() const
{
    return pulseNumber;
}
