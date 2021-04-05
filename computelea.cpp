#include "computelea.h"
#include "leadata.h"
#include <ctype.h>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <iostream>
#include <cmath>
#include "tablescontroller.h"

using namespace std;

    const double ComputeLEA::ALPHA_MIN= 1.5;

ComputeLEA::ComputeLEA(double _wavelength, double _pulseWidth, double _alpha):leaStructValues_1M{}, leaStructValues_3R{}, leaStructValues_3B{},
    LEA_Formula{}, LEA_FormulaSort{}, LEA_FormulaTipo{},
    LEA_FormulaUnit{}, myLeaData_1M{}, myLeaData_3R{}, myLeaData_3B{}
{
    wavelength = _wavelength;
    pulseWidth = _pulseWidth;
    alpha=_alpha;

    /*********************************************************************************
    * Con il singleton sono certo che vi sarà un'unica istanza della classe LeaTable *
    * anche instanziando più oggetti di tipo ComputeLEA                              *
    **********************************************************************************/

    leaStructValues_1M=TablesController::getInstance()->writeLeaInStructValues_1_1M();
    leaStructValues_3R=TablesController::getInstance()->writeLeaInStructValues_3R();
    leaStructValues_3B=TablesController::getInstance()->writeLeaInStructValues_3B();

    calculate();
}

void ComputeLEA::writeLeaInStructValues(ClassData myClassData)
{
    if(myClassData==ClassData::CLASSE_1_1M)
        leaStructValues_1M=TablesController::getInstance()->writeLeaInStructValues_1_1M();
    if(myClassData==ClassData::CLASSE_3R)
        leaStructValues_3R=TablesController::getInstance()->writeLeaInStructValues_3R();
    if(myClassData==ClassData::CLASSE_3B)
        leaStructValues_3B=TablesController::getInstance()->writeLeaInStructValues_3B();
}

void ComputeLEA::setWavelength(const double &_wavelength)
{
    wavelength = _wavelength;
}

double ComputeLEA::getWavelength() const
{
    return wavelength;
}

void ComputeLEA::setPulseWidth(const double &_pulseWidth)
{
    pulseWidth=_pulseWidth;
}

double ComputeLEA::getPulseWidth() const
{
    return pulseWidth;
}

void ComputeLEA::setAlpha(const double &_alpha)
{
    alpha = _alpha;
}

double ComputeLEA::getAlpha() const
{
    return alpha;
}

void ComputeLEA::selectLea_1M_Row(const std::array<leadata, EmpLeaTables::TABLEROW_1_1M> &myLeaStructValues)
{    
    double wavelenght1;
    double wavelenght2;
    double time1;
    double time2;
    int maxIndex;

    maxIndex=41;

    for(int index=0; index<maxIndex; index++)
    {
        wavelenght1=myLeaStructValues.at(index).wavelenght1;
        wavelenght2=myLeaStructValues.at(index).wavelenght2;
        time1=myLeaStructValues.at(index).time1;
        time2=myLeaStructValues.at(index).time2;


        if(((wavelength>wavelenght1)and(wavelength<=wavelenght2))and((pulseWidth>time1)and(pulseWidth<=time2 )))
         {
            myLeaData_1M=myLeaStructValues.at(index);
            break;
         }
    }
}

void ComputeLEA::selectLea_3R_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3R> &myLeaStructValues)
{
    double wavelenght1;
    double wavelenght2;
    double time1;
    double time2;
    int maxIndex;

    maxIndex=38;

    for(int index=0; index<maxIndex; index++)
    {
        wavelenght1=myLeaStructValues.at(index).wavelenght1;
        wavelenght2=myLeaStructValues.at(index).wavelenght2;
        time1=myLeaStructValues.at(index).time1;
        time2=myLeaStructValues.at(index).time2;


        if(((wavelength>wavelenght1)and(wavelength<=wavelenght2))and((pulseWidth>time1)and(pulseWidth<=time2 )))
         {
            myLeaData_3R=myLeaStructValues.at(index);
            break;
         }
    }
}

void ComputeLEA::selectLea_3B_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3B> &myLeaStructValues)
{
    double wavelenght1;
    double wavelenght2;
    double time1;
    double time2;
    int maxIndex;

    maxIndex=21;

    for(int index=0; index<maxIndex; index++)
    {
        wavelenght1=myLeaStructValues.at(index).wavelenght1;
        wavelenght2=myLeaStructValues.at(index).wavelenght2;
        time1=myLeaStructValues.at(index).time1;
        time2=myLeaStructValues.at(index).time2;


        if(((wavelength>wavelenght1)and(wavelength<=wavelenght2))and((pulseWidth>time1)and(pulseWidth<=time2 )))
         {
            myLeaData_3B=myLeaStructValues.at(index);
            break;
         }
    }
}

void ComputeLEA::LEA()
{
    selectLea_1M_Row(leaStructValues_1M);
    selectLea_3R_Row(leaStructValues_3R);
    selectLea_3B_Row(leaStructValues_3B);

    //Correggo le formule nel caso in cui siano previsti due pssibili valori

    if(((wavelength>302.5)and(wavelength<=315))and((pulseWidth>1.0e-009)and(pulseWidth<=10 )))
    {
        if(pulseWidth>T1)
        {
            myLeaData_1M.C1=0;
            myLeaData_1M.C2=1;
        }
    }

    if(((wavelength>450)and(wavelength<=500))and((pulseWidth>1.0e+001)and(pulseWidth<=1.0e+002 )))
    {
        if(3.9e-003*C3>3.9e-003*pulseWidth)
        {
            myLeaData_1M.formula=3.9e-003;
            myLeaData_1M.C3=0;
            myLeaData_1M.sort=3;
        }
    }

    if(((wavelength>302.5)and(wavelength<=315))and((pulseWidth>1.0e-009)and(pulseWidth<=10 )))
    {
        if(pulseWidth>T1)
        {
            myLeaData_3R.C1=0;
            myLeaData_3R.C2=1;
        }
    }

    if(((wavelength>400)and(wavelength<=700))and((pulseWidth>5.0e-006)and(pulseWidth<=0.35 )))
    {
        if(pulseWidth>=0.25)
        {
            myLeaData_3R.formula=5.0e-003;
            myLeaData_3R.t=0;
            myLeaData_3R.sort=3;
        }
    }

    if(((wavelength>400)and(wavelength<=700))and((pulseWidth>1.0e-009)and(pulseWidth<=0.25 )))
    {
        if(pulseWidth>=0.06)
        {
            myLeaData_3B.formula=0.5;
            myLeaData_3B.sort=3;
        }
    }

    if(((wavelength>700)and(wavelength<=1050))and((pulseWidth>1.0e-009)and(pulseWidth<=0.25 )))
    {
        if(pulseWidth>=0.06*C4)
        {
            myLeaData_3B.formula=0.5;
            myLeaData_3B.sort=3;
        }
    }

    LEA_Formula[0]=valuateFormula(myLeaData_1M);
    LEA_Formula[1]=valuate_LEA_2M_Formula();
    LEA_Formula[2]=valuateFormula(myLeaData_3R);
    LEA_Formula[3]=valuateFormula(myLeaData_3B);

    LEA_FormulaTipo[0]=valuateFormulaTipo(myLeaData_1M);
    LEA_FormulaTipo[1]=valuate_LEA_2M_FormulaTipo();
    LEA_FormulaTipo[2]=valuateFormulaTipo(myLeaData_3R);
    LEA_FormulaTipo[3]=valuateFormulaTipo(myLeaData_3B);

    LEA_FormulaUnit[0]=valuateFormulaUnit(myLeaData_1M);
    LEA_FormulaUnit[1]=valuate_LEA_2M_FormulaUnit();
    LEA_FormulaUnit[2]=valuateFormulaUnit(myLeaData_3R);
    LEA_FormulaUnit[3]=valuateFormulaUnit(myLeaData_3B);

    LEA_Result[0]=LEA_Value(myLeaData_1M);
    LEA_Result[1]=valuate_LEA_2M_Value();
    LEA_Result[2]=LEA_Value(myLeaData_3R);
    LEA_Result[3]=LEA_Value(myLeaData_3B);

    LEA_FormulaSort[0]=myLeaData_1M.sort;

    if(pulseWidth<0.25)
        LEA_FormulaSort[1]=myLeaData_1M.sort;
    else
        LEA_FormulaSort[1]=3;

    LEA_FormulaSort[2]=myLeaData_3R.sort;
    LEA_FormulaSort[3]=myLeaData_3B.sort;
}

double ComputeLEA::valuate_LEA_2M_Value()
{
    double leaValue;
    if((wavelength<=700)and(wavelength>=400))
    {
        if(pulseWidth<0.25)
            leaValue=LEA_Result[static_cast<int>(ClassData::CLASSE_1_1M)];
    else
            leaValue=C6*1.0e-003;
    }
    else
    {
        leaValue=std::nan("N.A.");
    }
        return leaValue;
}

string ComputeLEA::valuate_LEA_2M_Formula()
{
    string leaFormula;
    if((wavelength<=700)and(wavelength>=400))
    {
        if(pulseWidth<0.25)
            leaFormula=LEA_Formula[int(static_cast<int>(ClassData::CLASSE_1_1M))];
    else
            leaFormula="C6 10<sup>-3</sup>";
    }
    else
    {
        leaFormula="N.A.";
    }
    return leaFormula;
}

string ComputeLEA::valuate_LEA_2M_FormulaTipo()
{
    string leaFormulaTipo;
    if((wavelength<=700)and(wavelength>=400))
    {
        if(pulseWidth<0.25)
            leaFormulaTipo=LEA_FormulaTipo[static_cast<int>(ClassData::CLASSE_1_1M)];
    else
            leaFormulaTipo="P";
    }
    else
    {
        leaFormulaTipo="";
    }
    return leaFormulaTipo;
}

string ComputeLEA::valuate_LEA_2M_FormulaUnit()
{
    string leaFormulaUnit;
    if((wavelength<=700)and(wavelength>=400))
    {
        if(pulseWidth<0.25)
            leaFormulaUnit=LEA_FormulaUnit[static_cast<int>(ClassData::CLASSE_1_1M)];
    else
            leaFormulaUnit="W";
    }
    else
    {
        leaFormulaUnit="";
    }
    return leaFormulaUnit;
}

string ComputeLEA::valuateFormula(leadata &myLeaData)
{
    string tabC1;
    string tabC2;
    string tabC3;
    string tabC4;
    string tabC5;
    string tabC6;
    string tabC7;
    string tabt;
    string Tipo;
    string coeffValue;

    if (myLeaData.C1==0)
     tabC1 = "";
  else
      tabC1 = " C<sub>1</sub>";

  if (myLeaData.C2==0)
     tabC2="";
  else
      tabC2=" C<sub>2</sub>";

  if (myLeaData.C3==0)
     tabC3="";
  else
      tabC3=" C<sub>3</sub>";

  if (myLeaData.C4==0)
     tabC4="";
  else
      tabC4=" C<sub>4</sub>";
  
    if (myLeaData.C5==0)
     tabC5="";
  else
      tabC5=" C<sub>5</sub>";
  
    if (myLeaData.C6==0)
     tabC6="";
  else
      tabC6=" C<sub>6</sub>";
  
    if (myLeaData.C7==0)
     tabC7="";
  else
      tabC7=" C<sub>7</sub>";

  if (myLeaData.t==0)
    tabt="";
  else if (myLeaData.t==1)
    tabt=" t<sup>0,75</sup>";
  else if (myLeaData.t==2)
    tabt=" t<sup>0,25</sup>";
  else
    tabt=" t";
 
 // Double to string conversion, the C++03 way:
	ostringstream sstream;
    sstream << myLeaData.formula;
	coeffValue = sstream.str();

    string Formula = coeffValue + tabt + tabC1 + tabC2 + tabC3 + tabC4 + tabC5+ tabC6+ tabC7;

    return Formula;
}

string ComputeLEA::valuateFormulaTipo(leadata &myLeaData)
{
    string Tipo;

    if (myLeaData.sort==1)
         Tipo="E";
      else if (myLeaData.sort==2)
                  Tipo="H";
            else if (myLeaData.sort==3)
                        Tipo="P";
                else
                            Tipo="Q";

    return Tipo;
}


string ComputeLEA::valuateFormulaUnit(leadata &myLeaData)
{
    string FormulaUnit;

    if (myLeaData.sort==1)
         FormulaUnit="W/m<sup>2</sup>";
      else if (myLeaData.sort==2)
                  FormulaUnit="J/m<sup>2</sup>";
            else if (myLeaData.sort==3)
                        FormulaUnit="W";
                    else
                            FormulaUnit="J";

    return FormulaUnit;
}

string ComputeLEA::getRadiation() const
{
    return radiation;
}

string ComputeLEA::getSkinDamage() const
{
    return skinDamage;
}

string ComputeLEA::getEyeDamage() const
{
    return eyeDamage;
}

double ComputeLEA::LEA_Value(leadata &myLeaData)
{
    double _C1_LEA=0;
    double _C2_LEA=0;
    double _C3_LEA=0;
    double _C4_LEA=0;
    double _C5_LEA=0;
    double _C6_LEA=0;
    double _C7_LEA=0;
    double _t_exp_EMP=0;

    if (myLeaData.C1==0)
			_C1_LEA=1;
		else
			_C1_LEA=C1;
		
    if (myLeaData.C2==0)
			_C2_LEA=1;
		else
			_C2_LEA=C2;

    if (myLeaData.C3==0)
			_C3_LEA=1;
		else
			_C3_LEA=C3;		

    if (myLeaData.C4==0)
			_C4_LEA=1;
		else
			_C4_LEA=C4;
				
    if (myLeaData.C5==0)
			_C5_LEA=1;
		else
			_C5_LEA=C5;

    if (myLeaData.C6==0)
            _C6_LEA=1;
		else
            _C6_LEA=1;
		
    if (myLeaData.C7==0)
			_C7_LEA=1;
		else
            _C7_LEA=C7;

double t_exp =compute_t(myLeaData);

    if (myLeaData.t==0)
			_t_exp_EMP=1;
		else
			_t_exp_EMP=t_exp;
				
double LEA_Result=myLeaData.formula*_C1_LEA*_C2_LEA*_C3_LEA*_C4_LEA*_C5_LEA*_C6_LEA*_C7_LEA*_t_exp_EMP;
return LEA_Result;
}

double ComputeLEA::getC1() const
{
    return C1;
}

double ComputeLEA::getC2() const
{
    return C2;
}

double ComputeLEA::getC3() const
{
    return C3;
}

double ComputeLEA::getC4() const
{	
    return C4;
}

double ComputeLEA::getC5() const
{	
    return C5;
}

double ComputeLEA::getC6() const
{	
    return C6;
}

double ComputeLEA::getC7() const
{	
    return C7;
}

double ComputeLEA::getT1() const
{
    return T1;
}

double ComputeLEA::getT2() const
{
    return T2;
}

double ComputeLEA::getGamma() const
{
    return photoGamma;
}

void ComputeLEA::computeParameters()
{
    computeAlpha_max();
    if (wavelength<700)
				{					
					C4=1;
                }
        else if ((wavelength>=700) and (wavelength<=1050))
					{         
                        C4=pow(10,0.002*(wavelength-700));
					}
                        else if((wavelength>1050) and (wavelength<1400))
						{
							C4=5;
						}
							else
							{								
								C4=1;		
							}		
				
        if ((wavelength>=400) and (wavelength<450))
				{
					C3=1;
                } else if ((wavelength>=450) and (wavelength<=700))
					{
                        C3=pow(10,0.02*(wavelength-450));
					}
						else
						{						
							C3=1;		
						}	


        if ((wavelength>=700) and (wavelength<1150))
				{
					C7=1;
                } else if ((wavelength>=1150) and (wavelength<1200))
					{	
                        C7=pow(10,0.018*(wavelength-1150));
					}
                    else if ((wavelength>=1200) and (wavelength<=1400))
					{
                        C7=8+pow(10,0.04*(wavelength-1250));
					}
						else
							{
							C7=1;		
							}	

        if ((wavelength>=180) and (wavelength<302.5))
				{
					C2=30;
                } else if ((wavelength>=302.5) and (wavelength<315))
						{
                        C2=pow(10, 0.2*(wavelength-295));
						}
						else
							C2=1;

        if ((wavelength>=180) and (wavelength<400))
				{
                    C1=5.6*pow(10,3)*pow(pulseWidth, 0.25);
				} else
						{
						C1=1;		
						}	
		
		C5=1;		

        if ((wavelength>=400) and (wavelength<1400))
			{
            if (alpha<ALPHA_MIN)
					{
						C6=1.0;
                    } else if ((alpha>=ALPHA_MIN) and (alpha<=alpha_max))
						{
                            C6= alpha/ALPHA_MIN;
						}
                        else if (alpha>alpha_max)
							{
                            C6=pow(alpha,2)/(ALPHA_MIN*alpha_max);
							}
			}
			else{
						C6=1;
					}
					
        if ((wavelength>=302.5) and (wavelength<=315))
					{
                        T1=10*pow(10,0.8*(wavelength-295))*1.0e-015;
					} else
                        T1=std::nan("Nessun valore");

					
        if (alpha<ALPHA_MIN)
					{
						T2=10;
                    } else if ((alpha>=ALPHA_MIN) and (alpha<=alpha_max))
						{
                            T2=10*pow(10,0.02*((alpha-ALPHA_MIN)/98.5));
						}
                            else if (alpha>alpha_max)
							{
							T2=100;
							}

        computePhotoGamma();

}

double ComputeLEA::compute_t(leadata &myLeaData)
{
    double t_exp;

    if (myLeaData.t==1){
        t_exp=pow(pulseWidth, 0.75);}
    else if (myLeaData.t==2){
        t_exp=pow(pulseWidth, 0.25);}
    else if (myLeaData.t==3){
        t_exp=pulseWidth;}
    else{t_exp=1;}

    return t_exp;
}


array<double, ComputeLEA::N_LEA> ComputeLEA::getLEA() const
{
    return LEA_Result;
}

array <string, ComputeLEA::N_LEA> ComputeLEA::getLEA_Formula()const
{
    return LEA_Formula;
}

array <string, ComputeLEA::N_LEA> ComputeLEA::getLEA_FormulaTipo() const
{
    return LEA_FormulaTipo;
}

array <string, ComputeLEA::N_LEA> ComputeLEA::getLEA_FormulaUnit() const
{
    return LEA_FormulaUnit;
}

array <int, ComputeLEA::N_LEA> ComputeLEA::getLEA_FormulaSort() const
{
    return LEA_FormulaSort;
}

void ComputeLEA::computePhotoGamma()
{
    if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>10) and (pulseWidth<100)))
        photoGamma =11;
        else if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>=100) and (pulseWidth<10000)))
            photoGamma =11*pow(pulseWidth,0.5);
            else
                if (((wavelength>=400) and (wavelength<=600)) and ((pulseWidth>=10000) and (pulseWidth<=30000)))
                photoGamma =110;
                    else
                        photoGamma=std::nan("N.A.");

}

void ComputeLEA::computeAlpha_max()
{
if(pulseWidth<6.25e-004)
    alpha_max=5;
    else if((pulseWidth>=6.25e-004)and(pulseWidth<0.25))
        alpha_max=200*pow(pulseWidth, 0.5);
            else if(pulseWidth>=0.25)
            alpha_max=100;
}

void ComputeLEA::bioEffects()
{
switch (myLeaData_1M.effects)
    {
	case 1:

    radiation="UV";
    eyeDamage="danno fotochimico e danno termico;";
    skinDamage="eritema;";

	break;

	case 2:

    radiation="Visibile";
    eyeDamage="retina";
    skinDamage="danno termico";
	
	break;

	case 3:
	
    radiation="IRA";
    eyeDamage="retina";
    skinDamage="danno termico";

	break;

	case 4:

    radiation="IRB";
    eyeDamage="danno termico";
    skinDamage="danno termico";

	break;

	case 5:

    radiation="visibile";
    eyeDamage="retina, danno forochimico e danno termico";
    skinDamage="danno termico";

	break;

	case 6:

    radiation="visibile";
    eyeDamage="retina";
    skinDamage="danno termico";

	break;

	case 7:
	
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

string ComputeLEA::typeOfSkinDamage(const int &_effects)
{
    string mySkinDamage;
    switch (_effects)
        {
        case 1:
        mySkinDamage="eritema;";
        break;

        case 2:
        mySkinDamage="danno termico";

        break;

        case 3:
        mySkinDamage="danno termico";
        break;

        case 4:
        mySkinDamage="danno termico";
        break;

        case 5:
        mySkinDamage="danno termico";
        break;

        case 6:
        mySkinDamage="danno termico";
        break;

        case 7:
        mySkinDamage="danno termico";
        break;

        default:
        mySkinDamage="c'è qualcosa che non va";
        break;
        }
    return mySkinDamage;
}

string ComputeLEA::typeOfEyeDamage(const int &_effects)
{
    string myEyeDamage;
        switch (_effects)
        {
        case 1:
        myEyeDamage="danno fotochimico e danno termico;";
        break;

        case 2:
        myEyeDamage="retina";
        break;

        case 3:
        myEyeDamage="retina";
        break;

        case 4:
        myEyeDamage="danno termico";
        break;

        case 5:
        myEyeDamage="retina, danno forochimico e danno termico";
        break;

        case 6:
        myEyeDamage="retina";
        break;

        case 7:
        myEyeDamage="danno termico";
        break;

        default:
        myEyeDamage="c'è qualcosa che non va";
        break;
        }
    return myEyeDamage;
}

string ComputeLEA::typeOfRadiation(const int & _effects)
{
    string myRadiation;
        switch (_effects)
        {
        case 1:
        myRadiation="UV";
        break;

        case 2:
        myRadiation="Visibile";
        break;

        case 3:
        myRadiation="IRA";
        break;

        case 4:
        myRadiation="IRB";
        break;

        case 5:
        myRadiation="visibile";
        break;

        case 6:
        myRadiation="visibile";
        break;

        case 7:
        myRadiation="IRB, IRC";
        break;

        default:
        myRadiation="c'è qualcosa che non va";
        break;
        }
    return myRadiation;
}

void ComputeLEA::valuateCondition_1()
{
if((wavelength>=302.5)and(wavelength<400))
    {
    apertureStopCond_1=7;
    distanceCond_1=2000;
    }
    else if((wavelength>=400)and(wavelength<1400))
        {
        apertureStopCond_1=50;
        distanceCond_1=2000;
        }
        else if((wavelength>=1400)and(wavelength<4000))
            {
            apertureStopCond_1=7*apertureThirdCondition;
            distanceCond_1=2000;
            }
            else
            {
            apertureStopCond_1=std::nan("N.A.");
            distanceCond_1=std::nan("N.A.");
    }
}

void ComputeLEA::valuateCondition_3()
{
if(wavelength<302.5)
    {
    apertureStopCond_3=1;
    distanceCond_3=0;
    }
    else if((wavelength>=302.5)and(wavelength<400))
        {
        apertureStopCond_3=1;
        distanceCond_3=100;
        }
        else if((wavelength>=400)and(wavelength<1400))
            {
            apertureStopCond_3=7;
            distanceCond_3=100;
            }
            else if((wavelength>=1400)and(wavelength<4000))
                {
                apertureStopCond_3=apertureThirdCondition;
                distanceCond_3=100;
                }
                else if((wavelength>=4000)and(wavelength<1e+005))
                    {
                    apertureStopCond_3=apertureThirdCondition;
                    distanceCond_3=0;
                    }
                        else if((wavelength>=1e+005)and(wavelength<1e+006))
                        {
                        apertureStopCond_3=11;
                        distanceCond_3=0;
                        }
}

void ComputeLEA::valuateApertureCondition_3()
{
if(pulseWidth<=0.35)
    apertureThirdCondition=1;
        else if((pulseWidth>0.35)and(pulseWidth<10))
            apertureThirdCondition=powf(pulseWidth, 3/8);
            else
                apertureThirdCondition=3.5;
}

double ComputeLEA::getApertureStopCond_1()const
{
    return apertureStopCond_1;
}

double ComputeLEA::getApertureStopCond_3()const
{
    return apertureStopCond_3;
}

double ComputeLEA::getDistanceCond_1() const
{
    return distanceCond_1;
}

double ComputeLEA::getDistanceCond_3()const
{
    return distanceCond_3;
}

double ComputeLEA::getApertureThirdCondition()const
{
    return apertureThirdCondition;
}

void ComputeLEA::applyLEA_CorrectionFactor(const double& correction)
{
    for(size_t i=0; i<ComputeLEA::N_LEA; i++)
    {
        LEA_Result[i]=LEA_Result[i]*correction;
    }
}

void ComputeLEA::calculate()
{
    computeParameters();
    bioEffects();
    LEA();
    valuateApertureCondition_3();
    valuateCondition_3();
    valuateCondition_1();
}
