#ifndef DIFFUSIONHAZARD_H
#define DIFFUSIONHAZARD_H
#include <QString>


class DiffusionHazard
{
public:
    enum class kindOfSurface{MEDIUM, LARGE, POINT};
    DiffusionHazard(double, double, double);
    void computeAlpha();
    void computeCE();
    bool computeExendedReflection(double);
    void computeReflectorHazardDistance();
    void computeAlphaIndicator();
    void setPowerErg(const double&);
    double getPowerErg()const;
    double getReflectorHazardDistance()const;
    void setReflectorDistance(const double&);
    double getReflectorDistance()const;
    double getEMP()const;
    void setEMP(const double&);
    double getEps()const;
    void setEps(const double&);
    double getSpotDiameter()const;
    double getAlpha()const;
    double getCE()const;
    double getAlphaIndicator()const;
    double getConstant()const;
    void computeConstant();
    void setKindOfSurface(kindOfSurface);
    QString getKindOfSurface();
    double getNewRapSolution();
    double distanceFunction(double x);
    double newtonRapsonReflectorDistance(double, double);
    double getTestValue();

    const static double ALPHA_MIN;
    const static double ALPHA_MAX;
    const static double PHI;
    const static double PI_GRECO;

private:
    double spotDiameter;
    double reflectorHazardDistance;
    double divergence;
    double laserDistance;
    double reflectorDistance;
    double alpha;
    double EMP;
    double diffuseRadiance;
    double CE;
    double maxPerformance;
    double powerErg;
    double diffusion;
    double distanceToNoEffects;
    double alphaNoEffects;
    double eps;
    double constant;
    double newRapSolution;
    double alphaIndicator; //rapporto tra alpha e alpha_min senza le condizioni previste per CE
    kindOfSurface surface;
    double testValue;
};

#endif // DIFFUSIONHAZARD_H
