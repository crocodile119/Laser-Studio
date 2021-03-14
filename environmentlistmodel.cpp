#include "environmentlistmodel.h"
#include <QStandardItem>

const int EnvironmentListModel::STANDARD_VISIBILITY= 23;

EnvironmentListModel::EnvironmentListModel(const QList<LabRoom*> &_labroomList, bool _state, QObject *parent)
    : QAbstractListModel(parent), labroomList(_labroomList), state(_state), meteoVisibility(STANDARD_VISIBILITY)
{

}

int EnvironmentListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return labroomList.count();
}

QVariant EnvironmentListModel::data(const QModelIndex &index, int role) const
{
    QString laserDescriptor;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= labroomList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString meteoVisibilityStr=QString::number(meteoVisibility);

        if(state)
        laserDescriptor=labroomList.at(index.row())->getTextLabel();
        else
        {
         laserDescriptor=QString("Poligono di tiro militare. \nVisibilità meteorologica: %1 km")
            .arg(meteoVisibilityStr);
        }

        return  laserDescriptor;
    }

    if (role == Qt::DecorationRole)
    {
        QIcon environmentIcon;

        if(state)
        environmentIcon=QIcon(":/images/room.png");
            else
        environmentIcon=QIcon(":/images/polygon.png");

        return environmentIcon;
    }

     return QVariant();
}

QVariant EnvironmentListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

void EnvironmentListModel::addDescriptor(LabRoom& _labroom)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);
    labroomList.clear();
    labroomList.append(&_labroom);
    endInsertRows();
}

void EnvironmentListModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, rowCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}

void EnvironmentListModel::deleteElement(int& index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void EnvironmentListModel::deleteList()
{
    beginResetModel();
    labroomList.clear();
    endResetModel();
}

void EnvironmentListModel::setState(bool _state)
{
    state=_state;
}

bool EnvironmentListModel::getState()
{
    return state;
}

void EnvironmentListModel::setMeteoVisibility(const int& _meteoVisibility)
{
    int meteoVisibilityInMeters=_meteoVisibility/1000;

    if(meteoVisibilityInMeters==meteoVisibility)
        return;

    meteoVisibility=meteoVisibilityInMeters;
}
