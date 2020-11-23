#include "wetreflector.h"
#include <cmath>
#include <iostream>

const double WetReflector::phi_const = 2.5;

WetReflector::WetReflector(double n_refraction):FresnelReflector(n_refraction)
{
     n=n_refraction;
}

void WetReflector::computeTrigonometricReflection()
{

    reflectionAngleVect.clear();
    rho_sVect.clear();

    for(int i=0; i<=180; i++)
    {
        myAngle=90.0/(180.0)*i;
        myReflectionAngle=asin(1/n*sin(myAngle*radDeg));

        double myNumerator=pow(sin(myAngle*radDeg-myReflectionAngle),2);
        double myDenominator=pow(sin(myAngle*radDeg+myReflectionAngle),2);

        if(myDenominator==0)
            rho_s=0;
        else
            rho_s =myNumerator/myDenominator;

        reflectionAngleVect.push_back(make_pair(myAngle, myReflectionAngle));
        rho_sVect.push_back(make_pair(myAngle, rho_s));
    }
}

void WetReflector::computeZs(vector< pair <double,double> > myVector, double _dnro,
                                   double _objectDistance, double _divergence, double reflectionCoeff)
{
    vector< pair <double, double> >::iterator iterator; // const_iterator
    vector< pair <double, double> >::reverse_iterator reverse_iterator;
    double myRho_s;
    double sqrt_myRho_s;
    double myZsAngle;
    double myZsVector_second;
    int i=0;

    myZsVector.clear();

    // display vector elements using const_iterator
    for (iterator = myVector.begin();
         iterator != myVector.end(); ++iterator )
      {
        myRho_s=iterator->second;
        myRho_s*=reflectionCoeff;
                sqrt_myRho_s=pow(myRho_s, 0.5);
                    double divergenceCorrection;
                       divergenceCorrection=_divergence/(_divergence+phi_const);
                        myZsVector_second=(sqrt_myRho_s*_dnro*divergenceCorrection)-(_objectDistance*divergenceCorrection);
                                 myZsAngle=i;
        if(myZsVector_second>0)
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
               else
                   myZsVector.push_back(make_pair(myZsAngle, 0));
         i++;
      }
    for (reverse_iterator = myVector.rbegin()+1;
         reverse_iterator != myVector.rend(); ++reverse_iterator )
      {
        myRho_s=reverse_iterator->second;
        myRho_s*=reflectionCoeff;
                sqrt_myRho_s=pow(myRho_s, 0.5);
                 double divergenceCorrection;
                    divergenceCorrection=_divergence/(_divergence+phi_const);
                        myZsVector_second=(sqrt_myRho_s*_dnro*divergenceCorrection)-(_objectDistance*divergenceCorrection);
                                 myZsAngle=i;
        if(myZsVector_second>0)
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
               else
                   myZsVector.push_back(make_pair(myZsAngle, 0));
         i++;
      }
}

double WetReflector::getDivergence()const
{
    return divergence;
}

void WetReflector::setDivergence(const double& _divergence)
{
    divergence=_divergence;
}
