#include "lasergoggle.h"
#include <cmath>
#include <assert.h>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

const int LaserGoggle::timeBaseLowWavelength =30000;
const int LaserGoggle::timeBaseWavelength = 5;
const double LaserGoggle::PI=3.141592653589793238462643383;


LaserGoggle::LaserGoggle(int _wavelength, double _pulseWidth, double _powerErg, double _beamDiameter)
{
    /**************************************************************************************************
    * Costruttore per istanza della classe relativa al funzionamento ad impulsi ripetuti riguardante  *
    * gli effetti medi. Questa istanza riguarda solo il funzionamento ad impulsi ripetuti.            *                                                                              *
    ***************************************************************************************************/

    buildScaleNumbers();
    n=10;
    expositionData = new double[n];
    wavelength = _wavelength;

    /***************************************************************************************************
     * L'impulso non può avere durata nulla, gli assegno nel caso di durata 0 il valore convenzionale  *
     * di timeBaseLowWavelength se la lunghezza d'onda è compresa tra 180 e 315 nm, gli assegno il     *
     * valore di timeBaseWavelength per lunghezze d'onda maggiori di 315 nm.                           *
     * Nel caso di laser in continua, oppure nel caso di laser ad impulsi ripetuti per la verifica     *
     * della potenza media è possibile:                                                                *
     *  - inserire 0, in questo modo la classe seleziona direttamente la base dei tempi esatta;        *
     *  - inserire direttamente la base dei tempi pertinente.                                          *
     ***************************************************************************************************/

        if(_pulseWidth==0)
        {
            if((wavelength>=180)&&(wavelength<=315))
                pulseWidth = timeBaseLowWavelength;
                    else
                    if((wavelength>315)&&(wavelength<=1.0e+06))
                        pulseWidth = timeBaseWavelength;
        }
            else
                pulseWidth = _pulseWidth;

    powerErg = _powerErg;
    myMeanPower=0;
    beamDiameter = _beamDiameter;
    frequency=0;
    k=1.0;
    ki=1.0;
}

//Overload costruttore
LaserGoggle::LaserGoggle(int _wavelength, double _pulseWidth, double _powerErg, double _beamDiameter, int _frequency)
{
    buildScaleNumbers();
    n=10;
    expositionData = new double[n];
    wavelength = _wavelength;

    if(_pulseWidth==0)
    {
        if((wavelength>=180)&&(wavelength<=315))
            pulseWidth = timeBaseLowWavelength;
                else
                if((wavelength>315)&&(wavelength<=1.0e+06))
                    pulseWidth = timeBaseWavelength;
    }
        else
            pulseWidth = _pulseWidth;

    powerErg = _powerErg;
    beamDiameter = _beamDiameter;
    frequency = _frequency;
    frequencyCorrection();
    myMeanPower=_powerErg*frequency;
    k=1.0;
    ki=1.0;
}


int* LaserGoggle::buildScaleNumbers()
{
   LB_Scale = new int[10];
   LB_Scale[0] = 1;
   LB_Scale[1] = 2;
   LB_Scale[2] = 3;
   LB_Scale[3] = 4;
   LB_Scale[4] = 5;
   LB_Scale[5] = 6;
   LB_Scale[6] = 7;
   LB_Scale[7] = 8;
   LB_Scale[8] = 9;
   LB_Scale[9] = 10;
   return LB_Scale;
}

double* LaserGoggle::selectData(const double &wavelength, const double &pulseWidth)
{
    if((wavelength>=180.0) && (wavelength<315.0))
        {
        if(pulseWidth>=3.0e+04)
        {
            double D180_315_Values[] = {0.01, 0.1, 1.0, 10.0, 1.0e+02, 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07};
            for (int i=0; i<n; i++)
            expositionData[i]=D180_315_Values[i];
            pulseCode="D";
        }
        if((pulseWidth>=1.0e-09)&&(pulseWidth<3.0e+04))
        {
            double IR180_315_Values[] = {3.0e+02, 3.0e+03, 3.0e+04, 3.0e+05,  3.0e+06, 3.0e+07, 3.0e+08, 3.0e+09, 3.0e+10,3.0e+11};
            for (int i=0; i<n; i++)
            expositionData[i]=IR180_315_Values[i];
            if (pulseWidth<1.0e-06)
                pulseCode="R";
            else
                pulseCode="I";
        }
        if(pulseWidth<1.0e-09)
        {
            double M180_315_Values[] = {3.0e+11, 3.0e+12, 3.0e+13, 3.0e+14, 3.0e+15, 3.0e+16, 3.0e+17, 3.0e+18, 3.0e+19, 3.0e+20};
            for (int i=0; i<n; i++)
            expositionData[i]=M180_315_Values[i];
            pulseCode="M";
        }
    }

    if((wavelength>=315.0) && (wavelength<1400.0))
        {
        if(pulseWidth>=5.0e-04)
        {
            double D315_1400_Values[] = { 1.0e+02, 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11};
            for (int i=0; i<n; i++)
            expositionData[i]=D315_1400_Values[i];
            pulseCode="D";
        }
        if((pulseWidth>=1.0e-09)&&(pulseWidth<5.0e-04))
        {
            double IR315_1400_Values[] = {0.05, 0.5 , 5, 50, 5.0e+02, 5.0e+03, 5.0e+04, 5.0e+05, 5.0e+06, 5.0e+07};
            for (int i=0; i<n; i++)
            expositionData[i]=IR315_1400_Values[i];
            if (pulseWidth<1.0e-06)
                pulseCode="R";
            else
                pulseCode="I";
        }
        if(pulseWidth<1.0e-09)
        {
            double M315_1400_Values[] = {1.5e-03, 1.5e-02, 0.15, 1.5, 15, 1.5e+02, 1.5e+03, 1.5e+04, 1.5e+05, 1.5e+06};
            for (int i=0; i<n; i++)
            expositionData[i]=M315_1400_Values[i];
            pulseCode="M";
        }
    }


    if((wavelength>=1400.0) && (wavelength<1.0e+06))
        {
        if(pulseWidth>=1.0e-01)
        {
            double D1400_1mm_Values[] = {1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11, 1.0e+12, 1.0e+13};
            for (int i=0; i<n; i++)
            expositionData[i]=D1400_1mm_Values[i];
            pulseCode="D";
        }
        if((pulseWidth>=1.0e-09)&&(pulseWidth<1.0e-01))
        {
            double IR1400_1mm_Values[] = { 1.0e+03, 1.0e+04, 1.0e+05, 1.0e+06, 1.0e+07, 1.0e+08, 1.0e+09, 1.0e+10, 1.0e+11, 1.0e+12};
            for (int i=0; i<n; i++)
            expositionData[i]=IR1400_1mm_Values[i];
            if (pulseWidth<1.0e-06)
                pulseCode="R";
            else
                pulseCode="I";
        }
        if(pulseWidth<1.0e-09)
        {
            double M1400_1mm_Values[] = {1.0e+12,  1.0e+13, 1.0e+14, 1.0e+15, 1.0e+16, 1.0e+17, 1.0e+18, 1.0e+19, 1.0e+20, 1.0e+21};
            for (int i=0; i<n; i++)
            expositionData[i]=M1400_1mm_Values[i];
            pulseCode="M";
        }
    }

    return expositionData;
}

vector< pair <int,double> > LaserGoggle::buildDataVector(const double expositionData[], const int LB_Scale[])
    {
      vector< pair <int,double> > dataVector;
      // Entering values in vector of pairs

          for (int i=0; i<n; i++)
             dataVector.push_back( make_pair(LB_Scale[i],expositionData[i]) );

            return dataVector;
    }


 double LaserGoggle::laserIrrRadCorrected(double _laserOutput)
    {
       myExpositionValue=pulseTrainCorrectionK()*pulseTrainCorrectionKi()*laserIrrRad(_laserOutput);
       return myExpositionValue;
    }

  int LaserGoggle::scaleNumber(vector< pair <int,double> > _dataVector,double myExpositionValue)
  {
    int i=0;

        while((_dataVector[i].second<myExpositionValue)&& (i<n-1))
            i++;

                if(i==n)
                        myScaleNumber=0;
                else
                    myScaleNumber= _dataVector[i].first;

        return myScaleNumber;
    }

  double LaserGoggle::pulseTrainCorrectionK()
  {
      double myNymberOfPulse=numberOfPulse();

      if(frequency>1){
          if((wavelength>=400)and(wavelength<=1.0e+06))
              {
               k= pow(myNymberOfPulse, 0.25);
               //cout<< "Coefficiente k: "<< k <<endl;
              }
          else
              {
                  k=1;
              }
      }
      else
        {
          if(frequency==0)
              k=1;
        }
  return k;
  }

  double LaserGoggle::frequencyCorrection()
  {
      /* se la lunghezza d'onda appartiene ad alcun intervallo del prospetto B.2 EN207
       * la correzione non va applicata, ciò equivale a porre il valore di Ti=1/frequency
       * e quello di ni_max=frequency */
      if(frequency>1)
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
      else
      if ((wavelength >= 1050) && (wavelength<1400))
          {
          Ti=50e-06;
          ni_max=20e+03;
          }
      else
      if ((wavelength >= 1400) && (wavelength<2600))
          {
          Ti=1e-03;
          ni_max=1e+03;
          }
      else
      if ((wavelength >= 2600) && (wavelength<1e+06))
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
        numberOfPulses = fminf(ni_max, frequency)*timeBaseLowWavelength;
      }
          else
          if((wavelength>315)&&(wavelength<=1.0e+06))
          {
              numberOfPulses = fminf(ni_max, frequency)*timeBaseWavelength;
          }
          //cout<< "number of pulses: " << numberOfPulses << endl;
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

 void LaserGoggle::prepareGoggleMark()
{
  expositionData=selectData(wavelength, pulseWidth);
  //printData();
  dataVector=buildDataVector(expositionData, buildScaleNumbers());
  //cout << "Lunghezza d'onda: " << wavelength << endl;
  //cout << "Larghezza dell'impulso: " << pulseWidth << endl;
  //cout << "Uscita del laser: " << powerErg << endl;
  //cout << "Diametro del fascio: " << beamDiameter << endl;
  //cout << "Frequenza: " << frequency << endl;
  double myLaserDensityOutput;
  myLaserDensityOutput = laserIrrRadCorrected(powerErg);
  //cout << "Uscita corretta del dispositivo laser: " << myLaserDensityOutput << endl;
  myScaleNumber=scaleNumber(dataVector, myLaserDensityOutput );
  //cout<< "\nNumero di scala calcolato: " << myScaleNumber << endl;
}

  string LaserGoggle::printGoggleCode()
  {
      if(myScaleNumber)
      {
      std::string wavelength_str = std::to_string(getWavelength());
      std::string myScaleNumber_str = std::to_string(myScaleNumber);
      myGoggleCode= wavelength_str + " " + pulseCode + " LB" + myScaleNumber_str;
      }
      else
          myGoggleCode= "Eccede i valori specificati dalla EN207, non applicabile.";

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

      if(pulseCode=="D")
          codeUnit=power;
      else
          if(pulseCode=="M"){
              if((wavelength>=315.0)and(wavelength<1400.0))
                  codeUnit=energy;
              else
                  codeUnit=power;}
      else
          if((pulseCode=="R") || (pulseCode=="I"))
              codeUnit=energy;

      return codeUnit;
  }

  string LaserGoggle::outputSort()
  {
      if(pulseCode=="D")
          laserOutputSort="E";
      else
          if(pulseCode=="M"){
              if((wavelength>=315.0)and(wavelength<1400.0))
                  laserOutputSort="H<sub>picco</sub>";
              else
                  laserOutputSort="E<sub>picco</sub>";}

          else
              if((pulseCode=="R") || (pulseCode=="I"))
                  laserOutputSort="H";

      return laserOutputSort;
  }


  double LaserGoggle::pulseTrainCorrectionKi()
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
      irrRad=powerErg/beamArea;
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

  void LaserGoggle::setWavelength(const double _wavelength)
  {
      wavelength=_wavelength;
  }

  double LaserGoggle::getPulseWidth()
  {
      return pulseWidth;
  }

  void LaserGoggle::setPulseWidth(const double _pulseWidth)
  {
      if(_pulseWidth==0)
      {
          if((wavelength>=180)&&(wavelength<=315))
              pulseWidth = timeBaseLowWavelength;
                  else
                  if((wavelength>315)&&(wavelength<=1.0e+06))
                      pulseWidth = timeBaseWavelength;
      }
          else
              pulseWidth = _pulseWidth;
  }

  double LaserGoggle::getPowerErg()
  {
      return powerErg;
  }

  void LaserGoggle::setPowerErg(const double _powerErg)
  {
      powerErg=_powerErg;
  }

  double LaserGoggle::getBeamDiameter()
  {
      return beamDiameter;
  }

  void LaserGoggle::setBeamDiameter(const double _beamDiameter)
  {
      beamDiameter=_beamDiameter;
  }

  double LaserGoggle::getFrequency()
  {
      return frequency;
  }

  void LaserGoggle::setFrequency(const double _frequency)
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
      for(int i=0; i<n; i++)
          {
             cout<< "Valori EN207 selezionati: "<< expositionData[i] <<endl;
          }
  }

  void LaserGoggle::printScaleNumbers(int myLB[])
  {
   cout << "I numeri di scala sono i seguenti: \n";
      for(int i=0; i<n; ++i)
          {
             cout<< myLB[i] <<", ";
          }
  }


   void LaserGoggle::printVector( vector< pair <int,double> > dataVector)
     {
      vector< pair <int,double> >::const_iterator constIterator; // const_iterator
      // display vector elements using const_iterator
      for ( constIterator = dataVector.begin();
           constIterator != dataVector.end(); ++constIterator )
         cout << "\nNumero di scala: " << constIterator->first << " valore corrispondente: " <<  constIterator->second;
   } // end function printVector
