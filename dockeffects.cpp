#include "dockeffects.h"
#include "ui_dockeffects.h"

DockEffects::DockEffects(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockEffects)
{
    ui->setupUi(this);
}

DockEffects::~DockEffects()
{
    delete ui;
}
