#include "dockeffects.h"
#include "ui_dockeffects.h"

DockEffects::DockEffects(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockEffects)
{
    ui->setupUi(this);
    ui->tDeltaLabel->setText("δ");
    setObjectName(tr("Parametri"));
}

DockEffects::~DockEffects()
{
    delete ui;
}
