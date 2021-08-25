#ifndef FRESNELREFLECTOR_H
#define FRESNELREFLECTOR_H
#include <utility>
#include <vector>

using namespace std;

class FresnelReflector
{
public:
    FresnelReflector(double);
    FresnelReflector();

    const static double radDeg;

    void printVector( vector< pair <double, double> > );
    vector< pair <double, double> > getReflectionAngleVect();
    vector< pair <double, double> > getRho_iVect();
    vector< pair <double, double> > getRho_sVect();
    vector< pair <double,double> > getZsVect();
    void computeTrigonometricReflection();
    double getRefration_n() const;
    void setRefraction_n(const double&);
    double getDNRO() const;
    void setDNRO(const double&);
    void computeZs(vector< pair <double,double> >, double, double, double);
    double getObjectDistance()const;
    void setObjectDistance(const double&);
    double getMaxElement();

protected:

    vector <std::pair<double, double> > reflectionAngleVect;
    vector <std::pair<double, double> > rho_iVect;
    vector <std::pair<double, double> > rho_sVect;
    vector< pair <double, double> > myZsVector;

    double n;
    double angleOfRefraction;
    double myAngle;
    double myReflectionAngle;
    double rho_i;
    double rho_s;
    double dnro;
    double objectDistance;
    double myMaxElement;
};

#endif // FRESNELREFLECTOR_H
