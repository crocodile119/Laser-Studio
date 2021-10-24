#ifndef LAMBERTIANREFLECTOR_H
#define LAMBERTIANREFLECTOR_H
#include <utility>
#include <vector>
#include "wetreflector.h"
#include "diffusionhazard.h"

using namespace std;


class LambertianReflector: public WetReflector
{
public:
    LambertianReflector();
    void computeTrigonometricReflection();
    void computeZs(double, double);
    void setLambertianMax(const double &);
    double getLambertianMax()const;
    double getLaserBeamDiameter()const;
    double getLaserEMP()const;
    double getLaserPowerErg()const;
    double getLaserDivergence()const;
    double getLaserDistance()const;
    void setLaserBeamDiameter(const double&);
    void setLaserEMP(const double&);
    void setLaserPowerErg(const double&);
    void setLaserDivergence(const double&);
    void setLaserDistance(const double&);
    void setCorrectPositioning(const double&);
    bool evaluateDiffusionDistance();
    void setExendedDiffusion();

    double getConstant()const;
    void setReflectorHazardDistance(const double);
    double getReflectorHazardDistance()const;
    double getSpotDiameter()const;
    void computeSpotDiameter();
    double getAlpha()const;
    double getCE()const;
    QString getKindOfSurface()const;
    double getNewRapSolution()const;
    double getAlphaIndicator()const;
    void setReflectance(const double &);

private:
    double lambertianMax;    
    DiffusionHazard *myDiffusionHazard;
    double laserBeamDiameter;
    double laserEMP;
    double laserPowerErg;
    double laserDivergence;
    double laserDistance;
    double constant;
    double reflectorDistance;
    double reflectorHazardDistance;
    double spotDiameter;
    double correctPositioning;
    double alpha;
    double CE;
    QString kindOfSurface;
    double newRapSolution;
    double alphaIndicator;
    double reflectance;
};

#endif // LAMBERTIANREFLECTOR_H;
