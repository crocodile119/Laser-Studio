#include "scientificnumber.h"
#include <cmath>

ScientificNumber::ScientificNumber(double _floatNumber):floatNumber(_floatNumber)
{
    int exp=0;
    this->exp=exponent(floatNumber, exp);
    this->mant=mantissa(floatNumber, exp);
}

void ScientificNumber::computeScientificNumber()
{
    int exp=0;
    this->exp=exponent(floatNumber, exp);
    this->mant=mantissa(floatNumber, exp);
}

int ScientificNumber::exponent(double number, int &exp)
{
    if((number<1)&&(number>-1)&&(number!=0))
    {
        number=number*10;
        exp++;
        return exponent(number, exp);
    }
    else
        return exp;
}

double ScientificNumber::mantissa(double number, const int &exp)
{
    return number*pow(10, exp);
}

void ScientificNumber::setFloatNumber(const double _floatNumber)
{
    floatNumber=_floatNumber;
}

double ScientificNumber::getMantissa()const
{
    return mant;
}

int ScientificNumber::getExponent()const
{
    return exp;
}

double ScientificNumber::ceil()const
{
    return std::ceil(mant)*pow(10, -exp);
}

double ScientificNumber::floor()const
{
    return std::floor(mant)*pow(10, -exp);
}

double ScientificNumber::numberLimit()
{
    double approx;

    if(floatNumber>0)
        approx=ceil();
    else
        approx=floor();

    return approx;
}
