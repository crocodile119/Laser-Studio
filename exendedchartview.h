#ifndef EXENDEDCHARTVIEW_H
#define EXENDEDCHARTVIEW_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QPolarChart>
#include <utility>
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QPolarChart;
QT_CHARTS_END_NAMESPACE


QT_CHARTS_USE_NAMESPACE

class ExendedChartView: public QGraphicsView
{
    Q_OBJECT

public:
    ExendedChartView(QWidget *parent, double _hazardDistance);
    QtCharts::QLineSeries* buildDataSerie();

    void setRadialAxis();
    void setAngularAxis();
    void setChartBackgroundBrush(QColor color);

private:

    QtCharts::QPolarChart *chart;
    QtCharts::QLineSeries *series;
    QValueAxis *angularAxis;
    QValueAxis *radialAxis;
    double hazardDistance;
};

#endif // EXENDEDCHARTVIEW_H
