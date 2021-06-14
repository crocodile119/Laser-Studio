#include "wetchartview.h"

const qreal WetChartView::angularMin = 0;
const qreal WetChartView::angularMax = 360;

const qreal WetChartView::radialMin = 0;


WetChartView::WetChartView(QWidget *parent, std::vector<std::pair<double, double>> _dataVector,
                           double _radialMax, int _correctPositioning)
    : QGraphicsView(new QGraphicsScene, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    radialMax=_radialMax;
    dataVector=_dataVector;
    setDistance(getDistance());
    correctPositioning=_correctPositioning;

    series = new QLineSeries();
    series->setName("Distanza di rischio riflessione su superficie bagnata [m]");
    series=buildDataSerie();

    chart = new QPolarChart();
    QPen pen (QColor::fromRgb(0, 216, 0), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    series->setPen(pen);

    positioningSeries= new QLineSeries();
    positioningSeries=buildPositioningSeries();
    positioningSeries->setName(tr("Posizionamento"));

    chart->addSeries(series);

    QPen pen2 (Qt::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    positioningSeries->setPen(pen2);
    chart->addSeries(positioningSeries);

    setRadialAxis();
    setAngularAxis();
    chart->setBackgroundBrush(QColor::fromRgb(240, 240, 240));
    chart->setMinimumSize(640, 480);

    scene()->addItem(chart);
}


QtCharts::QLineSeries* WetChartView::buildDataSerie()
{
    series->clear();
    for (std::vector<std::pair<double, double>>::iterator it = dataVector.begin() ; it != dataVector.end(); ++it)
    {
        std::pair<double, double> myPair=*it;
        series->append(myPair.first, myPair.second);
    }
    return series;
}

double WetChartView::getDistance()const
{
    return distance;
}

void WetChartView::setDistance(const double& _distance)
{
    distance=_distance;
}

void WetChartView::setRadialAxis()
{
    radialAxis = new QValueAxis();
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");
    radialAxis->setRange(radialMin, radialMax);
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);
    positioningSeries->attachAxis(radialAxis);
}

void WetChartView::setAngularAxis()
{
    angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    angularAxis->setRange(angularMin, angularMax);
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);
    positioningSeries->attachAxis(angularAxis);
}

void WetChartView::updateChart(double _maxValue)
{
    chart->removeAxis(radialAxis);
    chart->removeAxis(angularAxis);
    setRadialMax(_maxValue);
    setRadialAxis();
    setAngularAxis();
}

void WetChartView::setTableSeries(std::vector<std::pair<double, double>> myVector)
{
    dataVector=myVector;
}

void WetChartView::setRadialMax(const double _maxElement)
{
    if(radialMax<_maxElement)
        radialMax=_maxElement*1.2;
    else if(radialMax>1.5*_maxElement)
        radialMax=_maxElement/1.2;
    else
        radialMax=_maxElement*1.1;
}

double WetChartView::getRadialMax()const
{
    return radialMax;
}

QtCharts::QLineSeries* WetChartView::buildPositioningSeries()
{
    positioningSeries->clear();
    positioningSeries->append(0.0, 0.0);

    if(correctPositioning<=0)
        correctPositioning=correctPositioning+360;

    positioningSeries->append(correctPositioning, radialMax);
    return positioningSeries;
}
