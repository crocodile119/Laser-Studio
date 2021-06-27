#include "reflectorslistmodel.h"

ReflectorsListModel::ReflectorsListModel(const QList <pair<Reflector *, int>>  &_myReflectors, QObject *parent)
    : QAbstractListModel(parent), myReflectors(_myReflectors)
{

}

int ReflectorsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myReflectors.count();
}

QVariant ReflectorsListModel::data(const QModelIndex &index, int role) const
{
    QString reflectorData;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= myReflectors.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        reflectorData=myReflectors.at(index.row()).first->getStringDetails();
        return  reflectorData;
    }
    else if (role == Qt::ForegroundRole)
    {
        QColor textColor;
        textColor=myReflectors.at(index.row()).first->textColor();

        return textColor;
    }

    else if (role == Qt::DecorationRole)
    {
        QIcon myIcon;

        if(myReflectors.at(index.row()).first->getReflectorKind()==WET_TARGET)
           myIcon=QIcon(":/images/wet1.png");

        else if(myReflectors.at(index.row()).first->getReflectorKind()==LAMBERTIAN_TARGET)
           myIcon=QIcon(":/images/lambertian1.png");

        else if(myReflectors.at(index.row()).first->getReflectorKind()==GLASS_TARGET)
           myIcon=QIcon(":/images/window1.png");

        else if(myReflectors.at(index.row()).first->getReflectorKind()==MIRROR_TARGET)
           myIcon=QIcon(":/images/mirror1.png");

        return myIcon;
    }

     return QVariant();
}

QVariant ReflectorsListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

int ReflectorsListModel::addElement(Reflector& _reflector)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);
    int index=rowCount();
    myReflectors.append(make_pair(&_reflector, rowCount()));
    endInsertRows();

    return index;
}

void ReflectorsListModel::deleteElement(int& index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void ReflectorsListModel::setElementList(QList <pair<Reflector *, int>> _myReflectors)
{
    beginResetModel();
    myReflectors.clear();
    myReflectors=_myReflectors;
    endResetModel();
}

void ReflectorsListModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, rowCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}

void ReflectorsListModel::deleteList()
{
    beginResetModel();
    myReflectors.clear();
    endResetModel();
}
