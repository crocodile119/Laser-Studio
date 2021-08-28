#ifndef SCIENTIFICNUMBER_H
#define SCIENTIFICNUMBER_H

class ScientificNumber
{
public:
    ScientificNumber(double _number);
    int exponent(double _number, int &_exp);
    double mantissa(double _number, const int &_exp);
    void setFloatNumber(const double _number);
    double getMantissa()const;
    int getExponent()const;
    void computeScientificNumber();
    double ceil() const;
    double floor()const;
    double numberLimit();

private:
   double floatNumber;
   double mant;
   int exp;

};

#endif // SCIENTIFICNUMBER_H
