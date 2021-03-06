#ifndef WETREFLECTOR_H
#define WETREFLECTOR_H
#include <utility>
#include <vector>
#include "fresnelreflector.h"

using namespace std;


class WetReflector : public FresnelReflector
{
public:
    WetReflector(double);

    static const double phi_const;

    void computeZs(vector< pair <double,double> >, double, double, double, double);
    void computeTrigonometricReflection();
    double getDivergence()const;
    void setDivergence(const double&);

private:
    double divergence;
};

#endif // WETREFLECTOR_H
