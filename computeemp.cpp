#include "computeemp.h"
#include "empsdata.h"
#include <ctype.h>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <iostream>
#include <cmath>


using namespace std;

	const double computeEMP::ALPHA_MIN= 1.5;
	const double computeEMP::ALPHA_MAX= 100.0;
    int computeEMP::fileReadCount=0;
	
computeEMP::computeEMP(double _Wavelength, double _PulseDuration, double _Alpha)
{
    Q_INIT_RESOURCE(LaserStudio);
    Wavelength = _Wavelength;
    PulseDuration = _PulseDuration;
	Alpha=_Alpha;
	EMP();

}

void computeEMP::setWavelength(const double _Wavelength)
{

    Wavelength = _Wavelength;

}

double computeEMP::getWavelength() const
{

    return Wavelength;
}

void computeEMP::setPulseDuration(const double _PulseDuration)
{

        PulseDuration= _PulseDuration;
}

double computeEMP::getPulseDuration() const
{

return PulseDuration;

}

void computeEMP::setAlpha(const double _Alpha)
{

    Alpha = _Alpha;
}

double computeEMP::getAlpha() const
{

    return Alpha;
}


void computeEMP::EMP() {

    int _FormulaNumber=0;
    double _Wavelenght1=0;
    double _Wavelenght2=0;
    double _Time1=0;
    double _Time2=0;
    int _Effects=0;
    int _t=0;
    int _CA=0;
    int _CB=0;
    int _CC=0;
    int _CE=0;
    double _Formula=0;
    int _Sort=0;

    QFile fileBinary(":/EMPdataBinary.dat");
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug("Can't open file");
        exit(1);
    }
    computeEMP::fileReadCount++;

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);

    for(int row=0; row<70; row++)
    {

              in >> _FormulaNumber >> _Wavelenght1 >> _Wavelenght2 >> _Time1 >> _Time2 >> _CA >> _CB
                    >> _CC >> _CE >> _t >> _Effects >> _Formula >> _Sort;

                    /*   qDebug() << _FormulaNumber << _Wavelenght1 << _Wavelenght2 << _Time1 << _Time2
                                << _CA << _CB << _CC << _CE << _t << _Effects << _Formula << _Sort;
                                */

         if(((Wavelength>_Wavelenght1)and(Wavelength<=_Wavelenght2))and((PulseDuration>_Time1)and(PulseDuration<=_Time2 )))
		  {
             LimitValues.formulaNumber=_FormulaNumber;
             LimitValues.Wavelenght1=_Wavelenght1;
             LimitValues.Wavelenght2=_Wavelenght2;
             LimitValues.Time1=_Time1;
             LimitValues.Time2=_Time2;
             LimitValues.Effects=_Effects;
             LimitValues.t=_t;
             LimitValues.CA=_CA;
             LimitValues.CB=_CB;
             LimitValues.CC=_CC;
             LimitValues.CE=_CE;
             LimitValues.formula=_Formula;
             LimitValues.Sort=_Sort;
         }
}
     fileBinary.close();
     //qDebug() << "Numero letture file dati: " << computeEMP::fileReadCount;
	 computeEMP::ComputeParameters();
	 computeEMP::BioEffects();
	 
        if (((Wavelength>=400) and (Wavelength<=1400)) and ((PulseDuration>10) and (PulseDuration<30000)))
			{
			computeEMP::PhotoEffects();
			computeEMP::ThermoEffects();
			computeEMP::EMPPhotoThermo();
			} 
		else{
			 _PhotoGamma=0;		
			 Note="nessuna";
			 computeEMP::valutateFormula();
			 computeEMP::EMP_Value();
			}
}

 void computeEMP::valutateFormula()
{
    string tabCA;
    string tabCB;
    string tabCC;
    string tabCE;
    string tabt;
    string Tipo;
    string coeffValue;

    if (LimitValues.Sort==0)
         Tipo="E";
      else if (LimitValues.Sort==1)
          Tipo="H";

    if (LimitValues.CA==0)
     tabCA = "";
  else
      tabCA = " C<sub>A</sub>";

  if (LimitValues.CB==0)
     tabCB="";
  else
      tabCB=" C<sub>B</sub>";

  if (LimitValues.CC==0)
     tabCC="";
  else
      tabCC=" C<sub>C</sub>";

  if (LimitValues.CE==0)
     tabCE="";
  else
      tabCE=" C<sub>E</sub>";

  if (LimitValues.t==1)
      tabt=" t<sup>0,75</sup>";
  else if (LimitValues.t==2)
      tabt=" t<sup>0,25</sup>";
  else
     tabt="";
 
 // Double to string conversion, the C++03 way:
	ostringstream sstream;
    sstream << LimitValues.formula;
	coeffValue = sstream.str();

	Formula = Tipo + " = " + coeffValue + tabt + tabCA + tabCB + tabCC + tabCE;
	FormulaSort=Tipo;
}

string computeEMP::getRadiation() const
{
	return Radiation;
}

string computeEMP::getSkinDamage() const
{
	return SkinDamage;
}

string computeEMP::getEyeDamage() const
{
	return EyeDamage;
}

string computeEMP::getPhotochemicalNote() const
{
	return Note;
}

void computeEMP::EMP_Value()
{
    double _CA_EMP=0;
    double _CB_EMP=0;
    double _CC_EMP=0;
    double _CE_EMP=0;
    double _t_exp_EMP=0;
	
    if (LimitValues.CA==0)
			_CA_EMP=1;
		else
			_CA_EMP=CA;
		
	if (LimitValues.CB==0)
			_CB_EMP=1;
		else
			_CB_EMP=CB;

	if (LimitValues.CC==0)
			_CC_EMP=1;
		else
			_CC_EMP=CC;			

	if (LimitValues.CE==0)
			_CE_EMP=1.0;
		else
			_CE_EMP=CE;
		
	if (LimitValues.t==0)
			_t_exp_EMP=1;
		else
			_t_exp_EMP=t_exp;
				

	EMP_Result=LimitValues.formula*_CA_EMP*_CB_EMP*_CC_EMP*_CE_EMP*_t_exp_EMP;
}

double computeEMP::getCA() const
{
	
		return CA;
}

double computeEMP::getCB() const
{
		
		return CB;
}

double computeEMP::getCC() const
{
		
		return CC;
}

double computeEMP::getCE() const
{	
		
		return CE;
}

double computeEMP::getT1() const
{
		
		return T1;
}

double computeEMP::getT2() const
{
		
		return T2;
}
	
double computeEMP::getEMP() const
{
	return EMP_Result;
}

double computeEMP::getGamma() const
{
	return _PhotoGamma;
}

void computeEMP::ComputeParameters()
{
		if (Wavelength<700)
				{					
					CA=1;
                }
        else if ((Wavelength>=700) and (Wavelength<=1050))
					{         
						CA=pow(10,0.002*(Wavelength-700));
					}
						else if((Wavelength>1050) and (Wavelength<1400))
						{
							CA=5;
						}
							else
							{								
								CA=1;		
							}		
				
		if ((Wavelength>=400) and (Wavelength<450))
				{
					CB=1;
				} else if ((Wavelength>=450) and (Wavelength<=700))
					{
						CB=pow(10,0.02*(Wavelength-450));
					}
						else
						{						
							CB=1;		
						}	


		if ((Wavelength>=700) and (Wavelength<1150))
				{
					CC=1;
				} else if ((Wavelength>=1150) and (Wavelength<1200))
					{	
						CC=pow(10,0.018*(Wavelength-1150));
					}
					else if ((Wavelength>=1200) and (Wavelength<=1400))
					{
						CC=8;
					}
						else
							{
							CC=1;		
							}	

        if ((Wavelength>=400) and (Wavelength<1400))
        {
            if (Alpha<ALPHA_MIN)
                    {
                        CE=1.0;
                    } else if ((Alpha>=ALPHA_MIN) and (Alpha<=ALPHA_MAX))
                        {
                            CE= Alpha/ALPHA_MIN;
                        }
                        else if (Alpha>ALPHA_MAX)
                            {
                            CE=pow(Alpha,2)/(ALPHA_MIN*ALPHA_MAX);
            }
        }
            else
            {
                CE=1.0;
            }

		if (Wavelength<450)
					{
						T1=1;
					} else if ((Wavelength>=450) and (Wavelength<=500))
						{
							T1=10*pow(10,0.02*(Wavelength-450));
						}
							else if (Wavelength>500)
							{
							T1=100;
							}
					
		if (Alpha<ALPHA_MIN)
					{
						T2=10;
					} else if ((Alpha>=ALPHA_MIN) and (Alpha<=ALPHA_MAX))
						{
							T2=10*pow(10,0.02*((Alpha-ALPHA_MIN)/98.5));
						}
							else if (Alpha>ALPHA_MAX)
							{
							T2=100;
							}
		if (LimitValues.t==1)
					{
						t_exp=pow(PulseDuration, 0.75);
					} else if (LimitValues.t==2)
						{
						t_exp=pow(PulseDuration, 0.25);
						}
							else
							{
							t_exp=1;
							}
}

string computeEMP::getFormulaEMP()const
{
	return Formula;
}

string computeEMP::getFormulaSort() const
{
	return FormulaSort;	
}
	
void computeEMP::PhotoEffects()
{
    if (((Wavelength>=400) and (Wavelength<=600)) and ((PulseDuration>10) and (PulseDuration<100)))
	{	
	_PhotoEMP= 100*CB;
	_PhotoFormulaSort="H";
    _PhotoFormula="H = 100 C<sub>B</sub>";
	_PhotoEMPH=_PhotoEMP;
	_PhotoGamma =11;
	}
	else
	if (((Wavelength>=400) and (Wavelength<=600)) and ((PulseDuration>=100) and (PulseDuration<10000)))
	{	
	_PhotoEMP= 1*CB;
	_PhotoFormulaSort="E";
    _PhotoFormula="E = 1 C<sub>B</sub>";
	_PhotoEMPH=PulseDuration*_PhotoEMP;
	_PhotoGamma =11*pow(PulseDuration,0.5);
	}
	else
	if (((Wavelength>=400) and (Wavelength<=600)) and ((PulseDuration>=10000) and (PulseDuration<=30000)))
	{	
	_PhotoEMP= 1*CB;
	_PhotoFormulaSort="E";
    _PhotoFormula="E = 1 C<sub>B</sub>";
	_PhotoEMPH=PulseDuration*_PhotoEMP;
	_PhotoGamma =110;
	}
	EMP_Result=_PhotoEMP;
	Formula=_PhotoFormula;
	FormulaSort=_PhotoFormulaSort;
}

void computeEMP::ThermoEffects()
{
    if (((Wavelength>=400) and (Wavelength<700)) and ((PulseDuration>10) and (PulseDuration<=30000)))
	{	
         if (Alpha <ALPHA_MIN)
		 {
			_ThermoEMP=10;
			_ThermoFormulaSort="E";
			_ThermoFormula="E =10";
			_ThermoEMPH=_ThermoEMP*PulseDuration;
		 }
		 else
                 if (Alpha >=ALPHA_MIN)
				 {
					 if (PulseDuration<=T2)
					 {
						_ThermoEMP=18*CE*pow(PulseDuration,0.75);
						_ThermoFormulaSort="H";
                        _ThermoFormula="H = 18 C<sub>E</sub> t<sup>0,75</sup>";
						_ThermoEMPH=_ThermoEMP;
					 }
						 else
							 if (PulseDuration>T2)
						{
							_ThermoEMP=18*CE*pow(T2,(-0.25));
							_ThermoFormulaSort="E";
                            _ThermoFormula="E = 18 C<sub>E</sub> T<sub>2</sub><sup>(-0.25)</sup>";
							_ThermoEMPH=_ThermoEMP*PulseDuration;
						}
				}
	}
	else
    if (((Wavelength>=700) and (Wavelength<1400)) and ((PulseDuration>10) and (PulseDuration<=30000)))
	{	
         if (Alpha <ALPHA_MIN)
		 {
            _ThermoFormula="E = 10 C<sub>A</sub> C<sub>C</sub>";
            _ThermoEMP=10*CA*CC;
			_ThermoFormulaSort="E";
			_ThermoEMPH=_ThermoEMP*PulseDuration;
		 } 
		 else 
			 
         if (Alpha >=ALPHA_MIN)
		 {
			 if (PulseDuration<=T2)
				{
				_ThermoEMP=18*CA*CC*CE*pow(PulseDuration,0.75);
				_ThermoEMPH=_ThermoEMP;
				_ThermoFormulaSort="H";
                _ThermoFormula="H = 18 C<sub>A</sub> C<sub>C</sub> C<sub>E</sub> t<sup>0,75</sup>";
				} 
				else 
					if (PulseDuration>T2)
					{
					_ThermoEMP=18*CA*CC*CE*pow(T2,(-0.25));
					_ThermoFormulaSort="E";
					_ThermoEMPH=_ThermoEMP*PulseDuration;
                    _ThermoFormula="E = 18 C<sub>A</sub> C<sub>C</sub> C<sub>E</sub> T<sub>2</sub><sup>(-0.25)</sup>";
					} 
		}
	}
		EMP_Result=_ThermoEMP;
		Formula=_ThermoFormula;
		FormulaSort=_ThermoFormulaSort;
}	
void computeEMP::EMPPhotoThermo()
	{
		if ((Wavelength>=400) and (Wavelength<=600))
		{
			if(_ThermoEMPH<=_PhotoEMPH)
			{
				EMP_Result=_ThermoEMP;
				Formula=_ThermoFormula;
				FormulaSort=_ThermoFormulaSort;
				_PhotoGamma=0;
				Note="prevale l'effetto termico";
			} 
			else 
				if(_PhotoEMPH<_ThermoEMPH)
			{
				EMP_Result=_PhotoEMP;
				Formula=_PhotoFormula;
				FormulaSort=_PhotoFormulaSort;
				Note="prevale l'effetto fotochimico";				
			}
		}
		else 
			Note="nessuna";	
	}
	
void computeEMP::BioEffects()
{
switch (LimitValues.Effects)
    {
	case 1:

	Radiation="UV";
	EyeDamage="danno fotochimico e danno termico;";
    SkinDamage="eritema;"; 

	break;

	case 2:

	Radiation="Visibile";
	EyeDamage="retina";
	SkinDamage="danno termico";
	
	break;

	case 3:
	
	Radiation="IRA";
	EyeDamage="retina";
	SkinDamage="danno termico";

	break;

	case 4:

	Radiation="IRB";
	EyeDamage="danno termico";
	SkinDamage="danno termico";

	break;

	case 5:

	Radiation="visibile";
	EyeDamage="retina, danno forochimico e danno termico";
	SkinDamage="danno termico";

	break;

	case 6:

	Radiation="visibile";
	EyeDamage="retina";
	SkinDamage="danno termico";

	break;

	case 7:
	
	Radiation="IRB, IRC";
	EyeDamage="danno termico";
	SkinDamage="danno termico";

	break;
	
	default:

	Radiation="c'è qualcosa che non va";
	EyeDamage="c'è qualcosa che non va";
	SkinDamage="c'è qualcosa che non va";

	break;
    }
}

void computeEMP::adaptForSkinEMP()
{
    if(PulseDuration<1.0e-09)
        {
        if((Wavelength >= 180) && (Wavelength < 400))
            {
            EMP_Result=3.0e+10;
            Formula="E = 3 10<sup>10</sup>";
            FormulaSort="E";
            }
        else if((Wavelength >= 400) && (Wavelength < 700))
            {
            EMP_Result=2.0e+11;
            Formula="E = 2 10<sup>11</sup>";
            FormulaSort="E";
            }
        else if((Wavelength >= 700) && (Wavelength < 1400))
            {
            EMP_Result=CA*2.0e+11;
            Formula="E = 2 10<sup>11</sup> C<sub>A</sub>";
            FormulaSort="E";
            }
        else if((Wavelength >= 1400) && (Wavelength < 1500))
            {
            EMP_Result=1.0e+12;
            Formula="E = 10<sup>12</sup>";
            FormulaSort="E";
            }
        else if((Wavelength >= 1500) && (Wavelength < 1800))
            {
            EMP_Result=1.0e+13;
            Formula="E = 10<sup>13</sup>";
            FormulaSort="E";
            }
        else if((Wavelength >= 1800) && (Wavelength < 2600))
            {
            EMP_Result=1.0e+12;
            Formula="E = 10<sup>12</sup>";
            FormulaSort="E";
            }
        else if((Wavelength >= 2600) && (Wavelength < 1.0e+06))
            {
            EMP_Result=1.0e+11;
            Formula="E = 10<sup>11</sup>";
            FormulaSort="E";
            }
        }
       if((Wavelength >= 400) && (Wavelength < 1400))
       {
            if((PulseDuration>=1.0e-09) && (PulseDuration<1.0e-07))
            {
            EMP_Result=200*CA;
            Formula="H = 200 C<sub>A</sub>";
            FormulaSort="H";
            }
       if((PulseDuration>=1.0e-07) && (PulseDuration<10))
           {
           EMP_Result=1.1*1.0e+04*CA*pow(PulseDuration,0.25);
           Formula="H = 1,1 10<sup>4</sup> C<sub>A</sub> t<sup>0,25</sup>";
           FormulaSort="H";
           }
       if((PulseDuration>=1.0e+01) && (PulseDuration<3.0e+04))
           {
           EMP_Result=2.0e+03*CA;
           Formula="E = 2 10<sup>3</sup> C<sub>A</sub>";
           FormulaSort="E";
           }
    }
}
