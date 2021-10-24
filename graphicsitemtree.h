#ifndef GRAPHICSITEMTREE_H
#define GRAPHICSITEMTREE_H

#include <QDockWidget>
#include <QModelIndex>

namespace Ui {
class GraphicsItemTree;
}

class GraphicsItemTree : public QDockWidget
{
    Q_OBJECT

public:
    explicit GraphicsItemTree(QWidget *parent = nullptr);
    ~GraphicsItemTree();
    QModelIndex getTreeViewIndex();
    Ui::GraphicsItemTree *ui;

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    QModelIndex treeViewIndex;
};

#endif // GRAPHICSITEMTREE_H
