#include "lambertianreflector.h"
#include "diffusionhazard.h"
#include <cmath>
#include <iostream>
#include <QDebug>

LambertianReflector::LambertianReflector():WetReflector()
{
}

void LambertianReflector::computeTrigonometricReflection()
{
    reflectionAngleVect.clear();

    for(int i{0}; i<90; i++)
        {
        myAngle=i;    
        myReflectionAngle=cos(myAngle*radDeg);

        reflectionAngleVect.push_back(make_pair(myAngle, myReflectionAngle));
        }

    for(int i{90}; i<270; i++)
        {
        myAngle=i;
        myReflectionAngle=0;

        reflectionAngleVect.push_back(make_pair(myAngle, myReflectionAngle));
        }

        for(int i{270}; i<360; i++)
        {
        myAngle=i;
        myReflectionAngle=cos(myAngle*radDeg);

        reflectionAngleVect.push_back(make_pair(myAngle, myReflectionAngle));
        }
}

void LambertianReflector::setLambertianMax(const double &_lambertianMax)
{
    lambertianMax=_lambertianMax;
}

void LambertianReflector::setReflectance(const double &_reflectance)
{
    reflectance=_reflectance;
}

double LambertianReflector::getLambertianMax()const
{
    return lambertianMax;
}

void LambertianReflector::computeZs(double _lambertianMax, double reflectionCoeff)
{
    vector< pair <double, double> >::iterator iterator; //iterator

    double myZsAngle;
    double myRho_s;
    double sqrt_myRho_s;
    double myZsVector_second;
    int i{0};

    myZsVector.clear();

    // display vector elements using const_iterator
    for (iterator = reflectionAngleVect.begin();
         iterator != reflectionAngleVect.end(); ++iterator )
      {
        myZsAngle=i;
        myRho_s=iterator->second;
                sqrt_myRho_s=std::sqrt(myRho_s);
                myZsVector_second=sqrt_myRho_s*_lambertianMax*std::pow(reflectionCoeff,0.5);
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
         i++;
      }
}

bool LambertianReflector::evaluateDiffusionDistance()
{
    bool isExendex=false;
    myDiffusionHazard=new DiffusionHazard(spotDiameter, laserEMP, laserPowerErg); //beamDiameter, divergence, EMP, powerErg. laserDistance

    qDebug()<< "Diametro del fascio laser" << laserBeamDiameter;
    qDebug()<< "Divergenza del fascio laser" << laserDivergence;
    qDebug()<< "Valore limite di esposizione del fascio laser" << laserEMP;
    qDebug()<< "Potenza/enegia del laser" << laserPowerErg;
    qDebug()<< "Distanza dal laser" << laserDistance;


    if(myDiffusionHazard->computeExendedReflection(1.0e-02))            //1.0e-02 è la eps
    {

        qDebug()<<"Alpha: "<< myDiffusionHazard->getAlpha();
        qDebug()<<"Alpha indicator: "<< myDiffusionHazard->getAlphaIndicator();

        constant=myDiffusionHazard->getConstant();
        reflectorDistance=myDiffusionHazard->getReflectorDistance();
        spotDiameter=myDiffusionHazard->getSpotDiameter();
        qDebug()<<"Diametro dello spot: " << spotDiameter;
        CE=myDiffusionHazard->getCE();
        kindOfSurface=myDiffusionHazard->getKindOfSurface();
        newRapSolution=myDiffusionHazard->getNewRapSolution();
        alpha=myDiffusionHazard->getAlpha();
        alphaIndicator=myDiffusionHazard->getAlphaIndicator();
        reflectorHazardDistance=reflectance*myDiffusionHazard->getReflectorHazardDistance();
        isExendex=true;
    }

    return isExendex;
}

double LambertianReflector::getLaserBeamDiameter()const
{
    return laserBeamDiameter;
}

double LambertianReflector::getLaserEMP()const
{
    return laserEMP;
}

double LambertianReflector::getLaserPowerErg()const
{
    return laserPowerErg;
}

void LambertianReflector::setLaserBeamDiameter(const double& _laserBeamDiameter)
{
    laserBeamDiameter=_laserBeamDiameter;
}

void LambertianReflector::setLaserEMP(const double& _laserEMP)
{
    laserEMP=_laserEMP;
}

void LambertianReflector::setLaserPowerErg(const double& _laserPowerErg)
{
    laserPowerErg=_laserPowerErg;
}

double LambertianReflector::getLaserDivergence()const
{
    return laserDivergence;
}

void LambertianReflector::setLaserDivergence(const double& _laserDivergence)
{
    laserDivergence=_laserDivergence;
}

double LambertianReflector::getLaserDistance()const
{
    return laserDistance;
}

void LambertianReflector::setLaserDistance(const double& _laserDistance)
{
    laserDistance=_laserDistance;
}

void LambertianReflector::setCorrectPositioning(const double & _correctPositioning)
{
    correctPositioning=_correctPositioning;
}

double LambertianReflector::getConstant()const
{
    return constant;
}

double LambertianReflector::getReflectorHazardDistance()const
{
    return reflectorHazardDistance;
}

void LambertianReflector::setReflectorHazardDistance(const double _reflectorHazardDistance)
{
    reflectorHazardDistance=_reflectorHazardDistance;
}

double LambertianReflector::getSpotDiameter()const
{
    return spotDiameter;
}

double LambertianReflector::getAlpha()const
{
    return alpha;
}

double LambertianReflector::getCE()const
{
    return CE;
}

QString LambertianReflector::getKindOfSurface()const
{
    return kindOfSurface;
}

double LambertianReflector::getNewRapSolution()const
{
    return newRapSolution;
}

double LambertianReflector::getAlphaIndicator()const
{
    return alphaIndicator;
}

void LambertianReflector::setExendedDiffusion()
{
    //Imposto i dati membro necessari per il calcolo della diffusione estesa
    myDiffusionHazard->setEMP(laserEMP);
    myDiffusionHazard->setPowerErg(laserPowerErg);
}

void LambertianReflector::computeSpotDiameter()
{
    double positioningCosine=cos(radDeg*correctPositioning);
    qDebug()<<"coseno dell'angolo complessivo formato dal riflettore rispetto al fascio:"<<positioningCosine;
    qDebug()<<"angolo complessivo formato dal riflettore rispetto al fascio: "<<correctPositioning;
    spotDiameter=(laserBeamDiameter+laserDistance*laserDivergence)*positioningCosine;
}
