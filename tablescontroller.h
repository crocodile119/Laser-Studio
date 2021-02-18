#ifndef TABLESCONTROLLER_H
#define TABLESCONTROLLER_H
#include "empleatables.h"


class TablesController
{
public:
    TablesController();
    // Funzione membro del singleton
    static TablesController *getInstance();

    //Distrugge l'istanza del singleton
    static void destroy();
    std::array<leadata, EmpLeaTables::TABLEROW_1_1M>  writeLeaInStructValues_1_1M();
    std::array<leadata, EmpLeaTables::TABLEROW_3R> writeLeaInStructValues_3R();
    std::array<leadata, EmpLeaTables::TABLEROW_3B> writeLeaInStructValues_3B();
    std::array<empdata, EmpLeaTables::TABLEROW_EMP> writeEmpInStructValues();

private:

  static TablesController *singleton; // pointer to the singleton

   EmpLeaTables *myEmpLeaTables;

};

#endif // TABLESCONTROLLER_H
