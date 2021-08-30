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

QModelIndex ReflectorsQList::getModelIndex()
{
    return index;
}

QModelIndex ReflectorsQList::getModelBinocularIndex()
{
    return binocularIndex;
}

QModelIndex ReflectorsQList::getModelBeamInspectorIndex()
{
    return inspectorIndex;
}

void ReflectorsQList::on_listView_clicked(const QModelIndex &index)
{
    this->index=index;
}

void ReflectorsQList::on_listView_doubleClicked(const QModelIndex &index)
{
    this->index=index;
}

void ReflectorsQList::on_binocularListView_clicked(const QModelIndex &index)
{
    binocularIndex=index;
}

void ReflectorsQList::on_binocularListView_doubleClicked(const QModelIndex &index)
{
    binocularIndex=index;
}

void ReflectorsQList::on_inspectorListView_doubleClicked(const QModelIndex &index)
{
    inspectorIndex=index;
}

void ReflectorsQList::on_inspectorListView_clicked(const QModelIndex &index)
{
    inspectorIndex=index;
}

void ReflectorsQList::on_laserListView_clicked(const QModelIndex &index)
{
    laserIndex=index;
}

void ReflectorsQList::on_laserListView_doubleClicked(const QModelIndex &index)
{
    laserIndex=index;
}

void ReflectorsQList::on_environmentListView_clicked(const QModelIndex &index)
{
    environmentIndex=index;
}

void ReflectorsQList::on_environmentListView_doubleClicked(const QModelIndex &index)
{
    environmentIndex=index;
}
