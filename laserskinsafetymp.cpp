#include <cmath>
#include "laserskinsafetymp.h"

using namespace std;

const double LaserSkinSafetyMP::PULSE_REPETITION_FREQEUNCY=10.0;
const double LaserSkinSafetyMP::PULSE_WIDTH=1.0e-06;

LaserSkinSafetyMP::LaserSkinSafetyMP(double _PRF, double _exposureTime, double _beamDiameter, double _powerErg,  double _divergence, double _wavelength,
                        double _pulseWidth, double _alpha):LaserSkinSafety(_beamDiameter, _powerErg, _divergence, _wavelength,
                        _pulseWidth, _alpha)
{
	//costruttore
    setBeamDiameter(_beamDiameter);
    setPowerErg(_powerErg);
    setDivergence(_divergence);
    setWavelength(_wavelength);
    setAlpha(_alpha);
    exposureTime= _exposureTime;
    PRF=_PRF;
		
	//Imposto i parametri del laser relativi al singolo impulso
    mySkinLaser.adaptForSkinEMP();
    mySkinLaser.EMP();
    mySkinLaser.getEMP();
	//Imposto i parametri del laser relativi al treno di impulsi nel tempo di funzionamento
    myMeanPower_SkinLaser.adaptForSkinEMP();
    myMeanPower_SkinLaser.EMP();
    myMeanPower_SkinLaser.getEMP();
}
 
void LaserSkinSafetyMP::setPulseWidth(const double& _pulseWidth)
{
    myMeanPower_SkinLaser.setPulseWidth(exposureTime);

    if(_pulseWidth==pulseWidth)
        return;

    mySkinLaser.setPulseWidth(_pulseWidth);
    pulseWidth=_pulseWidth;
}

void LaserSkinSafetyMP::setWavelength(const double& _wavelength)
{
    myMeanPower_SkinLaser.setWavelength(_wavelength);

    if(_wavelength==wavelength)
        return;

    mySkinLaser.setWavelength(_wavelength);
    wavelength=_wavelength;
}

void LaserSkinSafetyMP::setAlpha(const double& _alpha)
{
    myMeanPower_SkinLaser.setAlpha(_alpha);

    if(_alpha==alpha)
        return;

    mySkinLaser.setAlpha(_alpha);
    alpha=_alpha;
}

 void LaserSkinSafetyMP::computeMeanPower()
{
    meanPower=powerErg*PRF;
}

void LaserSkinSafetyMP::computeMeanPowerEMP()
{
    myMeanPower_SkinLaser.setWavelength(wavelength);
    myMeanPower_SkinLaser.setAlpha(alpha);
    myMeanPower_SkinLaser.setPulseWidth(exposureTime);// il termpo di esposizione di considera la durata del treno
    myMeanPower_SkinLaser.EMP();
    myMeanPower_SkinLaser.adaptForSkinEMP();
    meanPow_EMP_Result=myMeanPower_SkinLaser.getEMP();// restituisce l'EMP in funzione del paramatri forniti.
    powerFormulaEMP=myMeanPower_SkinLaser.getFormulaEMP();
    powerFormulaSort=myMeanPower_SkinLaser.getFormulaSort();
    powerSkinEMP=myMeanPower_SkinLaser.getEMP();

    if(PRF!=0)
    {
        if(powerFormulaSort=="H")
            meanPow_EMP_Result=powerSkinEMP/ceil(exposureTime*PRF);// calcola l'H medio come il rapporo dell'H in Te con N.
        else if(powerFormulaSort=="E")
            meanPow_EMP_Result=powerSkinEMP;//Nel caso in cui si considera l'irradianza l'irradianza media è proprio pari all'irradianza relativa la tempo di esposizione.
    }
        else
    meanPow_EMP_Result=powerSkinEMP;//utile per evitare divisione per 0 quando la PRF si riduce a 0.
}

void LaserSkinSafetyMP::computeMeanPowerIrradiance()
{
	computeBeamArea();
    meanPowerIrradiance=meanPower/getBeamArea();
}

void LaserSkinSafetyMP::computePulseNumber()
{
    pulseNumber=ceil(PRF*exposureTime);
}

void LaserSkinSafetyMP::computeSP_EMP()
{
    mySkinLaser.EMP();// calcolo l'EMP per il singolo impulso
    mySkinLaser.adaptForSkinEMP();
    formula=mySkinLaser.getFormulaEMP();
    formulaSort=mySkinLaser.getFormulaSort();
    SP_EMP_Result=mySkinLaser.getEMP();// calcolo l'EMP del singolo impulso
}

void LaserSkinSafetyMP::equateMeanPowerEMP()
{    
    if(formulaSort=="H")
    {
    //se l'EMP calcolato per il tempo di funzionamento è espresso in esposizione radiante esprimo l'emissione radiante in esposizione radiante
        if(powerFormulaSort=="H")
            meanPow_EMP_Equate=meanPow_EMP_Result;  
        else if(powerFormulaSort=="E")//altrimenti trasformo l'EMP in esposizione radiante un irradianza dividendo per il tempo di esposizione.
            meanPow_EMP_Equate=meanPow_EMP_Result*PRF;//ottengo il valore della radianza media da E
    }
    // viceversa se l'EMP del tempo di funzionamento è espresso in irradianza
    else if(formulaSort=="E")
    {
        if(powerFormulaSort=="H")
            meanPow_EMP_Equate=meanPow_EMP_Result/PRF;//calcolo E da H dividendo per t.
        else if(powerFormulaSort=="E")
            meanPow_EMP_Equate=meanPow_EMP_Result;
    }
}

double LaserSkinSafetyMP::getMeanPower_EMP_Equate() const
{
return meanPow_EMP_Equate;
}

double LaserSkinSafetyMP::returnMultiPulse_EMP()
{
    /*---------------------------------------------------------------------------------------------------------------------------
    | La funzione calcola il valore minimo tra i due valori di EMP calcolati:														 |
    | SP_EMP_Result:		EMP dell'impulso singolo																				 |																								 |
	| MeanPow_EMP_Result:	EMP medio del treno di impulsi omogeneizzato all'unità di misura dell'EMP del singolo impulso.			 |																						     |
	---------------------------------------------------------------------------------------------------------------------------------*/	
    EMP_Result=fmin(SP_EMP_Result, meanPow_EMP_Equate);
	return EMP_Result;
}


void LaserSkinSafetyMP::computeMeanPower_NSHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NSHD che deriverebbe dal solo effetto medio del treno di impulsi      				     |
    |Tale valore è pari all'irradianza emessa dal laser per la frequenza di ripetisioone degli impulsi       |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForNSHD;
    powerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media
	
    if(powerFormulaSort=="E")
        ForNSHD=meanPower;// se l'EMP è espresso in irradianza si considera la potenza media
            else
        ForNSHD=meanPower*exposureTime;// altrimenti si ricava l'energia

    double underroot;
    //si applica la formula pe il calcolo della NSHD

    underroot=(4*ForNSHD)/(PI*powerSkinEMP);//si impiega l'EMP per gli effetti medi
    meanPower_NSHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);
    if(meanPower_NSHD<0)// se il risultato è minore di zero allora la NSHD viene impostato a zero.
        meanPower_NSHD=0;
}

void LaserSkinSafetyMP::computeSinglePulse_NSHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NSHD che deriverebbe dal solo dell'impulso singolo				      				     |
   ----------------------------------------------------------------------------------------------------------*/
{		
    double ForNSHD=0;
    formulaSort=getFormulaSort();
	
    if(formulaSort=="E")
        ForNSHD=powerErg/pulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
    else if(formulaSort=="H")
        ForNSHD=powerErg;// altrimenti si considera l'energia dell'impulso

    double underroot;
    //si applica la formula pe il calcolo della NSHD
    underroot=(4*ForNSHD)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
    singlePulse_NSHD=(sqrt(underroot)-(beamDiameter/1000))/(divergence/1000);

    if (singlePulse_NSHD<0)// se il risultato è minore di zero allora la NSHD viene impostato a zero.
        singlePulse_NSHD=0;
}

void LaserSkinSafetyMP::computeNSHD()
{
	/*--------------------------------------------------------------------------------------------------------
    |La NSHD si calcola calcolando il massimo tra:															 |
    |SinglePulse_NSHD																						 |
    |MeanPower_NSHD																							 |
    ---------------------------------------------------------------------------------------------------------*/
    NSHD=fmax(meanPower_NSHD, singlePulse_NSHD);
}

	/*--------------------------------------------------------------------------------------------------------
	|Ricalcolo																								 |
    ---------------------------------------------------------------------------------------------------------*/
void LaserSkinSafetyMP::laserSkinUpdate()
{
	computeMeanPower();
    computePulseNumber();
	computeMeanPowerIrradiance();
	computeMeanPowerEMP();
    computeSP_EMP();
    equateMeanPowerEMP();

    computeMeanPower_NSHD();
    computeSinglePulse_NSHD();
	
    computeNSHD();
}

	/*--------------------------------------------------------------------------------------------------------
    |Getters e Setters								 														 |
    ---------------------------------------------------------------------------------------------------------*/
string LaserSkinSafetyMP::getMeanPowerFormulaSort()
{
    formulaSort=myMeanPower_SkinLaser.getFormulaSort();
    return formulaSort;
}

string LaserSkinSafetyMP::getMeanPowerFormulaEMP()
{
    formula=myMeanPower_SkinLaser.getFormulaEMP();
    return formula;
}

double LaserSkinSafetyMP::getEMP_MP()
{
    return myMeanPower_SkinLaser.getEMP();
}

void LaserSkinSafetyMP::setPRF(const int _PRF)
{
	PRF=_PRF;
}

double LaserSkinSafetyMP::getPRF() const
{
	return PRF;
}

double LaserSkinSafetyMP::getMeanPower() const
{
    return meanPower;
}

double LaserSkinSafetyMP::getMeanIrradiance() const
{
    return meanPowerIrradiance;
}

double LaserSkinSafetyMP::getPulseNumber() const
{
    return pulseNumber;
}

double LaserSkinSafetyMP::getExposureTime() const
{
    return exposureTime;
}

void LaserSkinSafetyMP::setExposureTime(const double &_ExposureTime)
{
    exposureTime=_ExposureTime;
}

double LaserSkinSafetyMP::getNSHD() const
{
    return NSHD;
}

double LaserSkinSafetyMP::getEMP_1stCondition()const
{
    return SP_EMP_Result;
}

double LaserSkinSafetyMP::getEMP_2ndCondition()const
{
    return meanPow_EMP_Result;
}
