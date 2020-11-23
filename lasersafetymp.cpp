#include <cmath>
#include "lasersafetymp.h"

using namespace std;

LaserSafetyMP::LaserSafetyMP(int _PRF, double _BeamDiameter, double _PowerErg,  double _Divergence, double _Wavelength,
                        double _PulseWidth, double _Alpha):LaserSafetyCW(_BeamDiameter, _PowerErg, _Divergence, _Wavelength,
						_PulseWidth, _Alpha)
{
	//costruttore
	BeamDiameter=_BeamDiameter; 
	PowerErg=_PowerErg;
	Divergence=_Divergence;
	Wavelength=_Wavelength;
	Alpha=_Alpha;
    PulseWidth=_PulseWidth;
    timeEditable=false;
	
		if(PRF> (1/Tmin))
			PRF=(1/Tmin);
		else
			PRF=_PRF;

    setExposureTime();
	//Imposto i parametri del laser relativi al singolo impulso
    MyLaser.setWavelength(Wavelength);
    MyLaser.setAlpha(Alpha);
    MyLaser.setPulseDuration(PulseWidth);
	MyLaser.EMP();
	MyLaser.getEMP();
	//Imposto i parametri del laser relativi al treno di impulsi nel tempo di funzionamento
	MyMeanPower_Laser.setWavelength(Wavelength);
	MyMeanPower_Laser.setAlpha(Alpha);
	MyMeanPower_Laser.setPulseDuration(getExposureTime());
	MyMeanPower_Laser.EMP();
	MyMeanPower_Laser.getEMP();
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
	MeanPower=PowerErg*PRF;
}

 void LaserSafetyMP::computeMeanPowerEMP()
{	/*MyMeanPower_Laser è l'eggetto che descrive il comportamento del laser in onda continua e in questo caso del treno 
	per il tempo di funzionamento di impulsi*/
	MyMeanPower_Laser.setWavelength(Wavelength);
	MyMeanPower_Laser.setAlpha(Alpha);
    MyMeanPower_Laser.setPulseDuration(exposureTime);// il termpo di esposizione di considera la durata del treno
    MyMeanPower_Laser.EMP();
	PowerFormulaEMP=MyMeanPower_Laser.getFormulaEMP();
	PowerFormulaSort=MyMeanPower_Laser.getFormulaSort();
    PowerEMP=MyMeanPower_Laser.getEMP();

        if(PRF!=0)
        {
            if(PowerFormulaSort=="H")
                MeanPow_EMP_Result=PowerEMP/(exposureTime*PRF);// calcola l'H medio come il rapporo dell'H in Te con N.
            else if(PowerFormulaSort=="E")
                MeanPow_EMP_Result=PowerEMP/(PRF*PulseWidth);//calcola l'E medio come il rapporo dell'H medio con t.
        }
        else
        {
        MeanPow_EMP_Result=PowerEMP;//utile per evitare divisione per 0 quando la PRF si riduce a 0.
        }
 }

void LaserSafetyMP::computePulseNumber()
{	
	/*La funzione calcola il numero degli impulsi del treno come prodotto del Pulse Repetition Frequency e del tempo di esposizione.*/
    if((Wavelength>=400)and(Wavelength<=1.0e+06))
        {
        if(PRF> (1/Tmin))
            PulseNumber=(int)(0.5+(1/Tmin)*exposureTime);//se il conteggio non è regolare il numero di impulsi è pari al rapporto del tempo di esposizione con Tmin
            else
            PulseNumber=PRF*exposureTime;//altrimenti è pari al prodotto della PRF con il tempo di esposizione.
         }
    else
        {
         PulseNumber=PRF*exposureTime;
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
    if((Wavelength>=400)and(Wavelength<=1.0e+06))
        {
        if(PulseNumber!=0)
            CP=pow(PulseNumber, (-0.25));
            else
            CP=1;
        }
    else
        {
         CP=1;
        }
}

void LaserSafetyMP::computeTmin()
{
    /***********************************************************************************************************************
    *Si imposta il valore di Tmin in funzione della lunghezza d'onda      			                                       *
    ************************************************************************************************************************/
	
	if((Wavelength>315) and (Wavelength<=400))
        Tmin=1.0e-09;
	else
	if((Wavelength>400) and (Wavelength<=1050))
        Tmin=18e-06;
	else
	if((Wavelength>1050) and (Wavelength<=1400))
		Tmin=0.00005;
	else
	if((Wavelength>1400) and (Wavelength<=1500))
		Tmin=0.001;
	else
	if((Wavelength>1500) and (Wavelength<=1800))
		Tmin=10;	
		else
	if((Wavelength>1800) and (Wavelength<=2600))
		Tmin=0.001;
		else
    if((Wavelength>2600) and (Wavelength<=1.0e+06))
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

    if((Wavelength>=400)and(Wavelength<=1.0e+06))
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
	MyLaser.setWavelength(Wavelength);
	MyLaser.setAlpha(Alpha);
	MyLaser.setPulseDuration(getPulseWidth());
	MyLaser.EMP();// calcolo l'EMP per il singolo impulso
	Formula=MyLaser.getFormulaEMP();
	FormulaSort=MyLaser.getFormulaSort();
    SP_EMP_Result=MyLaser.getEMP();// calcolo l'EMP del singolo impulso

    if((Wavelength>=400)and(Wavelength<=1.0e+06))
        {
        if(PRF>(1/Tmin))
            {
            MyTmin_Laser.setWavelength(Wavelength);
            MyTmin_Laser.setAlpha(Alpha);
            MyTmin_Laser.setPulseDuration(Tmin);
            MyTmin_Laser.EMP();// calcolo l'EMP per il singolo impulso
            Formula_Tmin=MyTmin_Laser.getFormulaEMP();
            FormulaSort_Tmin=MyTmin_Laser.getFormulaSort();
            MyTmin_Laser.EMP();
            SP_EMP_forCP=MyTmin_Laser.getEMP()/(PRF*Tmin);
            if(FormulaSort=="E")
            {
                if(FormulaSort_Tmin=="H")
                    SP_EMP_forCP=SP_EMP_forCP/Tmin;

            // calcolo l'EMP del singolo impulso che ha energia più elevata e quindi EMP più basso
            }
        }
        else
            {
            SP_EMP_forCP=SP_EMP_Result;// calcolo l'EMP del singolo impulso
            }

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
	MeanPowerIrradiance=MeanPower*1000000/getBeamArea();
}

/*calcolo il valolre dell'EMP della potenza media del laser facendo riferimento all'esposizione radiante H
 Quindi in confronto andraè
*/


void LaserSafetyMP::equateMeanPowerEMP()
{
    /*********************************************************************************************************************************
    * la funzione esprime l'EMP medio del laser relativo al treno di impulsi per la durata di funzionamento con unità di misura      *
    * omogenea all'EMP del singolo impulso.																						     *
    * La funzione sarà impiegata per confrontare l'EMP riguardante le tre condizioni relative al funzionamento ad impulsi ripetuti   *
    **********************************************************************************************************************************/
	//se l'EMP del funzionamento ad impulso è espresso in esposizione radiante
    //MeanPow_EMP_Result è la H o la E media in Te
		if(FormulaSort=="H")
		{
	//se l'EMP calcolato per il tempo di funzionamento è espresso in esposizione radiante esprimo l'emissione radiante in esposizione radiante 
			if(PowerFormulaSort=="H")
                MeanPow_EMP_Equate=MeanPow_EMP_Result;
			else
	//altrimenti trasformo l'EMP in esposizione radiante un irradianza dividendo per il tempo di esposizione.
			if(PowerFormulaSort=="E")
            {
                MeanPow_EMP_Equate=MeanPow_EMP_Result*PulseWidth;//ottengo il valore della radianza media da E
            }
		}
	// viceversa se l'EMP del tempo di funzionamento è espresso in irradianza
		else
		if(FormulaSort=="E")
		{
			if(PowerFormulaSort=="H")
            {
                MeanPow_EMP_Equate=MeanPow_EMP_Result/PulseWidth;//calcolo E da H dividendo per t.
            }
            else
			if(PowerFormulaSort=="E")
			MeanPow_EMP_Equate=MeanPow_EMP_Result;
		}
}

double LaserSafetyMP::getMeanPower_EMP_Equate() const
{
return MeanPow_EMP_Equate;
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
    if((Wavelength>=400)and(Wavelength<=1.0e+06))
        {
        double m1;
        double m2;
        m1=fmin(SP_EMP_Result, CP_EMP_Result);
        m2=fmin(CP_EMP_Result, MeanPow_EMP_Equate);
        EMP_Result=fmin(m1, m2);
        return EMP_Result;
    }
    else{
        EMP_Result=fmin(SP_EMP_Result, MeanPow_EMP_Equate);
        return EMP_Result;
    }
}

void LaserSafetyMP::computeEMP_ForOD()
{
    if((Wavelength>=400)and(Wavelength<=1.0e+06))
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
    FormulaSort=getFormulaSort();

        if(EMP_Result==SP_EMP_Result || EMP_Result==CP_EMP_Result)
        {
            if(FormulaSort=="E")
                //powerErg è l'energia dell'impulso quindi per ricavare
                //una potenza devo dividere per la durata dell'impulso
                powerErgForEMP=PowerErg/PulseWidth;
            else
                powerErgForEMP=PowerErg;
        }
        else
            {
                if(FormulaSort=="E")//MeanPow_EMP_Equate è omogenea a SP o CP
                powerErgForEMP=MeanPow_EMP_Equate;
                else
                powerErgForEMP=MeanPow_EMP_Equate*exposureTime;
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
	PowerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media
	
	if(PowerFormulaSort=="E")
        ForNOHD=MeanPower;// se l'EMP è espresso in irradianza si considera la potenza media
            else
        ForNOHD=MeanPower*exposureTime;// altrimenti si ricava l'energia

		double underroot;
	//si applica la formula pe il calcolo della NOHD
        underroot=(4*ForNOHD)/(PI*PowerEMP);//si impiega l'EMP per gli effetti medi
		MeanPower_NOHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
		if (MeanPower_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
			MeanPower_NOHD=0;
}

void LaserSafetyMP::computeSinglePulse_NOHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo impulso singolo		         		      				     |
   ----------------------------------------------------------------------------------------------------------*/
{		
	double ForNOHD;
	FormulaSort=getFormulaSort();
	
	if(FormulaSort=="E")
           ForNOHD=PowerErg/PulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
           ForNOHD=PowerErg;// altrimenti si considera l'energia dell'impulso

		double underroot;
	//si applica la formula pe il calcolo della NOHD		
		underroot=(4*ForNOHD)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
		SinglePulse_NOHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
		if (SinglePulse_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
			SinglePulse_NOHD=0;
}

void LaserSafetyMP::computePulseTrain_NOHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo dell'impulso singolo considerando gli effetti termici       |
   ----------------------------------------------------------------------------------------------------------*/
{
    if((Wavelength>=400)and(Wavelength<=1.0e+06))
        {
        double ForNOHD;
        FormulaSort=getFormulaSort();

        if(FormulaSort=="E")
             ForNOHD=PowerErg/PulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
             ForNOHD=PowerErg;// altrimenti si considera l'energia dell'impulso

            double underroot;
        //si applica la formula pe il calcolo della NOHD
            underroot=(4*ForNOHD)/(PI*CP_EMP_Result);//si impiega l'EMP per gli effetti termici
            PulseTrain_NOHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
            if (PulseTrain_NOHD<0)// se il risultato è minore di zero allora la NOHD viene impostato a zero.
                PulseTrain_NOHD=0;
    }
    else{
        PulseTrain_NOHD=0;
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

if((Wavelength>=400)and(Wavelength<=1.0e+06))
    {
        M1=fmax(MeanPower_NOHD, SinglePulse_NOHD);
        M2=fmax(SinglePulse_NOHD, PulseTrain_NOHD);

        qDebug()<<"MeanPower_NOHD: "<<MeanPower_NOHD;
        qDebug()<<"SinglePulse_NOHD: "<<SinglePulse_NOHD;
        qDebug()<<"PulseTrain_NOHD: "<<PulseTrain_NOHD;

        NOHD=fmax(M1, M2);

        qDebug()<<"NOHD: "<<NOHD;
    }
    else
    {
        NOHD=fmax(MeanPower_NOHD, SinglePulse_NOHD);;
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
    double ForLambertianMax;
    PowerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media

    if(PowerFormulaSort=="E")
        ForLambertianMax=MeanPower;// se l'EMP è espresso in irradianza si considera la potenza media
            else
        ForLambertianMax=MeanPower*exposureTime;// altrimenti si ricava l'irradianza

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(ForLambertianMax)/(PI*MeanPow_EMP_Result);//si impiega l'EMP per gli effetti medi
        MeanPowerLambertianMax=(sqrt(underroot));
}

void LaserSafetyMP::computeSinglePulseLambertianMax()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo della distanza di sicurezza per riflessione su di un riflettore lamberiano. Il calcolo fa       |
    |riferimento al caso di riflessione puntiforme con direzione normale al diffusore e coeficiente di       |
    |diffusione unitario. Si considerano gli effetti che deriverebbero dal solo impulso singolo.             |         		      				     |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForLambertianMax;
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
           ForLambertianMax=PowerErg/PulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
             else
           ForLambertianMax=PowerErg;// altrimenti si considera l'energia dell'impulso

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(ForLambertianMax)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
        SinglePulseLambertianMax=(sqrt(underroot));
}

void LaserSafetyMP::computePulseTrainLambertianMax()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NOHD che deriverebbe dal solo dell'impulso singolo considerando gli effetti termici       |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForLambertianMax;
    FormulaSort=getFormulaSort();

    if(FormulaSort=="E")
         ForLambertianMax=PowerErg/PulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
         else
         ForLambertianMax=PowerErg;// altrimenti si considera l'energia dell'impulso

        double underroot;
    //si applica la formula pe il calcolo della NOHD
        underroot=(ForLambertianMax)/(PI*CP_EMP_Result);//si impiega l'EMP per gli effetti termici
        PulseTrainLambertianMax=(sqrt(underroot));
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

    M1=fmax(MeanPowerLambertianMax, SinglePulseLambertianMax);
    M2=fmax(SinglePulseLambertianMax, PulseTrainLambertianMax);
    lambertianMax=fmax(M1, M2);
}

void LaserSafetyMP::laserUpdate()
{
	computeTmin();
	setExposureTime();
	computeMeanPower();
	computeMeanPowerIrradiance();
	computePulseNumber();
	computeCP();
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
	FormulaSort=MyMeanPower_Laser.getFormulaSort();
	return FormulaSort;
}

string LaserSafetyMP::getMeanPowerFormulaEMP()
{
	Formula=MyMeanPower_Laser.getFormulaEMP();
	return Formula;
}

double LaserSafetyMP::getCA_MP() const
{
	return MyMeanPower_Laser.getCA();
}

double LaserSafetyMP::getCB_MP() const
{
	return MyMeanPower_Laser.getCB();
}

double LaserSafetyMP::getCC_MP() const
{
	return MyMeanPower_Laser.getCC();
}

double LaserSafetyMP::getCE_MP() const
{
	return MyMeanPower_Laser.getCE();
}

double LaserSafetyMP::getT1_MP() const
{
	return MyMeanPower_Laser.getT1();
}

double LaserSafetyMP::getT2_MP() const
{
	return MyMeanPower_Laser.getT2();
}

double LaserSafetyMP::getGamma_MP() const
{
	return MyMeanPower_Laser.getGamma();
}	

string LaserSafetyMP::getRadiation_MP() const
{
    return MyMeanPower_Laser.getRadiation();
}

string LaserSafetyMP::getSkinDamage_MP() const
{
    return MyMeanPower_Laser.getSkinDamage();
}

string LaserSafetyMP::getEyeDamage_MP() const
{
    return MyMeanPower_Laser.computeEMP::getEyeDamage();
}

string LaserSafetyMP::getPhotochemicalNote_MP() const
{
    return MyMeanPower_Laser.getPhotochemicalNote();
}

double LaserSafetyMP::getEMP_MP()
{
	return MyMeanPower_Laser.getEMP();
}

void LaserSafetyMP::setPRF(const int _PRF)
{
	PRF=_PRF;
}

int LaserSafetyMP::getPRF() const
{
	return PRF;
}

double LaserSafetyMP::getMeanPower() const
{
	return MeanPower;
}

double LaserSafetyMP::getMeanIrradiance() const
{
	return MeanPowerIrradiance;
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
	return PulseNumber;
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
    return MeanPow_EMP_Result;
}

double LaserSafetyMP::getEMP_3rdCondition()const
{
    double SP_EMP_forCP;
    SP_EMP_forCP=MyTmin_Laser.getEMP()*CP/(PRF*Tmin);
    return SP_EMP_forCP;
}

string LaserSafetyMP::getFormula_3rdCondition()
{
    if(PRF>(1/Tmin))
        Formula_Tmin=MyTmin_Laser.getFormulaEMP()+" /(PRF T<sub>min</sub>)";
    else
         Formula_Tmin=MyLaser.getFormulaEMP();

    return Formula_Tmin;
}

string LaserSafetyMP::getFormulaSort_3rdCondition()
{
    if(PRF>(1/Tmin))
        FormulaSort_Tmin=MyTmin_Laser.getFormulaSort();
    else
         FormulaSort_Tmin=MyLaser.getFormulaSort();

    return FormulaSort_Tmin;
}
