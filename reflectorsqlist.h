#ifndef REFLECTORSQLIST_H
#define REFLECTORSQLIST_H

#include <QDockWidget>
#include <QModelIndex>

namespace Ui {
class ReflectorsQList;
}

class ReflectorsQList : public QDockWidget
{
    Q_OBJECT

public:
    explicit ReflectorsQList(QWidget *parent = nullptr);
    ~ReflectorsQList();
    QModelIndex getTreeViewIndex();
    Ui::ReflectorsQList *ui;

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    QModelIndex treeViewIndex;
};

#endif // REFLECTORSQLIST_H
