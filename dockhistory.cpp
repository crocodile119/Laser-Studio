#include "dockhistory.h"
#include "ui_dockhistory.h"

DockHistory::DockHistory(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockHistory)
{
    ui->setupUi(this);
    setObjectName(tr("Cronologia comandi"));
}

DockHistory::~DockHistory()
{
    delete ui;
}
