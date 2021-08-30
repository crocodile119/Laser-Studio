#include "inspectorslistmodel.h"
#include <QTextEdit>

InspectorsListModel::InspectorsListModel(const QList <std::pair<BeamInspector *, int>>  &_myBeamInspectors, QObject *parent)
    : QAbstractListModel(parent), myBeamInspectors(_myBeamInspectors)
{

}

int InspectorsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myBeamInspectors.count();
}

QVariant InspectorsListModel::data(const QModelIndex &index, int role) const
{
    QString binocularData;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= myBeamInspectors.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        binocularData=myBeamInspectors.at(index.row()).first->getStringPosition();
        return  binocularData;
    }

    else if (role == Qt::DecorationRole)
    {
        QIcon myIcon;
           myIcon=QIcon(":/images/inspector.png");

        return myIcon;
    }

     return QVariant();
}

QVariant InspectorsListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}

int InspectorsListModel::addElement(BeamInspector& _beamInspector)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);
    int index = rowCount();
    myBeamInspectors.append(std::make_pair(&_beamInspector, rowCount()));
    endInsertRows();

    return index;
}

void InspectorsListModel::deleteElement(int& index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void InspectorsListModel::setElementList(QList <std::pair<BeamInspector *, int>> _myBinoculars)
{
    beginResetModel();
    myBeamInspectors.clear();
    myBeamInspectors=_myBinoculars;
    endResetModel();
}

void InspectorsListModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, rowCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}

void InspectorsListModel::deleteList()
{
    beginResetModel();
    myBeamInspectors.clear();
    endResetModel();
}
