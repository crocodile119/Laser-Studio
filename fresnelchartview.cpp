#include "fresnelchartview.h"

const qreal FresnelChartView::angularMin = 0;
const qreal FresnelChartView::angularMax = 360;

const qreal FresnelChartView::radialMin = 0;


  FresnelChartView::FresnelChartView(QWidget *parent, std::vector<std::pair<double, double>> _dataVector,
                                     double _radialMax, int _correctPositioning): QGraphicsView(new QGraphicsScene, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    radialMax=_radialMax;
    dataVector=_dataVector;
    setDistance(getDistance());
    correctPositioning=double(_correctPositioning);

    series = new QLineSeries();
    series->setName("Distanza di rischio riflessione su superficie di vetro [m]");
    series=buildDataSerie();

    positioningSeries= new QLineSeries();
    positioningSeries=buildPositioningSeries();
    positioningSeries->setName(tr("Posizionamento"));

    chart = new QPolarChart();
    QPen pen (QColor::fromRgb(0, 216, 0), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    series->setPen(pen);
    chart->addSeries(series);

    QPen pen2 (Qt::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    positioningSeries->setPen(pen2);
    chart->addSeries(positioningSeries);
    setRadialAxis();
    setAngularAxis();


    chart->setMinimumSize(640, 480);

    scene()->addItem(chart);
}


  QtCharts::QLineSeries* FresnelChartView::buildDataSerie()
  {
         series->clear();
         for (std::vector<std::pair<double, double>>::iterator it = dataVector.begin() ; it != dataVector.end(); ++it)
           {
           std::pair<double, double> myPair=*it;
             series->append(myPair.first, myPair.second);
          }
         return series;
  }

  double FresnelChartView::getDistance()const
  {
      return distance;
  }

  void FresnelChartView::setDistance(const double& _distance)
  {
      distance=_distance;
  }

  void FresnelChartView::setRadialAxis()
  {
      radialAxis = new QValueAxis();
      radialAxis->setTickCount(9);
      radialAxis->setLabelFormat("%d");
      radialAxis->setRange(radialMin, radialMax);
      chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
      series->attachAxis(radialAxis);     
      positioningSeries->attachAxis(radialAxis);
  }

  void FresnelChartView::setAngularAxis()
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

  void FresnelChartView::updateChart(double _maxValue)
  {
      chart->removeAxis(radialAxis);
      chart->removeAxis(angularAxis);
      setRadialMax(_maxValue);
      setRadialAxis();
      setAngularAxis();
  }

  void FresnelChartView::setTableSeries(std::vector<std::pair<double, double>> myVector)
  {
      dataVector=myVector;
  }

  void FresnelChartView::setRadialMax(const double _maxElement)
  {
      if(radialMax<_maxElement)
            radialMax=_maxElement*1.2;
      else
         if(radialMax>1.5*_maxElement)
             radialMax=_maxElement/1.2;
      else
             radialMax=_maxElement*1.1;
  }

double FresnelChartView::getRadialMax()const
{
    return radialMax;
}

QtCharts::QLineSeries* FresnelChartView::buildPositioningSeries()
{
    positioningSeries->clear();
    positioningSeries->append(0.0, 0.0);
    positioningSeries->append(correctPositioning, radialMax);
    return positioningSeries;
}
