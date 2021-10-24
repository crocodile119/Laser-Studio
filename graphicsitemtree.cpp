#include "graphicsitemtree.h"
#include "ui_graphicsitemtree.h"

GraphicsItemTree::GraphicsItemTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::GraphicsItemTree)
{
    ui->setupUi(this);
}

GraphicsItemTree::~GraphicsItemTree()
{
    delete ui;
}

QModelIndex GraphicsItemTree::getTreeViewIndex()
{
    return treeViewIndex;
}

void GraphicsItemTree::on_treeView_clicked(const QModelIndex &index)
{
    treeViewIndex=index;
}

void GraphicsItemTree::on_treeView_doubleClicked(const QModelIndex &index)
{
    treeViewIndex=index;
}
