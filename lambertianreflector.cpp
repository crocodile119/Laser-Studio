#include "lambertianreflector.h"
#include "diffusionhazard.h"
#include <cmath>
#include <iostream>
#include <QDebug>

LambertianReflector::LambertianReflector(double n_refraction):WetReflector(n_refraction)
{
     n=n_refraction;
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
                sqrt_myRho_s=sqrt(myRho_s);
                myZsVector_second=sqrt_myRho_s*_lambertianMax*pow(reflectionCoeff,0.5);
            myZsVector.push_back(make_pair(myZsAngle, myZsVector_second));
         i++;
      }
}

bool LambertianReflector::evaluateDiffusionDistance()
{
    myDiffusionHazard=new DiffusionHazard(laserBeamDiameter, laserDivergence, laserDistance, laserEMP, laserPowerErg); //beamDiameter, divergence, EMP, powerErg. laserDistance

    qDebug()<< "Diametro del fascio laser" << laserBeamDiameter;
    qDebug()<< "Divergenza del fascio laser" << laserDivergence;
    qDebug()<< "Valore limite di esposizione del fascio laser" << laserEMP;
    qDebug()<< "Potenza/enegia del laser" << laserPowerErg;
    qDebug()<< "Distanza dal laser" << laserDistance;

    myDiffusionHazard->setKindOfSurface(DiffusionHazard::kindOfSurface::MEDIUM);
    myDiffusionHazard->computeConstant();
    myDiffusionHazard->computeSpotDiameter();

    double myConst;

    myConst=myDiffusionHazard->getConstant();

    qDebug()<< "Termine a secondo membro dell'equazione: "<< myConst;

    /**************************************************************************************************************************
    * Nell'ipotesi di reflessioni diffuse da superficie non puntiforme l'angolo alpha non può essere maggiore di PI_GRECO     *
    * (corrispondente a metà angolo solido), quindi l'argomento del coseno è al più pari a PI_GRECO/2.                        *
    * Ciò significa che:                                                                                                      *
    *   - quando l'angolo alpha è uguale a PI_GRECO, la funzione x/(1-cos(x/2)) assume valore PI_GRECO;                       *
    *   - la funzione ha un asintoto verticale in 0 per + infinito;                                                           *
    *   - la funzione è inoltre strettamente decrescente.                                                                     *
    * Ne consegue che affinchè possa essere applicata l'equazione per il calcolo della diffusione da superficie estesa è      *
    * necessario che la costante ricavata sia maggiore di PI_GRECO in caso contrario l'algoritmo per il calcolo della         *
    * soluzione certamente non converge, qualunque sia il valore del valore di prova.                                                                                      *
    ***************************************************************************************************************************/

    if(myConst<DiffusionHazard::PI_GRECO)
    {
        return false;
    }
    else
    {
       myDiffusionHazard->computeExendedReflection(1.0e-02);            //1.0e-02 è la eps
    }

    myDiffusionHazard->computeAlpha();

    if(myDiffusionHazard->getAlpha()>=100)
        return false;

    qDebug()<<"Alpha: "<< myDiffusionHazard->getAlpha();
    qDebug()<<"Alpha indicator: "<< myDiffusionHazard->getAlphaIndicator();

    if(myDiffusionHazard->getAlphaIndicator()<1)
    {
        return false;
    }
    else
    {
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

        return true;
    }
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
    myDiffusionHazard->setDivergence(laserDivergence);
    myDiffusionHazard->setLaserDistance(laserDistance);
    myDiffusionHazard->setBeamDiameter(laserBeamDiameter);
}

void LambertianReflector::computeSpotDiameter()
{
    spotDiameter=laserBeamDiameter+laserDistance*laserDivergence;
}
