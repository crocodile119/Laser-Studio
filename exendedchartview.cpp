#include "exendedchartview.h"

ExendedChartView::ExendedChartView(QWidget *parent, double _hazardDistance)
                            : QGraphicsView(new QGraphicsScene, parent), hazardDistance(_hazardDistance)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    series = new QLineSeries();
    series->setName("Distanza di rischio riflessione estesa su superficie lambertiana [m]");
    series=buildDataSerie();

    chart = new QPolarChart();
    QPen pen (QColor::fromRgb(0, 216, 0), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    series->setPen(pen);
    chart->addSeries(series);

    setRadialAxis();
    setAngularAxis();

    chart->setMinimumSize(520, 390);

    scene()->addItem(chart);
}


QtCharts::QLineSeries* ExendedChartView::buildDataSerie()
{
    series->clear();

    for(int i=0; i<10; i++)
    {
        int degree=i*10;
        series->append(degree, hazardDistance);
    }
    series->append(90, 0);
    series->append(270, 0);

    for(int i=0; i<10; i++)
    {
        int degree=270+i*10;
        series->append(degree, hazardDistance);
    }

   return series;
}

void ExendedChartView::setRadialAxis()
{
    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%.1e");
    radialAxis->setRange(0, 1.2*hazardDistance);
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);
}

void ExendedChartView::setAngularAxis()
{
    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    angularAxis->setRange(0, 360);
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);
}

void ExendedChartView::setChartBackgroundBrush(QColor color)
{
    chart->setBackgroundBrush(color);
}
