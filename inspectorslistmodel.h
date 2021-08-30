#ifndef INSPECTORSLISTMODEL_H
#define INSPECTORSLISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "beaminspector.h"

class InspectorsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    InspectorsListModel(const QList <std::pair<BeamInspector *, int>>  &_myBeamInspectors, QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    int addElement(BeamInspector&);
    void deleteElement(int&);
    void setElementList(QList <std::pair<BeamInspector *, int>>);
    void myDataHasChanged();
    void deleteList();

private:
    QStringList stringList;
    QList <std::pair<BeamInspector *, int>> myBeamInspectors;
    QIcon inspectorIcon;
};

#endif // INSPECTORSLISTMODEL_H
