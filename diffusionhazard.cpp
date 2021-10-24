#include "diffusionhazard.h"
#include <QDebug>
#include <cmath>

const double DiffusionHazard::ALPHA_MIN= 1.5;
const double DiffusionHazard::ALPHA_MAX= 100.0;
const double DiffusionHazard::PI_GRECO = 3.141592653589793;

DiffusionHazard::DiffusionHazard(double _spotDiameter, double _EMP, double _powerErg):
spotDiameter(_spotDiameter),  EMP(_EMP), powerErg(_powerErg)
{
}

void DiffusionHazard::computeAlpha()
{
    alpha=newRapSolution*1e+03;
}

void DiffusionHazard::computeCE()
{
    if(alpha<1.5)
        CE=1;
    else if((1.5<=alpha)&&(alpha<100))
        CE= alpha/ALPHA_MIN;
    else
        CE= ALPHA_MAX/ALPHA_MIN;
}

void DiffusionHazard::computeAlphaIndicator()
{
    alphaIndicator= alpha/ALPHA_MIN;
}

/***************************************************************************************
 * Metodo di Newton Rapson per il calcolo del valore di alpha in corrispondenza del       *
 * quale gli effetti di diffusioni divengono non pericolosi                             *
 ****************************************************************************************/

double DiffusionHazard::newtonRapsonReflectorDistance(double _eps, double testValue)
{
    int i;
    int n=10000; //massino numero di iterazioni
    double h=0.0000001;
    double x; 			//valore di prova
    double f1; 			//valore di f nel punto x
    double x1; 			//valore di h+x
    double eps; 		//precisione richiesta

//imposto il valore della funzione in corrispondenza del valore di prova e la precisione desiderata

    eps=_eps;
    x=testValue;
    f1=distanceFunction(x);

//imposto la variabile conv a falso (la funzione al primo tentativo non converge)
    bool conv=false;

    i=0;

//ciclo while
    while ((i<n) || (!conv))
    {
        i = i+1;
        x1= x+h;
        x=x-(h*f1/(distanceFunction(x1)-f1));
        f1=distanceFunction(x);
        conv=(std::abs(f1)< eps);
    }
    newRapSolution=x;

    return newRapSolution;
}

void DiffusionHazard::setKindOfSurface(kindOfSurface _surface)
{
    surface=_surface;
}

QString DiffusionHazard::getKindOfSurface()
{
    QString mySurface;
    if(surface==kindOfSurface::MEDIUM)
        mySurface="MEDIA";
    else if(surface==kindOfSurface::LARGE)
        mySurface="GRANDE";
    else
        mySurface="PUNTIFORME";

    return mySurface;
}

double DiffusionHazard::distanceFunction(double testValue)
{
    double function=0;

    switch(surface)
    {
        case(kindOfSurface::MEDIUM):
            function=testValue/(1-cos(testValue/2))-constant;
            break;
        case(kindOfSurface::LARGE):
            function=1/(1-cos(testValue/2))-constant;
            break;
        case(kindOfSurface::POINT):
            function=1/(1-cos(testValue/2))-constant;
            break;
    }
    return function;
}

void DiffusionHazard::setPowerErg(const double& _powerErg)
{
    powerErg=_powerErg;
}

double DiffusionHazard::getPowerErg()const
{
    return powerErg;
}

double DiffusionHazard::getReflectorHazardDistance()const
{
    return reflectorHazardDistance;
}

void DiffusionHazard::setReflectorDistance(const double& _reflectorDistance)
{
    reflectorDistance=_reflectorDistance;
}

double DiffusionHazard::getReflectorDistance()const
{
    return reflectorDistance;
}

double DiffusionHazard::getEMP()const
{
    return EMP;
}

void DiffusionHazard::setEMP(const double& _EMP)
{
    EMP=_EMP;
}

double DiffusionHazard::getEps()const
{
    return eps;
}

void DiffusionHazard::setEps(const double& _eps)
{
    eps=_eps;
}

double DiffusionHazard::getConstant()const
{
    return constant;
}

void DiffusionHazard::computeConstant()
{
    double spotDiameterInMeter;
    spotDiameterInMeter=spotDiameter*1.0e-03;

    double mediumConstant= 8/PI_GRECO*powerErg/EMP*ALPHA_MIN*1.0e-03/(std::pow(spotDiameterInMeter,2));
    double largeConstant=8/PI_GRECO*powerErg/EMP*ALPHA_MIN/ALPHA_MAX/(std::pow(spotDiameterInMeter,2));
    double pointConstant= 8/PI_GRECO*powerErg/EMP*1.0e-03/(std::pow(spotDiameterInMeter,2));

    switch(surface)
    {
        case(kindOfSurface::MEDIUM):
            constant=mediumConstant;
            qDebug()<<"Constante superficie media: "<< constant;
            break;
        case(kindOfSurface::LARGE):
            constant= largeConstant;
            qDebug()<<"Constante superficie grande: "<< constant;
            break;
        case(kindOfSurface::POINT):
            constant=pointConstant;
            qDebug()<<"Constante superficie puntiforme: "<< constant;
            break;
    }
}

double DiffusionHazard::getSpotDiameter()const
{
    return spotDiameter;
}

double DiffusionHazard::getAlpha()const
{
    return alpha;
}

double DiffusionHazard::getCE()const
{
    return CE;
}

double DiffusionHazard::getAlphaIndicator()const
{
    return alphaIndicator;
}

bool DiffusionHazard::computeExendedReflection(double _eps)
{
    bool computed=false;
    alphaIndicator=0;
    setKindOfSurface(kindOfSurface::MEDIUM);
    computeConstant();
    qDebug()<<"Constant MEDIUM: " << getConstant();

    /*******************************************************************************************************************
     * Ipotizzo teta molto piccolo in modo da poter esprimere il coseno in serie di Taylor fino al secondo termine.    *
     * Tale ipotesi è verificata per alpha che varia nell'intervallo tra 1.5e-03 e 0.1 radianti.                       *
     * Questa approssimazione è utile per il calcolo del valore di prova dell'algoritmo di Newton Rapson.              *
     * Per angoli  molto più grandi di 0* (siamo nel caso di superfici estese) la scelta del valore iniziale           *
     * coincidente con l'approssimazione fornito dalla serie di Taylor non è più valida                                *
     * in questi casi si dovrebbe:                                                                                     *
     *                                                                                                                 *
     *  - o escludere  i casi in cui alpha supera un certo valore (riflettere sul significato fisico dell'angolo       *
     *    alpha e sull'opportunità che questo assuma valori molto più grandi di 0,1 rad ad esempio la distanza non     *
     *    dovrebbe superare un certo valore minimo per motivi di accessibilità della riflessione);                     *
     *  - o calcolare un valore di prova diverso che porti alla risoluzione dell'equazione.                            *
     *                                                                                                                 *
     *    Gestione delle eccezioni da implementare ad esempio variando l'incremento o il numero di iterazioni          *
     *    nel metodo di Newton Rapson.                                                                                 *
     *                                                                                                                 *
     *******************************************************************************************************************/

    double myConst=getConstant();

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
        return computed;


    testValue=8/getConstant();
    qDebug()<<"Valore di prova: " << testValue;

    newRapSolution=newtonRapsonReflectorDistance(_eps, testValue);
    computeAlpha();
    computeAlphaIndicator();
    computeReflectorHazardDistance();
    computeCE();

   if((alphaIndicator>ALPHA_MAX/ALPHA_MIN)||(alphaIndicator<1))
    {
        setKindOfSurface(kindOfSurface::LARGE);
        computeConstant();
        testValue=std::sqrt(8/getConstant());

        newRapSolution=newtonRapsonReflectorDistance(_eps, testValue);
        computeAlpha();
        computeAlphaIndicator();
        computeReflectorHazardDistance();
        computeCE();

        if(alphaIndicator<=ALPHA_MAX/ALPHA_MIN)
        {
            setKindOfSurface(kindOfSurface::POINT);
            reflectorHazardDistance=std::sqrt(powerErg/(PI_GRECO*EMP));
            computed=false;
        }
        else
            computed=true;
    }
    else
        computed=true;

   return computed;
}

void DiffusionHazard::computeReflectorHazardDistance()
{
    double spotDiameterInMeter;

    spotDiameterInMeter=spotDiameter*1.0e-03;

    reflectorHazardDistance=(spotDiameterInMeter)/newRapSolution;
}

double DiffusionHazard::getNewRapSolution()
{
    return newRapSolution;
}

double DiffusionHazard::getTestValue()
{
    return testValue;
}
