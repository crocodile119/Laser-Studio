#ifndef LASERSAFETYCW_H
#define LASERSAFETYCW_H
#include "lasersafety.h"

using namespace std;

class LaserSafetyCW : public LaserSafety
{
public:
LaserSafetyCW(double, double, double, double, double, double);

void computeNOHD();
double getPowerErgForEMP();
void computeLambertianMax();
void setExposureTime();
double getExposureTime() const;
void laserUpdate();
void setExposureTimeEditable(bool);
bool isTimeEditable();
void setEditedExposureTime(const double &);

protected:
double exposureTime;
bool timeEditable;


private:

};

#endif // LASERSAFETYCW_H
