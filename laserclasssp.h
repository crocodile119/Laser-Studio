#ifndef LASERCLASSSP_H
#define LASERCLASSSP_H
#include "laserclasscw.h"

using namespace std;

class LaserClassSP : public LaserClassCW
{
public:
    LaserClassSP(double, double, double, double, double, double);

    void setPulseWidth(const double);
    double getPulseWidth() const;
    double* leaPowerErgUnit(int*, const double &, const double &);
    void updateAll();

protected:
    double pulseWidth;
};
#endif // LASERCLASSSP_H
