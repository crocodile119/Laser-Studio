#include "lasergoggle.h"
#include <cmath>
#include <assert.h>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

const int LaserGoggle::TIMEBASE = 5;
const int LaserGoggle::TIMEBASE_LOW_WAVELENGTH =30000;
const double LaserGoggle::PI=3.141592653589793238462643383;
const int LaserGoggle::CONTINUOS_OPERATION=0;
const double LaserGoggle::GLASS_EXPONENT=1.1693;
const double LaserGoggle::PLASTIC_EXPONENT=1.2233;
const int LaserGoggle::HE_NE_WAVELENGTH=633;
const double LaserGoggle::PULSE_WIDTH=1.0e-06;
const double LaserGoggle::POWER_ENERGY=1.0;
const double LaserGoggle::PUPIL_DIAMETER=7.0;

LaserGoggle::LaserGoggle(int _wavelength, double _pulseWidth, double _powerErg, double _beamDiameter):
        wavelength(_wavelength), powerErg(_powerErg), beamDiameter(_beamDiameter), frequency(0),
        myMeanPower(0), materialCorrection(1.0)
{
    /**************************************************************************************************
    * Costruttore per istanza della classe relativa al funzionamento ad impulsi multipli riguardante  *
    * gli effetti medi. Questa istanza riguarda solo il funzionamento ad impulsi multipli.            *                                                                              *
    ***************************************************************************************************/

    buildScaleNumbers();

    /***************************************************************************************************
     * L'impulso non può avere durata nulla, gli assegno nel caso di durata 0 il valore convenzionale  *
     * di timeBaseLowWavelength se la lunghezza d'onda è compresa tra 180 e 315 nm, gli assegno il     *
     * valore di timeBaseWavelength per lunghezze d'onda maggiori di 315 nm.                           *
     * Nel caso di laser in continua, oppure nel caso di laser ad impulsi multipli per la verifica     *
     * della potenza media è possibile:                                                                *
     *  - inserire 0, in questo modo la classe seleziona direttamente la base dei tempi esatta;        *
     *  - inserire direttamente la base dei tempi pertinente.                                          *
     ***************************************************************************************************/


    if(_pulseWidth==CONTINUOS_OPERATION)
    {
        if((wavelength>=180)&&(wavelength<=315))
            pulseWidth = TIMEBASE_LOW_WAVELENGTH;
        else if((wavelength>315)&&(wavelength<=1.0e+06))
            pulseWidth = TIMEBASE;

        myLaserOperation=CONTINUOS_WAVE;
    }
    else
    {
    pulseWidth = _pulseWidth;
    myLaserOperation=IMPULSATO;
    }
    k=1.0;
    ki=1.0;
}

//Overload costruttore
LaserGoggle::LaserGoggle(int _wavelength, double _pulseWidth, double _powerErg, double _beamDiameter, double _frequency):
    wavelength(_wavelength), powerErg(_powerErg), beamDiameter(_beamDiameter), frequency(_frequency),
    materialCorrection(1.0)
{
    buildScaleNumbers();

    if(_pulseWidth==CONTINUOS_OPERATION)
    {
        if((wavelength>=180)&&(wavelength<=315))
            pulseWidth = TIMEBASE_LOW_WAVELENGTH;
        else if((wavelength>315)&&(wavelength<=1.0e+06))
            pulseWidth = TIMEBASE;

        myLaserOperation=CONTINUOS_WAVE;
    }
        else
    {
        pulseWidth = _pulseWidth;

        if(round(frequency)==0)
        myLaserOperation=IMPULSATO;
        else
        myLaserOperation=IMPULSI_MULTIPLI;
    }

    frequencyCorrection();
    myMeanPower=_powerErg*frequency;
    k=1.0;
    ki=1.0;
}


void LaserGoggle::setLaserOperation()
{
    if(pulseWidth==CONTINUOS_OPERATION )
    myLaserOperation=CONTINUOS_WAVE;
    else
    {
    if(round(frequency)==0)
        myLaserOperation=IMPULSATO;
    else
        myLaserOperation=IMPULSI_MULTIPLI;
    }
}

LaserGoggle::laserOperation LaserGoggle::getLaserOperation()
{
    return myLaserOperation;
}

array<int, LaserGoggle::TABLEROWS> LaserGoggle::buildScaleNumbers()
{
   for(size_t i=0; i<TABLEROWS; i++ )
   LB_Scale[i] = i+1;

   return LB_Scale;
}

array<double, LaserGoggle::TABLEROWS> LaserGoggle::selectData(const double &wavelength, const double &applicableTime)
{
    if((wavelength>=180.0) && (wavelength<315.0))
        {
        if(applicableTime>=3.0e+04)
        {
            array<double, LaserGoggle::TABLEROWS> D180_315_Values = {0.01, 0.1, 1.0, 10.0, 1.0e+02, 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=D180_315_Values[i];

            pulseCode=CONTINUOS_EMISSION;
        }
        if((applicableTime>=1.0e-09)&&(applicableTime<3.0e+04))
        {
            array<double, LaserGoggle::TABLEROWS> IR180_315_Values = {3.0e+02, 3.0e+03, 3.0e+04, 3.0e+05,  3.0e+06, 3.0e+07, 3.0e+08, 3.0e+09, 3.0e+10,3.0e+11};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=IR180_315_Values[i];

            if (applicableTime<1.0e-06)
                pulseCode=Q_SWITCHED;
            else
                pulseCode=SIMPLE_PULSED;
        }
        if(applicableTime<1.0e-09)
        {
            array<double, LaserGoggle::TABLEROWS> M180_315_Values = {3.0e+11, 3.0e+12, 3.0e+13, 3.0e+14, 3.0e+15, 3.0e+16, 3.0e+17, 3.0e+18, 3.0e+19, 3.0e+20};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=M180_315_Values[i];

            pulseCode=MODE_LOCKED;
        }
    }

    if((wavelength>=315.0) && (wavelength<1400.0))
        {
        if(applicableTime>=5.0e-04)
        {
            array<double, LaserGoggle::TABLEROWS> D315_1400_Values = { 1.0e+02, 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=D315_1400_Values[i];

            pulseCode=CONTINUOS_EMISSION;
        }
        if((applicableTime>=1.0e-09)&&(applicableTime<5.0e-04))
        {
            array<double, LaserGoggle::TABLEROWS> IR315_1400_Values = {0.05, 0.5 , 5, 50, 5.0e+02, 5.0e+03, 5.0e+04, 5.0e+05, 5.0e+06, 5.0e+07};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=IR315_1400_Values[i];

            if (applicableTime<1.0e-06)
                pulseCode=Q_SWITCHED;
            else
                pulseCode=SIMPLE_PULSED;
        }
        if(applicableTime<1.0e-09)
        {
            array<double, LaserGoggle::TABLEROWS> M315_1400_Values = {1.5e-03, 1.5e-02, 0.15, 1.5, 15, 1.5e+02, 1.5e+03, 1.5e+04, 1.5e+05, 1.5e+06};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=M315_1400_Values[i];

            pulseCode=MODE_LOCKED;
        }
    }


    if((wavelength>=1400.0) && (wavelength<1.0e+06))
        {
        if(applicableTime>=1.0e-01)
        {
            array<double, LaserGoggle::TABLEROWS> D1400_1mm_Values = {1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11, 1.0e+12, 1.0e+13};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=D1400_1mm_Values[i];

            pulseCode=CONTINUOS_EMISSION;
        }
        if((applicableTime>=1.0e-09)&&(applicableTime<1.0e-01))
        {
            array<double, LaserGoggle::TABLEROWS> IR1400_1mm_Values = { 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11, 1.0e+12};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=IR1400_1mm_Values[i];

            if (applicableTime<1.0e-06)
                pulseCode=Q_SWITCHED;
            else
                pulseCode=SIMPLE_PULSED;
        }
        if(applicableTime<1.0e-09)
        {
            array<double, LaserGoggle::TABLEROWS> M1400_1mm_Values = {1.0e+12,  1.0e+13, 1.0e+14, 1.0e+15, 1.0e+16, 1.0e+17, 1.0e+18, 1.0e+19, 1.0e+20, 1.0e+21};

            for (size_t i=0; i<TABLEROWS; i++)
                expositionData[i]=M1400_1mm_Values[i];

            pulseCode=MODE_LOCKED;
        }
    }

    return expositionData;
}

void LaserGoggle::setPulseCode()
{
    setLaserOperation();
    switch(pulseCode)
    {
    case (CONTINUOS_EMISSION):
    pulseCodeString="Laser ad onda continua";
    pulseCodeLetter='D';
    break;

    case (SIMPLE_PULSED):
    if(myLaserOperation==IMPULSATO)
        pulseCodeString="Laser ad impulso ";
    else if(myLaserOperation==IMPULSI_MULTIPLI)
        pulseCodeString="Laser ad impulsi";

    pulseCodeLetter='I';
    break;

    case (Q_SWITCHED):
    if(myLaserOperation==IMPULSATO)
        pulseCodeString="Laser ad impulso Q Switched";
    else if(myLaserOperation==IMPULSI_MULTIPLI)
        pulseCodeString="Laser ad impulsi Q Switched";

    pulseCodeLetter='R';
    break;

    case (MODE_LOCKED):
    pulseCodeString="Laser a modi accoppiati (Mode Locked)";
    pulseCodeLetter='M';
    break;
    }
}

vector< pair <int,double> > LaserGoggle::buildDataVector(const array<int, LaserGoggle::TABLEROWS> LB_Scale,
                                                         const array<double, LaserGoggle::TABLEROWS> expositionData)
{
    vector< pair <int,double> > dataVector;
    // Entering values in vector of pairs
    for (size_t i=0; i<TABLEROWS; i++)
    dataVector.push_back( make_pair(LB_Scale[i],expositionData[i]) );

    return dataVector;
}


double LaserGoggle::laserIrrRadCorrected(double _laserOutput)
{
    myExpositionValue=pulseTrainCorrectionK()*get_Ki()*laserIrrRad(_laserOutput);
    return myExpositionValue;
}

int LaserGoggle::scaleNumber(vector< pair <int,double> > _dataVector, double myExpositionValue)
{
    size_t i=0;

    while((_dataVector[i].second<myExpositionValue)&& (i<=TABLEROWS-1))
    i++;

    if(i==TABLEROWS)
        myScaleNumber=0;
    else
        myScaleNumber= _dataVector[i].first;

    return myScaleNumber;
}

double LaserGoggle::pulseTrainCorrectionK()
{
    double myNymberOfPulse=numberOfPulse();

    if(round(frequency)==CONTINUOS_OPERATION)
       k=1;
    else
    {
        if((wavelength>=400)and(wavelength<=1.0e+06))
            k= pow(myNymberOfPulse, 0.25);
        else
            k=1;
    }

    return k;
}

double LaserGoggle::frequencyCorrection()
{
    /* se la lunghezza d'onda appartiene ad alcun intervallo del prospetto B.2 EN207
    * la correzione non va applicata, ciò equivale a porre il valore di Ti=1/frequency
    * e quello di ni_max=frequency */
    if(round(frequency)!=CONTINUOS_OPERATION)
    {
        if ((wavelength >= 180) && (wavelength<400))
        {
            Ti=1/frequency;
            ni_max=frequency;
        }
    else
    /* Nel caso si applichi il prospetto B.2 EN207 si memorizzano i valori di Ti e ni_max*/
    if ((wavelength >= 400) && (wavelength<1050))
    {
        Ti=18e-06;
        ni_max=55.56e+03;
    }
    else if ((wavelength >= 1050) && (wavelength<1400))
    {
        Ti=50e-06;
        ni_max=20e+03;
    }
    else if ((wavelength >= 1400) && (wavelength<2600))
    {
        Ti=1e-03;
        ni_max=1e+03;
    }
    else if((wavelength >= 2600) && (wavelength<1e+06))
    {
        Ti=1e-07;
        ni_max=1e+07;
    }
    else
    {
        Ti=1/frequency;
        ni_max=frequency;
    }
    /* Se la frequenza è maggiore del valore di ni_max si calcolano il valore di ki ed il valore della
    * potenza media. Successivamente si aggiorna il valore della frequenza (cfr. paragrafo B.3.3.2 EN207). */

    if(frequency>ni_max)
        ki=frequency*Ti;
    else
        ki=1;

    /* La funzione restituisce il valore  ni_max*/
    }
    return ni_max;
}

int LaserGoggle::numberOfPulse()
{
    /*************************************************************************************************
     *  La base dei tempi da considerare è quella dei 5 secondi nel caso di lunghezze                *
     * d'onda maggiori di 315 nm. Il valore di tale costante è memorizzato nella costante            *
     * timeBaseWavelength. Per lunghezze d'onda minori di 315 nm il progetto dei protettori          *
     * corrisponde alla base dei tempi di 30000 s tuttavia la verifica di reisitenza                 *
     * dei protettori è di 5s. Si assume per tale motivo una base dei tempi corrispondente ai        *
     * 5s fatto salvo eventuali esigenze, in quel caso bisognerà apportare correzioni al programma   *
     * Dopo una seconda revisione di ritiene più opportuno considerare per lunghezze d'onda minori   *
     * di 380 nm una base dei tempi di 30000 tale valore risulta valido solo per il calcolo degli    *
     * effetti medi                                                                                  *
     *************************************************************************************************/
    frequencyCorrection();

    if((wavelength>=180)&&(wavelength<=315))
    {
        numberOfPulses = fminf(ni_max, frequency)*TIMEBASE_LOW_WAVELENGTH;
        if(numberOfPulses<1)
            numberOfPulses=1;
    }
    else if((wavelength>315)&&(wavelength<=1.0e+06))
    {
        numberOfPulses = fminf(ni_max, frequency)*TIMEBASE;
        if(numberOfPulses<50)
            numberOfPulses=50;
    }

    return numberOfPulses;
}

string LaserGoggle::goggleMark(int _wavelength, double _pulseWidth, double _powerErg, double _beamDiameter, double _frequency)
{
    setWavelength(_wavelength);
    setPulseWidth(_pulseWidth);
    setPowerErg(_powerErg);
    setBeamDiameter(_beamDiameter);
    setFrequency(_frequency);
    prepareGoggleMark();
    string myGoggleMark;
    myGoggleMark=printGoggleCode();

    return myGoggleMark;
}

string LaserGoggle::goggleMark()
{
    prepareGoggleMark();
    string myGoggleMark;
    myGoggleMark=printGoggleCode();

    return myGoggleMark;
}

string LaserGoggle::getLaserPulseOperation()
{
    return pulseCodeString;
}

void LaserGoggle::prepareGoggleMark()
{
    expositionData=selectData(wavelength, pulseWidth);
    dataVector=buildDataVector(buildScaleNumbers(), expositionData);
    double myLaserDensityOutput;
    myLaserDensityOutput = laserIrrRadCorrected(powerErg);
    myScaleNumber=scaleNumber(dataVector, myLaserDensityOutput );
}

string LaserGoggle::printGoggleCode()
{
    if(myScaleNumber)
    {
        setPulseCode();
        std::string wavelength_str = std::to_string(getWavelength());
        std::string myScaleNumber_str = std::to_string(myScaleNumber);
        myGoggleCode= wavelength_str + " " + pulseCodeLetter + " LB" + myScaleNumber_str;
    }
    else
        myGoggleCode= "Eccede i limiti EN207";

    return myGoggleCode;
}

vector< pair <int,double> > LaserGoggle::getDataVector()
{
    return dataVector;
}

string LaserGoggle::getCodeUnit()
{
    string energy="[J/m<sup>2</sup>]";
    string power="[W/m<sup>2</sup>]";

    if(pulseCode==CONTINUOS_EMISSION)
        codeUnit=power;
    else if(pulseCode==MODE_LOCKED)
    {
        if((wavelength>=315.0)and(wavelength<1400.0))
            codeUnit=energy;
        else
            codeUnit=power;
    }
    else if((pulseCode==Q_SWITCHED) || (pulseCode==SIMPLE_PULSED))
        codeUnit=energy;

    return codeUnit;
}

void LaserGoggle::setMaterial(material typeOfMaterial)
{
    double myBeam;

    if(beamDiameter<15)
    myBeam=beamDiameter;
    else
    myBeam=15;

    switch (typeOfMaterial)
    {
        case GLASS:
        materialCorrection=pow(myBeam, GLASS_EXPONENT);
        break;

        case PLASTIC:
        materialCorrection=pow(myBeam, PLASTIC_EXPONENT);
        break;

        case ONLY_REFLECTOR:
        materialCorrection=1.0;
        break;

        default:
        materialCorrection=1.0;
        break;
    }
}

string LaserGoggle::outputSort()
{
    if(pulseCode==CONTINUOS_EMISSION)
        laserOutputSort="E";
    else if(pulseCode==MODE_LOCKED)
    {
        if((wavelength>=315.0)and(wavelength<1400.0))
            laserOutputSort="H<sub>picco </sub>";
        else
            laserOutputSort="E<sub>picco </sub>";
    }
    else if((pulseCode==Q_SWITCHED) || (pulseCode==SIMPLE_PULSED))
        laserOutputSort="H";

    return laserOutputSort;
}


double LaserGoggle::get_Ki()
{
    /* Restituisce il valore di ki*/
    return ki;
}

double LaserGoggle::meanPower()
{
    myMeanPower=powerErg*frequency;
    return myMeanPower;
}

double LaserGoggle::laserIrrRad(double powerErg)
{
    double beamArea= PI*pow(beamDiameter,2)/(4*1.0e+06);
    irrRad=materialCorrection*powerErg/beamArea;
    return irrRad;
}

int LaserGoggle::getScaleNumber()const
{
    return myScaleNumber;
}

int LaserGoggle::getWavelength()
{
    return wavelength;
}

void LaserGoggle::setWavelength(const double & _wavelength)
{
    wavelength=_wavelength;
}

double LaserGoggle::getPulseWidth()
{
    return pulseWidth;
}

void LaserGoggle::setPulseWidth(const double & _pulseWidth)
{
    if(_pulseWidth==0)
    {
        if((wavelength>=180)&&(wavelength<=315))
            pulseWidth = TIMEBASE_LOW_WAVELENGTH;
        else if((wavelength>315)&&(wavelength<=1.0e+06))
            pulseWidth = TIMEBASE;
    }
    else
        pulseWidth = _pulseWidth;
}

double LaserGoggle::getPowerErg()
{
    return powerErg;
}

void LaserGoggle::setPowerErg(const double & _powerErg)
{
    powerErg=_powerErg;
}

double LaserGoggle::getBeamDiameter()
{
    return beamDiameter;
}

void LaserGoggle::setBeamDiameter(const double & _beamDiameter)
{
    beamDiameter=_beamDiameter;
}

double LaserGoggle::getFrequency()
{
    return frequency;
}

void LaserGoggle::setFrequency(const double & _frequency)
{
    frequency=_frequency;
}

int LaserGoggle::getNumberOfPulse()const
{
    return numberOfPulses;
}

double LaserGoggle::getCoefficient_ki()const
{
    return ki;
}

double LaserGoggle::getCoefficient_k()const
{
    return k;
}

double LaserGoggle::get_ni_max()const
{
    return ni_max;
}

void LaserGoggle::printData()
{
    for(size_t i=0; i<TABLEROWS; i++)
    {
        cout<< "Valori EN207 selezionati: "<< expositionData[i] <<endl;
    }
}

void LaserGoggle::printScaleNumbers(int myLB[])
{
    cout << "I numeri di scala sono i seguenti: \n";
    for(size_t i=0; i<TABLEROWS; ++i)
        cout<< myLB[i] <<", ";
}


void LaserGoggle::printVector( vector< pair <int,double> > dataVector)
{
    vector< pair <int,double> >::const_iterator constIterator; // const_iterator
    // display vector elements using const_iterator
    for ( constIterator = dataVector.begin();
    constIterator != dataVector.end(); ++constIterator )
    cout << "\nNumero di scala: " << constIterator->first << " valore corrispondente: " <<  constIterator->second;
}

LaserGoggle::~LaserGoggle()
{
}
