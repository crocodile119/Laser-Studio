#ifndef REFLECTORSLISTMODEL_H
#define REFLECTORSLISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "reflector.h"

class ReflectorsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ReflectorsListModel(const QList <pair<Reflector *, int>>  &myReflectors, QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    void addElement(Reflector&);
    void deleteElement(int&);
    void setElementList(QList <pair<Reflector *, int>>);
    void deleteList();
    void myDataHasChanged();

private:
    QStringList stringList;
    QList <pair<Reflector *, int>> myReflectors;
    QIcon reflectorIcon;
};

#endif // REFLECTORSLISTMODEL_H
