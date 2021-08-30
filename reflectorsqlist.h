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
    QModelIndex getModelIndex();
    QModelIndex getModelBinocularIndex();
    QModelIndex getModelBeamInspectorIndex();
    Ui::ReflectorsQList *ui;

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_binocularListView_clicked(const QModelIndex &index);
    void on_binocularListView_doubleClicked(const QModelIndex &index);
    void on_inspectorListView_doubleClicked(const QModelIndex &index);
    void on_inspectorListView_clicked(const QModelIndex &index);
    void on_laserListView_clicked(const QModelIndex &index);
    void on_laserListView_doubleClicked(const QModelIndex &index);
    void on_environmentListView_clicked(const QModelIndex &index);
    void on_environmentListView_doubleClicked(const QModelIndex &index);

private:
    QModelIndex index;
    QModelIndex binocularIndex;
    QModelIndex inspectorIndex;
    QModelIndex laserIndex;
    QModelIndex environmentIndex;
};

#endif // REFLECTORSQLIST_H
