#ifndef ENVIRONMENTLISTMODEL_H
#define ENVIRONMENTLISTMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "labroom.h"

class EnvironmentListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    EnvironmentListModel(const QList<LabRoom*> &_labroomList, bool _state, QObject *parent);

    const static int STANDARD_VISIBILITY;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    void addDescriptor(LabRoom& _labroom);
    void deleteElement(int&);
    void deleteList();
/*
    void addElement(LaserPoint);

    void setElementList(LaserPoint);
*/
    //void createList(QStringList &);
    //void setLaserList();
    void myDataHasChanged();
    void setState(bool _state);
    bool getState();
    void setMeteoVisibility(const int &);

private:

    QList<LabRoom*> labroomList;
    bool state;
    int meteoVisibility;
};

#endif // ENVIRONMENTLISTMODEL_H
