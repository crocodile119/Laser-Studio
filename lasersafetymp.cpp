#include <cmath>
#include "lasersafetymp.h"

using namespace std;

LaserSafetyMP::LaserSafetyMP(double _PRF, double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                        double _pulseWidth, double _alpha):LaserSafetyCW(_beamDiameter, _powerErg, _divergence, _wavelength,
                        _pulseWidth, _alpha)
{
	//costruttore

    timeEditable=false;
	
		if(PRF> (1/Tmin))
			PRF=(1/Tmin);
		else
			PRF=_PRF;

    setExposureTime();

    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setAlpha(_alpha);
    setPulseWidth(_pulseWidth);
	//Imposto i parametri del laser relativi al singolo impulso
    myLaser.EMP();
    myLaser.getEMP();
    setTe();
	//Imposto i parametri del laser relativi al treno di impulsi nel tempo di funzionamento
    myMeanPower_Laser.EMP();
    myMeanPower_Laser.getEMP();
}

void LaserSafetyMP::setWavelength(const double& _wavelength)
{
    myMeanPower_Laser.setWavelength(_wavelength);
    myTmin_Laser.setWavelength(_wavelength);

    if(_wavelength==wavelength)
        return;

    myLaser.setWavelength(_wavelength);
    wavelength=_wavelength;
}


void LaserSafetyMP::setAlpha(const double& _alpha)
{   
    myMeanPower_Laser.setAlpha(_alpha);
    myTmin_Laser.setAlpha(_alpha);

    if(_alpha==alpha)
        return;

    myLaser.setAlpha(_alpha);
    alpha=_alpha;
}

void LaserSafetyMP::setPulseWidth(const double& _pulseWidth)
{
    myMeanPower_Laser.setPulseWidth(exposureTime);
    myTmin_Laser.setPulseWidth(Tmin);

    if(_pulseWidth==pulseWidth)
        return;

    myLaser.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}
	/*-------------------------------------------------------------------------------------------------------
	|MyMeanPower_Laser: oggetto che descrive il funzionamento del treno di impulsi per il tempo di funzionamento   |
	|MyLaser: oggetto che descrive il funzionamento del treno di impulsi per la durata del singolo impulso  |
	---------------------------------------------------------------------------------------------------------*/
 
  /*----------------------------------------------------------------------------------------------------------
   | Calcolo il valore medio della potenza del treno di impulsi                                              |
   --------------------------------------------------------------------------------------------------------- */
 
 void LaserSafetyMP::computeMeanPower()
{
    meanPower=powerErg*PRF;
}

 void LaserSafetyMP::computeMeanPowerEMP()
{	/*MyMeanPower_Laser è l'eggetto che descrive il comportamento del laser in onda continua e in questo caso del treno 
    per il tempo di funzionamento di impulsi*/
    myMeanPower_Laser.EMP();
    powerFormulaEMP=myMeanPower_Laser.getFormulaEMP();
    powerFormulaSort=myMeanPower_Laser.getFormulaSort();
    powerEMP=myMeanPower_Laser.getEMP();

        if(PRF!=0)
        {
            if(powerFormulaSort=="H")
                meanPow_EMP_Result=powerEMP/ceil(exposureTime*PRF);// calcola l'H medio come il rapporo dell'H in Te con N.
            else if(powerFormulaSort=="E")
                meanPow_EMP_Result=powerEMP/(PRF*pulseWidth);//calcola l'E medio come il rapporo dell'H medio con t.
        }
        else
        {
        meanPow_EMP_Result=powerEMP;//utile per evitare divisione per 0 quando la PRF si riduce a 0.
        }
 }

void LaserSafetyMP::computePulseNumber()
{	
	/*La funzione calcola il numero degli impulsi del treno come prodotto del Pulse Repetition Frequency e del tempo di esposizione.*/
    if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        if(PRF> (1/Tmin))
            pulseNumber=ceil((int)(0.5+(1/Tmin)*Te));//se il conteggio non è regolare il numero di impulsi è pari al rapporto del tempo di esposizione minimo tra T2 exposureTime con Tmin
            else
            pulseNumber=ceil(PRF*Te);//altrimenti è pari al prodotto della PRF con exposureTime.
         }
    else
        {
         pulseNumber=ceil(PRF*exposureTime);
        }
    /****************************************************************************************************
     *                                              ATTENZIONE                                          *
     * PulseNumber è un intero quindi risulta nullo se il prodotto di PRF*ExposureTime è minore di 1    *
     * **************************************************************************************************/
}

void LaserSafetyMP::computeCP()
{
    /***********************************************************************************************************************
    *La funzione calcola il coefficiente CP secondo la nota formula Cp=N^(-0-25)                                           *
    ************************************************************************************************************************/
    if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        if(pulseNumber!=0)
            CP=pow(pulseNumber, (-0.25));
            else
            CP=1;
        }
    else
        {
         CP=1;
        }
}

void LaserSafetyMP::setTe()
{
    if(PRF>(1/Tmin))
        T2=myTmin_Laser.getT2();
           else
        T2=myLaser.getT2();

    Te=std::min(exposureTime, T2);
}

double LaserSafetyMP::getTe()const
{
    return Te;
}

void LaserSafetyMP::computeTmin()
{
    /***********************************************************************************************************************
    *Si imposta il valore di Tmin in funzione della lunghezza d'onda      			                                       *
    ************************************************************************************************************************/
	
    if((wavelength>315) and (wavelength<=400))
        Tmin=1.0e-09;
	else
    if((wavelength>400) and (wavelength<=1050))
        Tmin=18e-06;
	else
    if((wavelength>1050) and (wavelength<=1400))
		Tmin=0.00005;
	else
    if((wavelength>1400) and (wavelength<=1500))
		Tmin=0.001;
	else
    if((wavelength>1500) and (wavelength<=1800))
		Tmin=10;	
		else
    if((wavelength>1800) and (wavelength<=2600))
		Tmin=0.001;
		else
    if((wavelength>2600) and (wavelength<=1.0e+06))
        Tmin=1.0e-07;;
}

string LaserSafetyMP::valutateCounting()
{
    /*************************************************************************************************************************
    *Si valuta il numero di impulsi da considerare. Se la PRF è maggiore di Tmin il numero di impulsi in Tmin è maggiore     *
    *di 1 mentre il numero di impulsi da contare è pari a uno. In questo caso il conteggio non è regolare ed il numero di    *
    *impulsi è pari al rapporto tra il tempo di esposizione e Tmin															 *
    **************************************************************************************************************************/
    string _Counting;

    if((wavelength>=400)and(wavelength<=1.0e+06))
    {
         if(PRF> (1/Tmin))
          _Counting="non regolare";
          else
          _Counting="regolare";
     }else
        {
           _Counting="regolare";
        }

	return _Counting;
}

double LaserSafetyMP::getTmin() const
{
	return Tmin;
}

void LaserSafetyMP::computeSP_CP_EMP()
{
    double SP_EMP_forCP;
	/*---------------------------------------------------------------------------------------------------------------------
	 | La funzione calcola l'EMP per il treno di impulsi considerando l'EMP del singolo impulso.                           | 
	 | MyLaser è l'oggetto che modella il funzionamento del singolo impulso. Si calcola innanzitutto l'EMP per sinogolo    |
	 | impulso SP_EMP_Result (SP sta per Single Pulse) successivamente si valutano gli effeti termici del treno calcolati  |
	 | moltiplicando SP_EMP_Result del singolo impulso per CP                                                              |
	 ----------------------------------------------------------------------------------------------------------------------*/
    //Imposto l'oggetto.
    myLaser.EMP();// calcolo l'EMP per il singolo impulso
    formula=myLaser.getFormulaEMP();
    formulaSort=myLaser.getFormulaSort();
    SP_EMP_Result=myLaser.getEMP();// calcolo l'EMP del singolo impulso

    if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        if(PRF>(1/Tmin))
            {
            myTmin_Laser.EMP();// calcolo l'EMP per il singolo impulso
            formula_Tmin=myTmin_Laser.getFormulaEMP();
            formulaSort_Tmin=myTmin_Laser.getFormulaSort();

            SP_EMP_forCP=myTmin_Laser.getEMP()/(PRF*Tmin);
            if(formulaSort=="E")
            {
                if(formulaSort_Tmin=="H")
                    SP_EMP_forCP=SP_EMP_forCP/Tmin;

            // calcolo l'EMP del singolo impulso che ha energia più elevata e quindi EMP più basso
            }
        }
        else
            {
            SP_EMP_forCP=SP_EMP_Result;// calcolo l'EMP del singolo impulso
            }
            setTe();
            computePulseNumber();
            computeCP();
            CP_EMP_Result=CP*SP_EMP_forCP;// calcolo l'EMP per considerare gli effetti termici del treno.
        }
    else{
    CP_EMP_Result=0;
    }
}
    /***********************************************************************************************************************
     * La funzione calcola l'irradianza media del treno di impulsi  utilizzando la funzione getBeamArea (si convertono i   *
     * millimetri in metri                                                                                                 *
     ***********************************************************************************************************************/

    void LaserSafetyMP::computeMeanPowerIrradiance()
{
	computeBeamArea();
    meanPowerIrradiance=meanPower/getBeamArea();
}

/*calcolo il valolre dell'EMP della potenza media del laser facendo riferimento all'esposizione radiante H
 Quindi in confronto andraè
*/


void LaserSafetyMP::equateMeanPowerEMP()
{
    /*********************************************************************************************************************************
    * la funzione esprime l'EMP medio del laser relativo al treno di impulsi per la durata di funzionamento con unità di misura      *
    * omogenea all'EMP del singolo impulso.																						     *
    * La funzione sarà impiegata per confrontare l'EMP riguardante le tre condizioni relative al funzionamento ad impulsi multipli   *
    **********************************************************************************************************************************/
	//se l'EMP del funzionamento ad impulso è espresso in esposizione radiante
    //MeanPow_EMP_Result è la H o la E media in Te
        if(formulaSort=="H")
		{
	//se l'EMP calcolato per il tempo di funzionamento è espresso in esposizione radiante esprimo l'emissione radiante in esposizione radiante 
            if(powerFormulaSort=="H")
                meanPow_EMP_Equate=meanPow_EMP_Result;
			else
	//altrimenti trasformo l'EMP in esposizione radiante un irradianza dividendo per il tempo di esposizione.
            if(powerFormulaSort=="E")
            {
                meanPow_EMP_Equate=meanPow_EMP_Result*pulseWidth;//ottengo il valore della radianza media da E
            }
		}
	// viceversa se l'EMP del tempo di funzionamento è espresso in irradianza
		else
        if(formulaSort=="E")
		{
            if(powerFormulaSort=="H")
            {
                meanPow_EMP_Equate=meanPow_EMP_Result/pulseWidth;//calcolo E da H dividendo per t.
            }
            else
            if(powerFormulaSort=="E")
            meanPow_EMP_Equate=meanPow_EMP_Result;
		}
}

double LaserSafetyMP::getMeanPower_EMP_Equate() const
{
return meanPow_EMP_Equate;
}

double LaserSafetyMP::returnMultiPulse_EMP()
{
    /*********************************************************************************************************************************
    * La funzione calcola il valore minimo tra i tre valori di EMP calcolati:														 *
    * SP_EMP_Result:		EMP dell'impulso singolo																				 *															 |
    * CP_EMP_Result:		EMP dell'impulso singolo corretto per considerare gli effetti termici									 *																								 |
    * MeanPow_EMP_Result:	EMP medio del treno di impulsi omogeneizzato all'unità di misura dell'EMP del singolo impulso.           *
    * L'EMP minimo sarà sempre omogeneo all'EMP dell'impulso singolo																 *					     |
    **********************************************************************************************************************************/
    if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        double m1;
        double m2;
        m1=fmin(SP_EMP_Result, CP_EMP_Result);
        m2=fmin(CP_EMP_Result, meanPow_EMP_Equate);
        EMP_Result=fmin(m1, m2);
        return EMP_Result;
    }
    else{
        EMP_Result=fmin(SP_EMP_Result, meanPow_EMP_Equate);
        return EMP_Result;
    }
}

void LaserSafetyMP::computeEMP_ForOD()
{
    if((wavelength>=400)and(wavelength<=1.0e+06))
        EMP_ForOD=fmin(SP_EMP_Result, CP_EMP_Result);
    else
        EMP_ForOD=SP_EMP_Result;
}

double LaserSafetyMP::getPowerErgForEMP()
{
    /*********************************************************************************************************************************
    * La funzione individua la potenza/energia impulso del laser corrispondente all'EMP minimo.:                                      *
    * In pratica se l'EMP minimo corrisponde al valore di potenza media (E_imp*PRF)	si considera questo valore come quello            *
    * significativo altrimenti si considera l'EMP che corrisponde al prodotto EPM*Cp. In ogni caso se l'EMP del singolo impulso è     *
    * espressa in esposizione radiante il parametro del laser corrispondente sarà espresso in energia (dell'impuslo o energia media)  *
    * mentre se l'EMP è espresso in irradianza il parametro del laser corrispondente sarà espresso in potenza.                        *
    **********************************************************************************************************************************/

    double powerErgForEMP;
    formulaSort=getFormulaSort();

        if(EMP_Result==SP_EMP_Result || EMP_Result==CP_EMP_Result)
        {
            if(formulaSort=="E")
                //powerErg è l'energia dell'impulso quindi per ricavare
                //una potenza devo dividere per la durata dell'impulso
                powerErgForEMP=powerErg/pulseWidth;
            else
                powerErgForEMP=powerErg;
        }
        else
            {
                if(formulaSort=="E")//MeanPow_EMP_Equate è omogenea a SP o CP
                powerErgForEMP=meanPow_EMP_Equate;
                else
                powerErgForEMP=meanPow_EMP_Equate*exposureTime;
            }

    return powerErgForEMP;
}

void LaserSafetyMP::computeMeanPower_NOHD()
   /**********************************************************************************************************
   * Calcolo dell'NOHD che deriverebbe dal solo effetto medio del treno di impulsi      				     *
   * Tale valore è pari all'irradianza emessa dal laser per la frequenza di ripetisioone degli impulsi       *
   ***********************************************************************************************************/
{
	double ForNOHD;
    powerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media
	
    if(powerFormulaSort=="E")
        ForNOHD=meanPower;// se l'EMP è espresso in irradianza si considera la potenza media
            else
        ForNOHD=meanPower*exposureTime;// altrimenti si ricava l'energia

		double underroot;
	//si applica la formula pe il calcolo della NOHD
        underroot=(4*ForNOHD)/(PI*powerEMP);//si impiega l'EMP per gli effetti medi
        meanPower_NOHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
        if (meanPower_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
            meanPower_NOHD=0;
}

void LaserSafetyMP::computeSinglePulse_NOHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo impulso singolo		         		      				     |
   ----------------------------------------------------------------------------------------------------------*/
{		
	double ForNOHD;
    formulaSort=getFormulaSort();
	
    if(formulaSort=="E")
           ForNOHD=powerErg/pulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
           ForNOHD=powerErg;// altrimenti si considera l'energia dell'impulso

		double underroot;
	//si applica la formula pe il calcolo della NOHD		
		underroot=(4*ForNOHD)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
        singlePulse_NOHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
        if (singlePulse_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
            singlePulse_NOHD=0;
}

void LaserSafetyMP::computePulseTrain_NOHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo dell'impulso singolo considerando gli effetti termici       |
   ----------------------------------------------------------------------------------------------------------*/
{
    if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        double ForNOHD;
        formulaSort=getFormulaSort();

        if(formulaSort=="E")
             ForNOHD=powerErg/pulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
             ForNOHD=powerErg;// altrimenti si considera l'energia dell'impulso

            double underroot;
        //si applica la formula pe il calcolo della NOHD
            underroot=(4*ForNOHD)/(PI*CP_EMP_Result);//si impiega l'EMP per gli effetti termici
            pulseTrain_NOHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
            if (pulseTrain_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
                pulseTrain_NOHD=0;
    }
    else{
        pulseTrain_NOHD=0;
    }
}

void LaserSafetyMP::computeNOHD()
{
	/*--------------------------------------------------------------------------------------------------------
    |La NOHD si calcola calcolando il massimo tra:															 |    
	|SinglePulse_NOHD																					 	 |
	|PulseTrain_NOHD																						 |
	|MeanPower_NOHD																							 |
    ---------------------------------------------------------------------------------------------------------*/
	double M1;
	double M2;

if((wavelength>=400)and(wavelength<=1.0e+06))
    {
        M1=fmax(meanPower_NOHD, singlePulse_NOHD);
        M2=fmax(singlePulse_NOHD, pulseTrain_NOHD);

        qDebug()<<"MeanPower_NOHD: "<<meanPower_NOHD;
        qDebug()<<"SinglePulse_NOHD: "<<singlePulse_NOHD;
        qDebug()<<"PulseTrain_NOHD: "<<pulseTrain_NOHD;

        NOHD=fmax(M1, M2);

        qDebug()<<"NOHD: "<<NOHD;
    }
    else
    {
        NOHD=fmax(meanPower_NOHD, singlePulse_NOHD);;
    }
}

	/*--------------------------------------------------------------------------------------------------------
	|Ricalcolo																								 |
    ---------------------------------------------------------------------------------------------------------*/

void LaserSafetyMP::computeMeanPowerLambertianMax()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo della distanza di sicurezza per riflessione su di un riflettore lamberiano. Il calcolo fa       |
    |riferimento al caso di riflessione puntiforme con direzione normale al diffusore e coeficiente di       |
    |diffusione unitario. Si considerano gli effetti che deriverebbero dal solo effetto medio del treno      |
    |di impulsi.                                                                         				     |
    |Tale valore è pari all'irradianza emessa dal laser per la frequenza di ripetisioone degli impulsi       |
   ----------------------------------------------------------------------------------------------------------*/
{
    double forLambertianMax;
    powerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media

    if(powerFormulaSort=="E")
        forLambertianMax=powerErg*PRF;// se l'EMP è espresso in irradianza si considera la potenza di picco
            else
        forLambertianMax=powerErg;// altrimenti si ricava l'esposizione radiante per la durata dell'impulso medio

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(forLambertianMax)/(PI*meanPow_EMP_Result);//si impiega l'EMP per gli effetti medi
        meanPowerLambertianMax=(sqrt(underroot));
}

void LaserSafetyMP::computeSinglePulseLambertianMax()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo della distanza di sicurezza per riflessione su di un riflettore lamberiano. Il calcolo fa       |
    |riferimento al caso di riflessione puntiforme con direzione normale al diffusore e coeficiente di       |
    |diffusione unitario. Si considerano gli effetti che deriverebbero dal solo impulso singolo.             |         		      				     |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForLambertianMax;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
           ForLambertianMax=powerErg/pulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
           ForLambertianMax=powerErg;// altrimenti si considera l'energia dell'impulso

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(ForLambertianMax)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
        singlePulseLambertianMax=(sqrt(underroot));
}

void LaserSafetyMP::computePulseTrainLambertianMax()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo dell'impulso singolo considerando gli effetti termici       |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForLambertianMax;
    formulaSort=getFormulaSort();

    if(formulaSort=="E")
         ForLambertianMax=powerErg/pulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
         else
         ForLambertianMax=powerErg;// altrimenti si considera l'energia dell'impulso

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(ForLambertianMax)/(PI*CP_EMP_Result);//si impiega l'EMP per gli effetti termici
        pulseTrainLambertianMax=(sqrt(underroot));
}

void LaserSafetyMP::computeLambertianMax()
{
    /*--------------------------------------------------------------------------------------------------------
    |Il massimo valore della distanza ottica per riflesione lambertiana                                      |
    |si calcola calcolando il massimo tra:                  												 |
    |SinglePulseLambertianMax																					 	 |
    |PulseTrainLambertianMax																						 |
    |MeanPowerLambertianMax																						 |
    ---------------------------------------------------------------------------------------------------------*/
    double M1;
    double M2;

    M1=fmax(meanPowerLambertianMax, singlePulseLambertianMax);
    M2=fmax(singlePulseLambertianMax, pulseTrainLambertianMax);
    lambertianMax=fmax(M1, M2);
}

void LaserSafetyMP::laserUpdate()
{
    computePulseNumber();
	computeMeanPower();
	computeMeanPowerIrradiance();

	computeMeanPowerEMP();
	computeSP_CP_EMP();
	equateMeanPowerEMP();
    computeEMP_ForOD();
	
	computeMeanPower_NOHD();
	computeSinglePulse_NOHD();
	computePulseTrain_NOHD();

    computeMeanPowerLambertianMax();
    computeSinglePulseLambertianMax();
    computePulseTrainLambertianMax();
	
	computeNOHD();

    computeLambertianMax();
}

	/*--------------------------------------------------------------------------------------------------------
	|Getters e Stters								 														 |
    ---------------------------------------------------------------------------------------------------------*/
string LaserSafetyMP::getMeanPowerFormulaSort()
{
    formulaSort=myMeanPower_Laser.getFormulaSort();
    return formulaSort;
}

string LaserSafetyMP::getMeanPowerFormulaEMP()
{
    formula=myMeanPower_Laser.getFormulaEMP();
    return formula;
}

double LaserSafetyMP::getCA_MP() const
{
    return myMeanPower_Laser.getCA();
}

double LaserSafetyMP::getCB_MP() const
{
    return myMeanPower_Laser.getCB();
}

double LaserSafetyMP::getCC_MP() const
{
    return myMeanPower_Laser.getCC();
}

double LaserSafetyMP::getCE_MP() const
{
    return myMeanPower_Laser.getCE();
}

double LaserSafetyMP::getT1_MP() const
{
    return myMeanPower_Laser.getT1();
}

double LaserSafetyMP::getT2_MP() const
{
    return myMeanPower_Laser.getT2();
}

double LaserSafetyMP::getGamma_MP() const
{
    return myMeanPower_Laser.getGamma();
}	

string LaserSafetyMP::getRadiation_MP() const
{
    return myMeanPower_Laser.getRadiation();
}

string LaserSafetyMP::getSkinDamage_MP() const
{
    return myMeanPower_Laser.getSkinDamage();
}

string LaserSafetyMP::getEyeDamage_MP() const
{
    return myMeanPower_Laser.getEyeDamage();
}

string LaserSafetyMP::getPhotochemicalNote_MP() const
{
    return myMeanPower_Laser.getPhotochemicalNote();
}

double LaserSafetyMP::getEMP_MP()
{
    return myMeanPower_Laser.getEMP();
}

void LaserSafetyMP::setPRF(const int& _PRF)
{
	PRF=_PRF;
}

double LaserSafetyMP::getPRF() const
{
	return PRF;
}

double LaserSafetyMP::getMeanPower() const
{
    return meanPower;
}

double LaserSafetyMP::getMeanIrradiance() const
{
    return meanPowerIrradiance;
}

double LaserSafetyMP::getThermalEMP() const
{
	return CP_EMP_Result;
}

double LaserSafetyMP::getCPCoefficient() const
{
	return CP;
}

double LaserSafetyMP::getPulseNumber() const
{
    return pulseNumber;
}

double LaserSafetyMP::getEMP_ForOD()const
{
    return EMP_ForOD;
}

double LaserSafetyMP::getEMP_1stCondition()const
{
    return SP_EMP_Result;
}

double LaserSafetyMP::getEMP_2ndCondition()const
{
    return meanPow_EMP_Result;
}

double LaserSafetyMP::getEMP_3rdCondition()const
{
    double SP_EMP_forCP;
    if(PRF>(1/Tmin))
    SP_EMP_forCP=myTmin_Laser.getEMP()*CP/(PRF*Tmin);
    else
    SP_EMP_forCP=myLaser.getEMP()*CP;

    return SP_EMP_forCP;
}

string LaserSafetyMP::getFormula_3rdCondition()
{
    if(PRF>(1/Tmin))
        formula_Tmin=myTmin_Laser.getFormulaEMP()+" /(PRF T<sub>min</sub>)";
    else
        formula_Tmin=myLaser.getFormulaEMP();

    return formula_Tmin;
}

string LaserSafetyMP::getFormulaSort_3rdCondition()
{
    if(PRF>(1/Tmin))
        formulaSort_Tmin=myTmin_Laser.getFormulaSort();
    else
        formulaSort_Tmin=myLaser.getFormulaSort();

    return formulaSort_Tmin;
}
