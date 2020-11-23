#ifndef WETCHARTVIEW_H
#define WETCHARTVIEW_H

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

class WetChartView: public QGraphicsView
{
    Q_OBJECT

public:
    WetChartView(QWidget *parent, std::vector<std::pair<double, double>>, double, int);
    QtCharts::QLineSeries* buildDataSerie();
    QtCharts::QLineSeries* buildPositioningSeries();
    static const qreal radialMin;
    static const qreal angularMin;
    static const qreal angularMax;

    double getDistance()const;
    void setDistance(const double&);
    void setRadialAxis();
    void setAngularAxis();
    void updateChart(double);
    void setRadialMax(const double);
    double getRadialMax()const;
    void setTableSeries(std::vector<std::pair<double, double>>);

private:

    QtCharts::QPolarChart *chart;
    QtCharts::QLineSeries *series;
    QtCharts::QLineSeries *positioningSeries;
    QValueAxis *angularAxis;
    QValueAxis *radialAxis;
    std::vector<std::pair<double, double>> dataVector;
    double distance;
    double radialMax;
    double maxElement;
    double positioning;
    double correctPositioning;
};

#endif // WETCHARTVIEW_H
