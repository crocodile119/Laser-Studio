#include "laserlistmodel.h"
#include <QStandardItem>

LaserListModel::LaserListModel(const QList<LaserPoint*> &_laserPointList, QObject *parent)
    : QAbstractListModel(parent), laserPointList(_laserPointList),  laserIcon(":/images/laserpix.png")
{

}

int LaserListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return laserPointList.count();
}

QVariant LaserListModel::data(const QModelIndex &index, int role) const
{
    QString laserDescriptor;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= laserPointList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        laserDescriptor=laserPointList.at(index.row())->getStringPosition()+
                        "\nPostazione: " + laserPointList.at(index.row())->getLaserInstallation();

        return  laserDescriptor;
    }

    if (role == Qt::DecorationRole)
    {
        return laserIcon;
    }

     return QVariant();
}

QVariant LaserListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

void LaserListModel::addDescriptor(LaserPoint& _laserpoint)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);
    laserPointList.append(&_laserpoint);
    endInsertRows();
}

void LaserListModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, rowCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}

void LaserListModel::deleteList()
{
    beginResetModel();
    laserPointList.clear();
    endResetModel();
}
