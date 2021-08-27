#ifndef BEAMINSPECTORCHART_H
#define BEAMINSPECTORCHART_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include "chart.h"

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class BeamInspectorChart: QWidget
{
    Q_OBJECT

public:
    BeamInspectorChart(QWidget *parent, double _rayleighDistance, std::vector<std::pair<double,
                       double>> _beamVector, std::vector<std::pair<double, double> > _apparentVector,
                       QPointF _translation, double _rotation);
    QtCharts::QLineSeries* buildDataSerie(std::vector<std::pair<double, double> > dataVector, double rotation, QPointF translation, QtCharts::QLineSeries *mySerie);
    void buildChart(double _min, double _max);
    void setVectorsForSeries(std::vector<std::pair<double, double> > _beamVector, std::vector<std::pair<double, double> > _apparentVector);
    void updateChart();
    void setAxisX();
    void setAxisY();
    void buildDataSeries();
    QChart *getBeamChartObject()const;

private:

    Chart *chart;
    double rayleighDistance;
    std::vector<std::pair<double, double>> beamVector;
    std::vector<std::pair<double, double>> apparentVector;
    QtCharts::QLineSeries *beamSeries;
    QtCharts::QLineSeries *apparentSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    double firstSeriesItem;
    double lastSeriesItem;
    std::string kindOfValues;
    QPointF translation;
    double rotation;
};

#endif // BEAMINSPECTORCHART_H
