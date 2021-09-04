#include "binocularslistmodel.h"

BinocularsListModel::BinocularsListModel(const QList <std::pair<Binocular *, int>>  &_myBinoculars, QObject *parent)
    : QAbstractListModel(parent), myBinoculars(_myBinoculars)
{

}

int BinocularsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myBinoculars.count();
}

QVariant BinocularsListModel::data(const QModelIndex &index, int role) const
{
    QString binocularData;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= myBinoculars.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        binocularData=myBinoculars.at(index.row()).first->getStringPosition();
        return  binocularData;
    }

    else if (role == Qt::DecorationRole)
    {
        QIcon myIcon;
           myIcon=QIcon(":/images/binocularpix.png");

        return myIcon;
    }

     return QVariant();
}

QVariant BinocularsListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

int BinocularsListModel::addElement(Binocular& _binocular)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);
    int index = rowCount();
    myBinoculars.append(std::make_pair(&_binocular, rowCount()));
    endInsertRows();

    return index;
}

void BinocularsListModel::deleteElement(int& index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void BinocularsListModel::setElementList(QList <std::pair<Binocular *, int>> _myBinoculars)
{
    beginResetModel();
    myBinoculars.clear();
    myBinoculars=_myBinoculars;
    endResetModel();
}

void BinocularsListModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, rowCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}

void BinocularsListModel::deleteList()
{
    beginResetModel();
    myBinoculars.clear();
    endResetModel();
}
