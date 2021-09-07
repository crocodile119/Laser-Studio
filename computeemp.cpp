#include "computeemp.h"
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <iostream>
#include <array>
#include <algorithm>
#include "tablescontroller.h"


using namespace std;

    const double ComputeEMP::ALPHA_MIN= 1.5;
    const double ComputeEMP::ALPHA_MAX= 100.0;
	
ComputeEMP::ComputeEMP(double _wavelength, double _pulseWidth, double _alpha): EMP_Result(0),
    CA(0),CB(0), CC(0), CE(0), T1(0), T2(0), empStructValues{}, myEmpData{}, EMP_FormulaSort(0), _PhotoGamma(0)
{
    /*********************************************************************************
    * Con il singleton sono certo che vi sarà un'unica istanza della classe LeaTable *
    * anche instanziando più oggetti di tipo ComputeLEA                              *
    **********************************************************************************/

    writeEmpInStructValues();
    wavelength = _wavelength;
    pulseWidth = _pulseWidth;
    alpha=_alpha;
	EMP();
}

void ComputeEMP::setWavelength(const double& _wavelength)
{
    wavelength = _wavelength;
}

double ComputeEMP::getWavelength() const
{
    return wavelength;
}

void ComputeEMP::setPulseWidth(const double& _pulseWidth)
{
    pulseWidth=_pulseWidth;
}

double ComputeEMP::getPulseWidth() const
{
    return pulseWidth;
}

void ComputeEMP::setAlpha(const double& _alpha)
{
    alpha = _alpha;
}

double ComputeEMP::getAlpha() const
{
    return alpha;
}

void ComputeEMP::writeEmpInStructValues()
{
    empStructValues=TablesController::getInstance()->writeEmpInStructValues();
}

void ComputeEMP::selectLeaRow()
{
    double wavelenght1;
    double wavelenght2;
    double time1;
    double time2;
    int maxIndex;

    maxIndex=70;

    for(int index=0; index<maxIndex; index++)
    {
        wavelenght1=empStructValues.at(index).wavelenght1;
        wavelenght2=empStructValues.at(index).wavelenght2;
        time1=empStructValues.at(index).time1;
        time2=empStructValues.at(index).time2;

        if(((wavelength>wavelenght1)and(wavelength<=wavelenght2))and((pulseWidth>time1)and(pulseWidth<=time2 )))
        {
            myEmpData=empStructValues.at(index);
            break;
        }
    }
}

void ComputeEMP::EMP()
{
    selectLeaRow();

    ComputeParameters();
    BioEffects();
	 
    if (((wavelength>=400) and (wavelength<=1400)) and ((pulseWidth>10) and (pulseWidth<30000)))
    {
        PhotoEffects();
        ThermoEffects();
        EMPPhotoThermo();
    }
    else{
        _PhotoGamma=0;
        notes="nessuna";
        valutateFormula();
        EMP_Value();
    }
}

void ComputeEMP::valutateFormula()
{
    string tabCA;
    string tabCB;
    string tabCC;
    string tabCE;
    string tabt;
    string Tipo;
    string coeffValue;

    if (myEmpData.sort==0)
        Tipo="E";
    else if (myEmpData.sort==1)
        Tipo="H";

    if (myEmpData.CA==0)
        tabCA = "";
    else
        tabCA = " C<sub>A</sub>";

    if (myEmpData.CB==0)
        tabCB="";
    else
        tabCB=" C<sub>B</sub>";

  if (myEmpData.CC==0)
      tabCC="";
  else
      tabCC=" C<sub>C</sub>";

  if (myEmpData.CE==0)
      tabCE="";
  else
      tabCE=" C<sub>E</sub>";

  if (myEmpData.t==1)
      tabt=" t<sup>0.75</sup>";
  else if (myEmpData.t==2)
      tabt=" t<sup>0.25</sup>";
  else
     tabt="";
 
 // Double to string conversion, the C++03 way:
	ostringstream sstream;
    sstream << myEmpData.formula;
    coeffValue = sstream.str();

    formula = Tipo + " = " + coeffValue + tabt + tabCA + tabCB + tabCC + tabCE;
    formulaSort=Tipo;
}

string ComputeEMP::getRadiation() const
{
    return radiation;
}

string ComputeEMP::getSkinDamage() const
{
    return skinDamage;
}

string ComputeEMP::getEyeDamage() const
{
    return eyeDamage;
}

string ComputeEMP::getPhotochemicalNote() const
{
    return notes;
}

void ComputeEMP::EMP_Value()
{
    double _CA_EMP=0;
    double _CB_EMP=0;
    double _CC_EMP=0;
    double _CE_EMP=0;
    double _t_exp_EMP=0;
	
    if (myEmpData.CA==0)
        _CA_EMP=1;
    else
        _CA_EMP=CA;
		
    if (myEmpData.CB==0)
        _CB_EMP=1;
    else
        _CB_EMP=CB;

    if (myEmpData.CC==0)
        _CC_EMP=1;
    else
        _CC_EMP=CC;

    if (myEmpData.CE==0)
        _CE_EMP=1.0;
    else
        _CE_EMP=CE;
		
    if (myEmpData.t==0)
        _t_exp_EMP=1;
    else
        _t_exp_EMP=t_exp;
				
    EMP_Result=myEmpData.formula*_CA_EMP*_CB_EMP*_CC_EMP*_CE_EMP*_t_exp_EMP;
}

double ComputeEMP::getCA() const
{
    return CA;
}

double ComputeEMP::getCB() const
{
    return CB;
}

double ComputeEMP::getCC() const
{
    return CC;
}

double ComputeEMP::getCE() const
{	
    return CE;
}

double ComputeEMP::getT1() const
{
    return T1;
}

double ComputeEMP::getT2() const
{
    return T2;
}
	
double ComputeEMP::getEMP() const
{
	return EMP_Result;
}

double ComputeEMP::getGamma() const
{
	return _PhotoGamma;
}

void ComputeEMP::ComputeParameters()
{
    if (wavelength<700)
    {
        CA=1;
    }
    else if ((wavelength>=700) and (wavelength<=1050))
    {
        CA=pow(10,0.002*(wavelength-700));
    }
    else if((wavelength>1050) and (wavelength<1400))
    {
        CA=5;
    }
    else
    {
        CA=1;
    }

    if ((wavelength>=400) and (wavelength<450))
    {
        CB=1;
    }
    else if ((wavelength>=450) and (wavelength<=700))
    {
        CB=pow(10,0.02*(wavelength-450));
    }
    else
    {
        CB=1;
    }

    if ((wavelength>=700) and (wavelength<1150))
    {
        CC=1;
    }
    else if ((wavelength>=1150) and (wavelength<1200))
    {
        CC=pow(10,0.018*(wavelength-1150));
    }
    else if ((wavelength>=1200) and (wavelength<=1400))
    {
        CC=8;
    }
    else
    {
        CC=1;
    }

    if ((wavelength>=400) and (wavelength<1400))
    {
        if (alpha<ALPHA_MIN)
        {
            CE=1.0;
        }
        else if ((alpha>=ALPHA_MIN) and (alpha<=ALPHA_MAX))
        {
            CE= alpha/ALPHA_MIN;
        }
        else if (alpha>ALPHA_MAX)
        {
            CE=pow(alpha,2)/(ALPHA_MIN*ALPHA_MAX);
        }
    }
    else
    {
        CE=1.0;
    }

    if (wavelength<450)
    {
        T1=10;
    }
    else if ((wavelength>=450) and (wavelength<=500))
    {
        T1=10*pow(10,0.02*(wavelength-450));
    }
    else if (wavelength>500)
    {
        T1=100;
    }
					
    if (alpha<ALPHA_MIN)
    {
        T2=10;
    }
    else if ((alpha>=ALPHA_MIN) and (alpha<=ALPHA_MAX))
    {
        T2=10*pow(10,(alpha-ALPHA_MIN)/98.5);
    }
    else if (alpha>ALPHA_MAX)
    {
        T2=100;
    }
    if (myEmpData.t==1)
    {
        t_exp=pow(pulseWidth, 0.75);
    }
    else if (myEmpData.t==2)
    {
    t_exp=pow(pulseWidth, 0.25);
    }
    else
    {
    t_exp=1;
    }
}

string ComputeEMP::getFormulaEMP()const
{
    return formula;
}

string ComputeEMP::getFormulaSort() const
{
    return formulaSort;
}
	
void ComputeEMP::PhotoEffects()
{
    if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>10) and (pulseWidth<100)))
	{	
	_PhotoEMP= 100*CB;
	_PhotoFormulaSort="H";
    _PhotoFormula="H = 100 C<sub>B</sub>";
	_PhotoEMPH=_PhotoEMP;
	_PhotoGamma =11;
    photoEmpData.formulaNumber=80;
    photoEmpData.wavelenght1=400;
    photoEmpData.wavelenght1=600;
    photoEmpData.time1=10;
    photoEmpData.time2=100;
    photoEmpData.CA=0;
    photoEmpData.CB=1;
    photoEmpData.CC=0;
    photoEmpData.CE=0;
    photoEmpData.t=0;
    photoEmpData.effects=5;
    photoEmpData.formula=100;
    photoEmpData.sort=1;
	}
	else
    if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>=100) and (pulseWidth<10000)))
	{	
	_PhotoEMP= 1*CB;
	_PhotoFormulaSort="E";
    _PhotoFormula="E = 1 C<sub>B</sub>";
    _PhotoEMPH=pulseWidth*_PhotoEMP;
    _PhotoGamma =1.1*pow(pulseWidth,0.5);
    photoEmpData.formulaNumber=81;
    photoEmpData.wavelenght1=400;
    photoEmpData.wavelenght1=600;
    photoEmpData.time1=100;
    photoEmpData.time2=10000;
    photoEmpData.CA=0;
    photoEmpData.CB=1;
    photoEmpData.CC=0;
    photoEmpData.CE=0;
    photoEmpData.t=0;
    photoEmpData.effects=5;
    photoEmpData.formula=1;
    photoEmpData.sort=0;
	}
	else
    if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>=10000) and (pulseWidth<=30000)))
	{	
	_PhotoEMP= 1*CB;
	_PhotoFormulaSort="E";
    _PhotoFormula="E = 1 C<sub>B</sub>";
    _PhotoEMPH=pulseWidth*_PhotoEMP;
	_PhotoGamma =110;
    photoEmpData.formulaNumber=82;
    photoEmpData.wavelenght1=400;
    photoEmpData.wavelenght1=600;
    photoEmpData.time1=10000;
    photoEmpData.time2=30000;
    photoEmpData.CA=0;
    photoEmpData.CB=1;
    photoEmpData.CC=0;
    photoEmpData.CE=0;
    photoEmpData.t=0;
    photoEmpData.effects=5;
    photoEmpData.formula=1;
    photoEmpData.sort=0;
	}
	EMP_Result=_PhotoEMP;
    formula=_PhotoFormula;
    formulaSort=_PhotoFormulaSort;
}

void ComputeEMP::ThermoEffects()
{
    if (((wavelength>=400) and (wavelength<700)) and ((pulseWidth>10) and (pulseWidth<=30000)))
    {
    if (alpha <ALPHA_MIN)
     {
        _ThermoEMP=10;
        _ThermoFormulaSort="E";
        _ThermoFormula="E = 10";
        _ThermoEMPH=_ThermoEMP*pulseWidth;
        thermoEmpData.formulaNumber=83;
        thermoEmpData.wavelenght1=400;
        thermoEmpData.wavelenght1=700;
        thermoEmpData.time1=10;
        thermoEmpData.time2=30000;
        thermoEmpData.CA=0;
        thermoEmpData.CB=0;
        thermoEmpData.CC=0;
        thermoEmpData.CE=0;
        thermoEmpData.t=0;
        thermoEmpData.effects=0;
        thermoEmpData.formula=10;
        thermoEmpData.sort=0;
     }
     else
        if (alpha >=ALPHA_MIN)
        {
            if (pulseWidth<=T2)
            {
                _ThermoEMP=18*CE*pow(pulseWidth,0.75);
                _ThermoFormulaSort="H";
                _ThermoFormula="H = 18 C<sub>E</sub> t<sup>0.75</sup>";
                _ThermoEMPH=_ThermoEMP;
                thermoEmpData.formulaNumber=84;
                thermoEmpData.wavelenght1=400;
                thermoEmpData.wavelenght1=700;
                thermoEmpData.time1=10;
                thermoEmpData.time2=30000;
                thermoEmpData.CA=0;
                thermoEmpData.CB=0;
                thermoEmpData.CC=0;
                thermoEmpData.CE=1;
                thermoEmpData.t=1;
                thermoEmpData.effects=2;
                thermoEmpData.formula=18;
                thermoEmpData.sort=1;
             }
            else
            if (pulseWidth>T2)
            {
                _ThermoEMP=18*CE*pow(T2,(-0.25));
                _ThermoFormulaSort="E";
                _ThermoFormula="E = 18 C<sub>E</sub> T<sub>2</sub><sup>(-0.25)</sup>";
                _ThermoEMPH=_ThermoEMP*pulseWidth;
                thermoEmpData.formulaNumber=85;
                thermoEmpData.wavelenght1=400;
                thermoEmpData.wavelenght1=700;
                thermoEmpData.time1=10;
                thermoEmpData.time2=30000;
                thermoEmpData.CA=0;
                thermoEmpData.CB=0;
                thermoEmpData.CC=0;
                thermoEmpData.CE=1;
                thermoEmpData.t=0;
                thermoEmpData.effects=2;
                thermoEmpData.formula=18;
                thermoEmpData.sort=1;
            }
        }
	}
	else
    if (((wavelength>=700) and (wavelength<1400)) and ((pulseWidth>10) and (pulseWidth<=30000)))
	{	
        if (alpha <ALPHA_MIN)
        {
            _ThermoFormula="E = 10 C<sub>A</sub> C<sub>C</sub>";
            _ThermoEMP=10*CA*CC;
            _ThermoFormulaSort="E";
            _ThermoEMPH=_ThermoEMP*pulseWidth;
            thermoEmpData.formulaNumber=86;
            thermoEmpData.wavelenght1=700;
            thermoEmpData.wavelenght1=1400;
            thermoEmpData.time1=10;
            thermoEmpData.time2=30000;
            thermoEmpData.CA=1;
            thermoEmpData.CB=0;
            thermoEmpData.CC=1;
            thermoEmpData.CE=0;
            thermoEmpData.t=0;
            thermoEmpData.effects=2;
            thermoEmpData.formula=10;
            thermoEmpData.sort=0;
        }
        else if (alpha >=ALPHA_MIN)
        {
            if (pulseWidth<=T2)
            {
                _ThermoEMP=18*CA*CC*CE*pow(pulseWidth,0.75);
                _ThermoEMPH=_ThermoEMP;
                _ThermoFormulaSort="H";
                _ThermoFormula="H = 18 C<sub>A</sub> C<sub>C</sub> C<sub>E</sub> t<sup>0.75</sup>";
                thermoEmpData.formulaNumber=87;
                thermoEmpData.wavelenght1=700;
                thermoEmpData.wavelenght1=1400;
                thermoEmpData.time1=10;
                thermoEmpData.time2=30000;
                thermoEmpData.CA=1;
                thermoEmpData.CB=0;
                thermoEmpData.CC=1;
                thermoEmpData.CE=1;
                thermoEmpData.t=1;
                thermoEmpData.effects=2;
                thermoEmpData.formula=18;
                thermoEmpData.sort=1;
            }
            else
            if (pulseWidth>T2)
            {
                _ThermoEMP=18*CA*CC*CE*pow(T2,(-0.25));
                if(_ThermoEMP>=1000)
                    _ThermoEMP=1000;

                _ThermoFormulaSort="E";
                _ThermoEMPH=_ThermoEMP*pulseWidth;
                _ThermoFormula="E = 18 C<sub>A</sub> C<sub>C</sub> C<sub>E</sub> T<sub>2</sub><sup>(-0.25)</sup>";
                thermoEmpData.formulaNumber=88;
                thermoEmpData.wavelenght1=700;
                thermoEmpData.wavelenght1=1400;
                thermoEmpData.time1=10;
                thermoEmpData.time2=30000;
                thermoEmpData.CA=1;
                thermoEmpData.CB=0;
                thermoEmpData.CC=1;
                thermoEmpData.CE=1;
                thermoEmpData.t=0;
                thermoEmpData.effects=2;
                thermoEmpData.formula=18;
                thermoEmpData.sort=0;
            }
		}
	}
    EMP_Result=_ThermoEMP;
    formula=_ThermoFormula;
    formulaSort=_ThermoFormulaSort;
}

void ComputeEMP::EMPPhotoThermo()
{
    if ((wavelength>=400) and (wavelength<=600))
    {
        if(_ThermoEMPH<=_PhotoEMPH)
        {
            EMP_Result=_ThermoEMP;
            formula=_ThermoFormula;
            formulaSort=_ThermoFormulaSort;
            _PhotoGamma=0;
            myEmpData=thermoEmpData;
            notes="prevale l'effetto termico";
        }
        else
        if(_PhotoEMPH<_ThermoEMPH)
        {
            EMP_Result=_PhotoEMP;
            formula=_PhotoFormula;
            formulaSort=_PhotoFormulaSort;
            myEmpData=photoEmpData;
            notes="prevale l'effetto fotochimico";
        }
    }
    else
    notes="nessuna";
}

bool ComputeEMP::isPhotochemical()
{
    bool photo=false;
    if(((wavelength>=400)&&(wavelength<=600))&&(_PhotoEMPH<_ThermoEMPH))
        photo=true;

    return photo;
}

void ComputeEMP::BioEffects()
{
switch ((EmpLeaTables::typeOfOutput)myEmpData.effects)
    {
    case EmpLeaTables::UV:
    radiation="UV";
    eyeDamage="danno fotochimico e danno termico;";
    skinDamage="eritema;";
	break;

    case EmpLeaTables::VIS:
    radiation="Visibile";
    eyeDamage="retina";
    skinDamage="danno termico";
    break;

    case EmpLeaTables::IRA:
    radiation="IRA";
    eyeDamage="retina";
    skinDamage="danno termico";
	break;

    case EmpLeaTables::IRA_2:
    radiation="IRB";
    eyeDamage="danno termico";
    skinDamage="danno termico";
	break;

    case EmpLeaTables::VIS_2:
    radiation="visibile";
    eyeDamage="retina, danno forochimico e danno termico";
    skinDamage="danno termico";
	break;

    case EmpLeaTables::VIS_3:
    radiation="visibile";
    eyeDamage="retina";
    skinDamage="danno termico";
	break;

    case EmpLeaTables::IRA_IRB:
    radiation="IRB, IRC";
    eyeDamage="danno termico";
    skinDamage="danno termico";
	break;
	
	default:
    radiation="c'è qualcosa che non va";
    eyeDamage="c'è qualcosa che non va";
    skinDamage="c'è qualcosa che non va";
	break;
    }
}

void ComputeEMP::adaptForSkinEMP()
{
    _PhotoGamma=0;

    if(pulseWidth<1.0e-09)
        {
        if((wavelength >= 180) && (wavelength < 400))
            {
            EMP_Result=3.0e+10;
            formula="E = 3 10<sup>10</sup>";
            formulaSort="E";
            }
        else if((wavelength >= 400) && (wavelength < 700))
            {
            EMP_Result=2.0e+11;
            formula="E = 2 10<sup>11</sup>";
            formulaSort="E";
            }
        else if((wavelength >= 700) && (wavelength < 1400))
            {
            EMP_Result=CA*2.0e+11;
            formula="E = 2 10<sup>11</sup> C<sub>A</sub>";
            formulaSort="E";
            }
        else if((wavelength >= 1400) && (wavelength < 1500))
            {
            EMP_Result=1.0e+12;
            formula="E = 10<sup>12</sup>";
            formulaSort="E";
            }
        else if((wavelength >= 1500) && (wavelength < 1800))
            {
            EMP_Result=1.0e+13;
            formula="E = 10<sup>13</sup>";
            formulaSort="E";
            }
        else if((wavelength >= 1800) && (wavelength < 2600))
            {
            EMP_Result=1.0e+12;
            formula="E = 10<sup>12</sup>";
            formulaSort="E";
            }
        else if((wavelength >= 2600) && (wavelength < 1.0e+06))
            {
            EMP_Result=1.0e+11;
            formula="E = 10<sup>11</sup>";
            formulaSort="E";
            }
        }
       if((wavelength >= 400) && (wavelength < 1400))
       {
            if((pulseWidth>=1.0e-09) && (pulseWidth<1.0e-07))
            {
            EMP_Result=200*CA;
            formula="H = 200 C<sub>A</sub>";
            formulaSort="H";
            }
       if((pulseWidth>=1.0e-07) && (pulseWidth<10))
           {
           EMP_Result=1.1*1.0e+04*CA*pow(pulseWidth,0.25);
           formula="H = 1.1 10<sup>4</sup> C<sub>A</sub> t<sup>0.25</sup>";
           formulaSort="H";
           }
       if((pulseWidth>=1.0e+01) && (pulseWidth<3.0e+04))
           {
           EMP_Result=2.0e+03*CA;
           formula="E = 2 10<sup>3</sup> C<sub>A</sub>";
           formulaSort="E";
           }
    }
}

std::array<empdata, EmpLeaTables::TABLEROW_EMP> ComputeEMP::getEMP_Table()const
{
    return empStructValues;
}

empdata ComputeEMP::getEMP_Data()const
{
    return myEmpData;
}
