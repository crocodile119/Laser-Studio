#include "fresnelreflector.h"
#include <cmath>
#include <iostream>
#include <QDebug>

const double FresnelReflector::radDeg = 3.1415926535897932384626433832795/180;

FresnelReflector::FresnelReflector(double n_refraction): n(n_refraction)
{
}

void FresnelReflector::computeTrigonometricReflection()
{
    reflectionAngleVect.clear();
    rho_iVect.clear();
    rho_sVect.clear();

    for(int i=0; i<=180; i++)
    {
        myAngle=90.0/(180.0)*i;
        myReflectionAngle=asin(1/n*sin(myAngle*radDeg));

        double myNumerator=pow(sin(myAngle*radDeg-myReflectionAngle),2);
        double myDenominator=pow(sin(myAngle*radDeg+myReflectionAngle),2);

        if(myDenominator==0)
            rho_i=0;
        else
            rho_i =myNumerator/myDenominator;

        rho_s=2*rho_i/(1+rho_i);

        reflectionAngleVect.push_back(make_pair(myAngle, myReflectionAngle));
        rho_iVect.push_back(make_pair(myAngle, rho_i));
        rho_sVect.push_back(make_pair(myAngle, rho_s));
    }
}

double FresnelReflector::getRefration_n() const
{
    return n;
}

void FresnelReflector::setRefraction_n(const double& _n)
{
    n=_n;
}

void FresnelReflector::printVector( vector< pair <double,double> > myVector)
{
   vector< pair <double,double> >::const_iterator constIterator; // const_iterator
   // display vector elements using const_iterator
   for ( constIterator = myVector.begin(); constIterator != myVector.end(); ++constIterator )
   {
        cout << "Angolo: " << constIterator->first << " Secondo elemento: " <<  constIterator->second << endl;
   }
}

vector< pair <double,double> > FresnelReflector::getReflectionAngleVect()
{
    return reflectionAngleVect;
}

vector< pair <double,double> > FresnelReflector::getRho_iVect()
{
    return rho_iVect;
}

vector< pair <double,double> > FresnelReflector::getRho_sVect()
{
    return rho_sVect;
}

double FresnelReflector::getDNRO() const
{
    return dnro;
}

void FresnelReflector::setDNRO(const double& _dnro)
{
    dnro=_dnro;
}

double FresnelReflector::getMaxElement()
{
    int myZsVectorHalfSize=myZsVector.size()/2;
    myMaxElement=myZsVector.at(myZsVectorHalfSize).second;
    return myMaxElement;
}

void FresnelReflector::computeZs(vector< pair <double,double> > myVector, double _dnro, double _objectDistance, double materialCoeff)
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
    for (iterator = myVector.begin(); iterator != myVector.end(); ++iterator )
    {
        myRho_s=iterator->second;
        myRho_s *=materialCoeff;
        sqrt_myRho_s=pow(myRho_s, 0.5);
        myZsVector_second=sqrt_myRho_s*_dnro-_objectDistance;
        myZsAngle=i;

        if(myZsVector_second>0)
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
        else
            myZsVector.push_back(make_pair(myZsAngle, 0));

        i++;
      }
    for (reverse_iterator = myVector.rbegin()+1; reverse_iterator != myVector.rend(); ++reverse_iterator )
    {
        myRho_s=reverse_iterator->second;
        myRho_s*=materialCoeff;
        sqrt_myRho_s=pow(myRho_s, 0.5);
        myZsVector_second=sqrt_myRho_s*_dnro-_objectDistance;
        myZsAngle=i;

        if(myZsVector_second>0)
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
        else
            myZsVector.push_back(make_pair(myZsAngle, 0));

        i++;
      }
    printVector(myZsVector);
}

vector< pair <double,double> > FresnelReflector::getZsVect()
{
    return myZsVector;
}

double FresnelReflector::getObjectDistance()const
{
    return objectDistance;
}

void FresnelReflector::setObjectDistance(const double &_objectDistance)
{
    objectDistance=_objectDistance;
}
