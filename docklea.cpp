#include "docklea.h"
#include "ui_docklea.h"

DockLea::DockLea(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockLea)
{
    ui->setupUi(this);
    setObjectName(tr("Classificazione"));
}

DockLea::~DockLea()
{
    delete ui;
}
