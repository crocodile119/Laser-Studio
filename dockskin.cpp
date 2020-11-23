#include "dockskin.h"
#include "ui_dockskin.h"

DockSkin::DockSkin(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockSkin)
{
    ui->setupUi(this);
}

DockSkin::~DockSkin()
{
    delete ui;
}
