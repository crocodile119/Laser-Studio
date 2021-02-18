#ifndef EMPLEATABLES_H
#define EMPLEATABLES_H

#include "leadata.h"
#include "empdata.h"
#include <QtGui>
#include <array>

class EmpLeaTables
{
public:
    enum tableRows{TABLEROW_1_1M=41, TABLEROW_3R=38, TABLEROW_3B=21, TABLEROW_EMP=70};
    enum typeOfOutput{UV=1, VIS=2, IRA=3, IRA_2=4, VIS_2=5, VIS_3=6, IRA_IRB=7};
    EmpLeaTables();
    void writeEmpValues();
    void writeLeaValues_1_1M();
    void writeLeaValues_3R();
    void writeLeaValues_3B();
    std::array<empdata, TABLEROW_EMP> getEmpData();
    std::array<leadata, TABLEROW_1_1M> getLeaData_1_1M();
    std::array<leadata, TABLEROW_3R> getLeaData_3R();
    std::array<leadata, TABLEROW_3B> getLeaData_3B();

private:
    std::array<empdata, TABLEROW_EMP> empValues;
    std::array<leadata, TABLEROW_1_1M> leaValues_1_1M;
    std::array<leadata, TABLEROW_3R> leaValues_3R;
    std::array<leadata, TABLEROW_3B> leaValues_3B;
};

#endif // EMPLEATABLES_H
