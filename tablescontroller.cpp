#include "tablescontroller.h"

TablesController *TablesController::singleton=nullptr;

// Membro statico del singleton
TablesController *TablesController::getInstance()
{
    if (singleton == nullptr)
        singleton= new TablesController;

    return singleton;
}

TablesController::TablesController():myEmpLeaTables(new EmpLeaTables)
{
}
// Releases the singleton instance of this controller.

void TablesController::destroy() {

    if (singleton != nullptr)
    {
        delete singleton;
        singleton= nullptr;
    }
}

std::array<leadata, EmpLeaTables::TABLEROW_1_1M>TablesController::writeLeaInStructValues_1_1M()
{
    myEmpLeaTables->writeLeaValues_1_1M();
    return myEmpLeaTables->getLeaData_1_1M();
}

std::array<leadata, EmpLeaTables::TABLEROW_3R>TablesController::writeLeaInStructValues_3R()
{
    myEmpLeaTables->writeLeaValues_3R();
    return myEmpLeaTables->getLeaData_3R();
}

std::array<leadata, EmpLeaTables::TABLEROW_3B>TablesController::writeLeaInStructValues_3B()
{
    myEmpLeaTables->writeLeaValues_3B();
    return myEmpLeaTables->getLeaData_3B();
}

std::array<empdata, EmpLeaTables::TABLEROW_EMP> TablesController::writeEmpInStructValues()
{
    myEmpLeaTables->writeEmpValues();
    return myEmpLeaTables->getEmpData();
}
