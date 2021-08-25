#include "mychartview.h"
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <vector>
#include <QtCharts/QValueAxis>
#include <QLogValueAxis>


  MyChartView::MyChartView(QWidget *parent, std::vector<std::pair<int, double>> _dataVector, double _laserOutput)
      : QGraphicsView(new QGraphicsScene, parent), dataVector(_dataVector), laserOutput(_laserOutput)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    series =new QtCharts::QLineSeries();
    series=buildDataSerie();
    series->setName("");


    chart= new QtCharts::QChart();
    chart->addSeries(series);
    chart->legend()->hide();

    series2 =new QtCharts::QLineSeries();
    series2=buildDataLaserOutput();
    chart->addSeries(series2);
    firstSeriesItem = series->at(0).y();
    kindOfValues="[W/m^2]";
    setAxisX();
    setAxisY(firstSeriesItem);


    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chart->setBackgroundBrush(QColor::fromRgb(240, 240, 240));
    chart->setMinimumSize(440, 300);
    scene()->addItem(chart);
}

QtCharts::QLineSeries* MyChartView::buildDataSerie()
{
    series->clear();
    for (std::vector<std::pair<int, double>>::iterator it = dataVector.begin() ; it != dataVector.end(); ++it)
    {
        std::pair<int, double> myPair=*it;
        series->append(myPair.first, myPair.second);
    }
    return series;
}

QtCharts::QLineSeries* MyChartView::buildDataLaserOutput()
{
    series2->clear();
    for(int i=1; i<=10; i++ )
        series2->append(i, laserOutput);

    return series2;
}

void MyChartView::setAxisY(double _firstSeriesItem)
{
    axisY = new QLogValueAxis();
    axisY->setTitleText(QString::fromStdString("Valori limite " + getKindOfValue()));
    axisY->setLabelFormat("%g");
    axisY->setBase(10.0);

    axisY->setMin(_firstSeriesItem);
    axisY->setMax(_firstSeriesItem*1.0e+09);

    axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    series2->attachAxis(axisY);
}

void MyChartView::setAxisX()
{
    axisX = new QValueAxis();
    axisX->setTitleText("Numeri di scala");
    axisX->setLabelFormat("%i");
    axisX->setMin(1.0);
    axisX->setMax(10);
    axisX->setTickCount(series->count());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    series2->attachAxis(axisX);
}

void MyChartView::setTableSeries(std::vector<std::pair<int, double>> myVector)
{
    dataVector=myVector;
}

void MyChartView::setLaserOutput(double _laserOutput)
{
    laserOutput=_laserOutput;
}

void MyChartView::updateChart(std::string _kindOfValue)
{
    chart->removeAxis(axisY);
    setKindOfValue(_kindOfValue);
    firstSeriesItem = series->at(0).y();
    setAxisY(firstSeriesItem);
}

void MyChartView::setKindOfValue(const std::string _kindOfValues)
{
    kindOfValues=_kindOfValues;
}

std::string MyChartView::getKindOfValue() const
{
    return kindOfValues;
}
