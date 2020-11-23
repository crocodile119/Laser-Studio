#ifndef LISTLISTMODEL_H
#define LISTLISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "laserpoint.h"

class LaserListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    LaserListModel(const QList<LaserPoint*> &, QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    void addDescriptor(LaserPoint& _laserpoint);
    void deleteList();
/*
    void addElement(LaserPoint);

    void setElementList(LaserPoint);
*/
    //void createList(QStringList &);
    //void setLaserList();
    void myDataHasChanged();


private:
    QList<LaserPoint*> laserPointList;
    LaserPoint laserpoint;
    QIcon laserIcon;

};

#endif // LISTLISTMODEL_H
