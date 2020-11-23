#include "dockresults.h"
#include "ui_dockresults.h"

DockResults::DockResults(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockResults)
{
    ui->setupUi(this);
}

DockResults::~DockResults()
{
    delete ui;
}
