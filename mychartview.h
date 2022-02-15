#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QLogValueAxis>
#include <vector>
#include <utility>
#include <string>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE


QT_CHARTS_USE_NAMESPACE

class MyChartView: public QGraphicsView
{
    Q_OBJECT

public:
    MyChartView(QWidget *parent, std::vector<std::pair<int, double>>, double);
    QtCharts::QLineSeries* buildDataSerie();
    QtCharts::QLineSeries* buildDataLaserOutput();
    void buildChart(double _min, double _max);
    void setTableSeries(std::vector<std::pair<int, double>>);
    void setLaserOutput(double);
    void updateChart(std::string);
    void setAxisX();
    void setAxisY(double);
    void setKindOfValue(const std::string);
    std::string getKindOfValue()const;
    void setViewTheme(bool);

private:

    QtCharts::QChart *chart;
    std::vector<std::pair<int, double>> dataVector;
    QtCharts::QLineSeries *series;
    QtCharts::QLineSeries *series2;
    double laserOutput;
    QValueAxis *axisX;
    QLogValueAxis *axisY;
    double firstSeriesItem;
    std::string kindOfValues;
};

#endif // MYCHARTVIEW_H
