#include "dockgoggle.h"
#include "ui_dockgoggle.h"

DockGoggle::DockGoggle(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockGoggle)
{
    ui->setupUi(this);
}

DockGoggle::~DockGoggle()
{
    delete ui;
}
