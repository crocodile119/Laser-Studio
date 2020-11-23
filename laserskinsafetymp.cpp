#include <cmath>
#include "laserskinsafetymp.h"


using namespace std;

LaserSkinSafetyMP::LaserSkinSafetyMP(int _PRF, double _exposureTime, double _BeamDiameter, double _PowerErg,  double _Divergence, double _Wavelength,
                        double _PulseWidth, double _Alpha):LaserSkinSafety(_BeamDiameter, _PowerErg, _Divergence, _Wavelength,
						_PulseWidth, _Alpha)
{
	//costruttore
	BeamDiameter=_BeamDiameter; 
	PowerErg=_PowerErg;
	Divergence=_Divergence;
	Wavelength=_Wavelength;
	Alpha=_Alpha;
    exposureTime= _exposureTime;
    PRF=_PRF;
		
	//Imposto i parametri del laser relativi al singolo impulso
    MySkinLaser.setWavelength(_Wavelength);
    MySkinLaser.setAlpha(_Alpha);
    MySkinLaser.setPulseDuration(getPulseWidth());
    MySkinLaser.adaptForSkinEMP();
    MySkinLaser.EMP();
    MySkinLaser.getEMP();
	//Imposto i parametri del laser relativi al treno di impulsi nel tempo di funzionamento
    MyMeanPower_SkinLaser.setWavelength(Wavelength);
    MyMeanPower_SkinLaser.setAlpha(Alpha);
    MyMeanPower_SkinLaser.setPulseDuration(exposureTime);
    MyMeanPower_SkinLaser.adaptForSkinEMP();
    MyMeanPower_SkinLaser.EMP();
    MyMeanPower_SkinLaser.getEMP();
}
 
 void LaserSkinSafetyMP::computeMeanPower()
{
	MeanPower=PowerErg*PRF;
}

void LaserSkinSafetyMP::computeMeanPowerEMP()
{
    MyMeanPower_SkinLaser.setWavelength(Wavelength);
    MyMeanPower_SkinLaser.setAlpha(Alpha);
    MyMeanPower_SkinLaser.setPulseDuration(exposureTime);// il termpo di esposizione di considera la durata del treno
    MyMeanPower_SkinLaser.EMP();
    MyMeanPower_SkinLaser.adaptForSkinEMP();
    MeanPow_EMP_Result=MyMeanPower_SkinLaser.getEMP();// restituisce l'EMP in funzione del paramatri forniti.
    PowerFormulaEMP=MyMeanPower_SkinLaser.getFormulaEMP();
    PowerFormulaSort=MyMeanPower_SkinLaser.getFormulaSort();
    PowerSkinEMP=MyMeanPower_SkinLaser.getEMP();

        if(PRF!=0)
        {
            if(PowerFormulaSort=="H")
                MeanPow_EMP_Result=PowerSkinEMP/(exposureTime*PRF);// calcola l'H medio come il rapporo dell'H in Te con N.
            else if(PowerFormulaSort=="E")
                MeanPow_EMP_Result=PowerSkinEMP/(PRF*PulseWidth);//calcola l'E medio come il rapporo dell'H medio con t.
        }
        else
        {
        MeanPow_EMP_Result=PowerSkinEMP;//utile per evitare divisione per 0 quando la PRF si riduce a 0.
        }
}

void LaserSkinSafetyMP::computeMeanPowerIrradiance()
{
	computeBeamArea();
	MeanPowerIrradiance=MeanPower*1000000/getBeamArea();
}

void LaserSkinSafetyMP::computePulseNumber()
{
    PulseNumber=PRF*exposureTime;
}

void LaserSkinSafetyMP::computeSP_EMP()
{
    MySkinLaser.setWavelength(Wavelength);
    MySkinLaser.setAlpha(Alpha);
    MySkinLaser.setPulseDuration(getPulseWidth());
    MySkinLaser.EMP();// calcolo l'EMP per il singolo impulso
    MySkinLaser.adaptForSkinEMP();
    Formula=MySkinLaser.getFormulaEMP();
    FormulaSort=MySkinLaser.getFormulaSort();
    SP_EMP_Result=MySkinLaser.getEMP();// calcolo l'EMP del singolo impulso
}

void LaserSkinSafetyMP::equateMeanPowerEMP()
{    
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

double LaserSkinSafetyMP::getMeanPower_EMP_Equate() const
{
return MeanPow_EMP_Equate;
}

double LaserSkinSafetyMP::returnMultiPulse_EMP()
{
    /*---------------------------------------------------------------------------------------------------------------------------
    | La funzione calcola il valore minimo tra i due valori di EMP calcolati:														 |
    | SP_EMP_Result:		EMP dell'impulso singolo																				 |																								 |
	| MeanPow_EMP_Result:	EMP medio del treno di impulsi omogeneizzato all'unità di misura dell'EMP del singolo impulso.			 |																						     |
	---------------------------------------------------------------------------------------------------------------------------------*/	
    EMP_Result=fmin(SP_EMP_Result, MeanPow_EMP_Equate);
	return EMP_Result;
}


void LaserSkinSafetyMP::computeMeanPower_NSHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NSHD che deriverebbe dal solo effetto medio del treno di impulsi      				     |
    |Tale valore è pari all'irradianza emessa dal laser per la frequenza di ripetisioone degli impulsi       |
   ----------------------------------------------------------------------------------------------------------*/
{
    double ForNSHD;
	PowerFormulaSort= getMeanPowerFormulaSort();//Ricavo l'unità di misura dell'EMP relativo alla potenza media
	
	if(PowerFormulaSort=="E")
        ForNSHD=MeanPower;// se l'EMP è espresso in irradianza si considera la potenza media
            else
        ForNSHD=MeanPower*exposureTime;// altrimenti si ricava l'energia

		double underroot;
    //si applica la formula pe il calcolo della NSHD

        underroot=(4*ForNSHD)/(PI*PowerSkinEMP);//si impiega l'EMP per gli effetti medi
        MeanPower_NSHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
        if (MeanPower_NSHD<0)// se il risultato è minore di zero allora la NSHD viene impostato a zero.
            MeanPower_NSHD=0;
}

void LaserSkinSafetyMP::computeSinglePulse_NSHD()
   /*--------------------------------------------------------------------------------------------------------
    |Calcolo dell'NSHD che deriverebbe dal solo dell'impulso singolo				      				     |
   ----------------------------------------------------------------------------------------------------------*/
{		
    double ForNSHD=0;
	FormulaSort=getFormulaSort();
	
    if(FormulaSort=="E"){
           ForNSHD=PowerErg/PulseWidth;// se l'EMP è espresso in irradianza si considera la potenza (PowerErg è l'energia dell'impulso).
           }
             else if(FormulaSort=="H")
                {
                ForNSHD=PowerErg;// altrimenti si considera l'energia dell'impulso
                }

		double underroot;
    //si applica la formula pe il calcolo della NSHD
        underroot=(4*ForNSHD)/(PI*SP_EMP_Result);//si impiega l'EMP del singolo impulso
        SinglePulse_NSHD=(sqrt(underroot)-(BeamDiameter/1000))/(Divergence/1000);
        if (SinglePulse_NSHD<0)// se il risultato è minore di zero allora la NSHD viene impostato a zero.
            SinglePulse_NSHD=0;
}

void LaserSkinSafetyMP::computeNSHD()
{
	/*--------------------------------------------------------------------------------------------------------
    |La NSHD si calcola calcolando il massimo tra:															 |
    |SinglePulse_NSHD																						 |
    |MeanPower_NSHD																							 |
    ---------------------------------------------------------------------------------------------------------*/
    NSHD=fmax(MeanPower_NSHD, SinglePulse_NSHD);
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
    FormulaSort=MyMeanPower_SkinLaser.getFormulaSort();
	return FormulaSort;
}

string LaserSkinSafetyMP::getMeanPowerFormulaEMP()
{
    Formula=MyMeanPower_SkinLaser.getFormulaEMP();
	return Formula;
}

double LaserSkinSafetyMP::getEMP_MP()
{
    return MyMeanPower_SkinLaser.getEMP();
}

void LaserSkinSafetyMP::setPRF(const int _PRF)
{
	PRF=_PRF;
}

int LaserSkinSafetyMP::getPRF() const
{
	return PRF;
}

double LaserSkinSafetyMP::getMeanPower() const
{
	return MeanPower;
}

double LaserSkinSafetyMP::getMeanIrradiance() const
{
	return MeanPowerIrradiance;
}

double LaserSkinSafetyMP::getPulseNumber() const
{
	return PulseNumber;
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
    return MeanPow_EMP_Result;
}
