#include <QtCore>
#include <QPointF>
#include <vector>
#include <utility>
#include <QBrush>
#include <QFont>
#include "scalenumbersmodelview.h"

const int ScaleNumbersModelView::SCALE_NUMBER=5;
const int ScaleNumbersModelView::ROW_NUMBERS=10;
const int ScaleNumbersModelView::COLUMN_NUMBERS=2;

ScaleNumbersModelView::ScaleNumbersModelView(QObject *parent):QAbstractTableModel(parent),
    scaleNumber(SCALE_NUMBER)
{
}

int ScaleNumbersModelView::rowCount(const QModelIndex & /*parent*/) const
{
   return ROW_NUMBERS;
}

int ScaleNumbersModelView::columnCount(const QModelIndex & /*parent*/) const
{
    return COLUMN_NUMBERS;
}

QVariant ScaleNumbersModelView::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignVCenter);

    else if (role == Qt::DisplayRole)
    {
        QString tableElement;
        if(index.column()==0)
            tableElement= QString("LB%1").arg(pointAt(index.row()).first);
        else if(index.column()==1)
            tableElement = QString::number(pointAt(index.row()).second);

        return tableElement;
    }

    else if (role == Qt::BackgroundRole)
    {
        if (index.row()==scaleNumber-1)
            return QBrush(QColor::fromRgb(230,230,230));
    }

    else if (role == Qt::FontRole)
    {
        if (index.row()==scaleNumber-1)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        if (index.row()==scaleNumber-1)
            return QBrush(Qt::black);
    }
    return QVariant();
}

void ScaleNumbersModelView::setTableList(vector<pair<int, double>> myVector)
{
    beginResetModel();
    dataVector=myVector;
    endResetModel();
}

pair<int, double> ScaleNumbersModelView::pointAt(int offset) const
{
    pair<int, double> myPoint=make_pair(dataVector.at(offset).first, dataVector.at(offset).second);
    //qDebug() << myPoint;
    return myPoint;
}

QVariant ScaleNumbersModelView::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section)
        {
            case 0:
                return QString("Numero di scala");
            case 1:
                return QString("Valore limite");
        }
    }
    return QVariant();
}

void ScaleNumbersModelView::setScaleNumber(const int & _scaleNumber)
{
    beginResetModel();
    scaleNumber=_scaleNumber;
    endResetModel();
}

int ScaleNumbersModelView::getScaleNumber()const
{
    return scaleNumber;
}
