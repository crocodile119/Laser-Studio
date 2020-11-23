#ifndef SCALENUMBERSMODELVIEW_H
#define SCALENUMBERSMODELVIEW_H

#include <QAbstractTableModel>
#include <QPointF>
#include <vector>
#include <utility>

using namespace std;
class ScaleNumbersModelView : public QAbstractTableModel
{
    Q_OBJECT
public:
    ScaleNumbersModelView(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void setTableList(vector<pair<int, double>>);
    QVariant headerData(int, Qt::Orientation, int) const override;
    void setScaleNumber(const int);
    int getScaleNumber()const;
private:
    int scaleNumber;
    pair<int, double> pointAt(int) const;
    vector<std::pair<int, double>> dataVector;
};

#endif // SCALENUMBERSMODELVIEW_H
