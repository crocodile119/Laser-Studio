#ifndef BINOCULARSLISTMODEL_H
#define BINOCULARSLISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "binocular.h"

class BinocularsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    BinocularsListModel(const QList <std::pair<Binocular *, int>>  &myBinoculars, QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    void addElement(Binocular&);
    void deleteElement(int&);
    void setElementList(QList <std::pair<Binocular *, int>>);
    void myDataHasChanged();
    void deleteList();

private:
    QStringList stringList;
    QList <std::pair<Binocular *, int>> myBinoculars;
    QIcon binocularIcon;
};

#endif // BINOCULARSLISTMODEL_H
