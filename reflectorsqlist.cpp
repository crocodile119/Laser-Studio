#include "reflectorsqlist.h"
#include "ui_reflectorsqlist.h"

ReflectorsQList::ReflectorsQList(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ReflectorsQList)
{
    ui->setupUi(this);
}

ReflectorsQList::~ReflectorsQList()
{
    delete ui;
}

QModelIndex ReflectorsQList::getTreeViewIndex()
{
    return treeViewIndex;
}

void ReflectorsQList::on_treeView_clicked(const QModelIndex &index)
{
    treeViewIndex=index;
}

void ReflectorsQList::on_treeView_doubleClicked(const QModelIndex &index)
{
    treeViewIndex=index;
}
