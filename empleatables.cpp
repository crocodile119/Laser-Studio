#include "empleatables.h"
#include "leadata.h"
#include "empdata.h"
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <array>
#include <algorithm>

const double EmpLeaTables::HE_NE_WAVELENGTH=633;
const double EmpLeaTables::NATURAL_AVERSION_TIME=0.25;
const double EmpLeaTables::ALPHA_MIN=1.5;
const double EmpLeaTables::ALPHA_MAX=100.0;

EmpLeaTables::EmpLeaTables():empValues{},leaValues_1_1M{},leaValues_3R{},leaValues_3B{}
{
    Q_INIT_RESOURCE(LaserStudio);

    writeEmpValues();
    writeLeaValues_1_1M();
    writeLeaValues_3R();
    writeLeaValues_3B();
}

void EmpLeaTables::writeLeaValues_1_1M()
{
    int _formulaNumber;
    double _wavelenght1;
    double _wavelenght2;
    double _time1;
    double _time2;
    int _effects;
    int _t;
    int _C1;
    int _C2;
    int _C3;
    int _C4;
    int _C5;
    int _C6;
    int _C7;
    double _formula;
    int _sort;
    int maxRow;
    QString filename;

    maxRow=TABLEROW_1_1M;
    filename=":/CLASSE_1_1M_dataBinary.dat";

    QFile fileBinary(filename);
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug()<<("Can't open file");
        exit(1);
    }

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);


    for(int row=0; row<maxRow; row++)
    {
              in >> _formulaNumber >> _wavelenght1 >> _wavelenght2 >> _time1 >> _time2 >> _C1 >> _C2
                    >> _C3 >> _C4 >> _C5 >> _C6 >> _C7 >> _t >> _effects >> _formula >> _sort;

              leaValues_1_1M.at(row)={_formulaNumber, _wavelenght1, _wavelenght2, _time1, _time2,
                      _C1, _C2, _C3, _C4, _C5, _C6, _C7, _t, _effects, _formula, _sort};
    }
     fileBinary.close();
}

void EmpLeaTables::writeLeaValues_3R()
{
    int _formulaNumber;
    double _wavelenght1;
    double _wavelenght2;
    double _time1;
    double _time2;
    int _effects;
    int _t;
    int _C1;
    int _C2;
    int _C3;
    int _C4;
    int _C5;
    int _C6;
    int _C7;
    double _formula;
    int _sort;
    int maxRow;
    QString filename;

    maxRow=TABLEROW_3R;
    filename=":/CLASSE_3R_dataBinary.dat";

    QFile fileBinary(filename);
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug()<<("Can't open file");
        exit(1);
    }

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);


    for(int row=0; row<maxRow; row++)
    {

              in >> _formulaNumber >> _wavelenght1 >> _wavelenght2 >> _time1 >> _time2 >> _C1 >> _C2
                    >> _C3 >> _C4 >> _C5 >> _C6 >> _C7 >> _t >> _effects >> _formula >> _sort;

              leaValues_3R.at(row)={_formulaNumber, _wavelenght1, _wavelenght2, _time1, _time2,
                      _C1, _C2, _C3, _C4, _C5, _C6, _C7, _t, _effects, _formula, _sort};
    }
     fileBinary.close();
}

void EmpLeaTables::writeLeaValues_3B()
{
    int _formulaNumber;
    double _wavelenght1;
    double _wavelenght2;
    double _time1;
    double _time2;
    int _effects;
    int _t;
    int _C1;
    int _C2;
    int _C3;
    int _C4;
    int _C5;
    int _C6;
    int _C7;
    double _formula;
    int _sort;
    int maxRow;
    QString filename;

    maxRow=TABLEROW_3B;
    filename=":/CLASSE_3B_dataBinary.dat";

    QFile fileBinary(filename);
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug()<<("Can't open file");
        exit(1);
    }

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);


    for(int row=0; row<maxRow; row++)
    {

              in >> _formulaNumber >> _wavelenght1 >> _wavelenght2 >> _time1 >> _time2 >> _C1 >> _C2
                    >> _C3 >> _C4 >> _C5 >> _C6 >> _C7 >> _t >> _effects >> _formula >> _sort;

              leaValues_3B.at(row)={_formulaNumber, _wavelenght1, _wavelenght2, _time1, _time2,
                      _C1, _C2, _C3, _C4, _C5, _C6, _C7, _t, _effects, _formula, _sort};
    }
     fileBinary.close();
}

void EmpLeaTables::writeEmpValues() {

    int _formulaNumber;
    double _wavelenght1;
    double _wavelenght2;
    double _time1;
    double _time2;
    int _effects;
    int _t;
    int _CA;
    int _CB;
    int _CC;
    int _CE;
    double _formula;
    int _sort;
    int rowNumber=TABLEROW_EMP;
    QString filename=":/EMPdataBinary.dat";

    QFile fileBinary(filename);
    if (!fileBinary.open(QIODevice::ReadOnly))
    {
        qDebug()<<("Can't open file");
        exit(1);
    }

    QDataStream in(&fileBinary);
    in.setVersion(QDataStream::Qt_4_3);

    for(int row=0; row<rowNumber; row++)
    {
              in >> _formulaNumber >> _wavelenght1 >> _wavelenght2 >> _time1 >> _time2 >> _CA >> _CB
                    >> _CC >> _CE >> _t >> _effects >> _formula >> _sort;

              empValues.at(row)={_formulaNumber, _wavelenght1, _wavelenght2, _time1, _time2,
                                 _CA, _CB, _CC, _CE, _t, _effects, _formula, _sort};
    }
     fileBinary.close();
}

std::array<leadata, EmpLeaTables::TABLEROW_1_1M> EmpLeaTables::getLeaData_1_1M()
{
   return leaValues_1_1M;
}

std::array<leadata, EmpLeaTables::TABLEROW_3R> EmpLeaTables::getLeaData_3R()
{
   return leaValues_3R;
}

std::array<leadata, EmpLeaTables::TABLEROW_3B> EmpLeaTables::getLeaData_3B()
{
   return leaValues_3B;
}

std::array<empdata, EmpLeaTables::TABLEROW_EMP> EmpLeaTables::getEmpData()
{
   return empValues;
}
