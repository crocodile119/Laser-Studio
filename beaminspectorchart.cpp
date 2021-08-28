#include "beaminspectorchart.h"
#include <vector>
#include <QDebug>
#include <QLegendMarker>

  BeamInspectorChart::BeamInspectorChart(QWidget *parent, double _rayleighDistance,
                                         std::vector<std::pair<double, double>> _beamVector,
                                         std::vector<std::pair<double, double>> _apparentVector)
      :  QWidget(parent), rayleighDistance(_rayleighDistance), beamVector(_beamVector),apparentVector(_apparentVector),
         chart(new Chart()), beamSeries(new QtCharts::QLineSeries())
{
    beamSeries->setName(tr("Diametro del waist"));
    beamSeries ->setColor(QColor::fromRgb(255, 0, 0));
    apparentSeries=new QtCharts::QLineSeries();
    apparentSeries->setName(tr("Diametro della sorgente apparente"));
    apparentSeries ->setColor(QColor::fromRgb(0, 255, 0));

    buildDataSeries();

    chart->addSeries(beamSeries);
    chart->addSeries(apparentSeries);

    setAxisX();
    setAxisY();
}

QtCharts::QLineSeries* BeamInspectorChart::buildDataSerie(std::vector<std::pair<double, double>> dataVector,
                                    QtCharts::QLineSeries *mySerie)
{
    mySerie->clear();
    for (std::vector<std::pair<double, double>>::iterator it = dataVector.begin() ; it != dataVector.end(); ++it)
    {
        std::pair<double, double> myPair=*it;
        double x=myPair.first;
        double y=myPair.second;
        mySerie->append(x,y);
    }
    return mySerie;
}

void BeamInspectorChart::setAxisY()
{
    axisY = new QValueAxis();
    axisY->setTitleText(QString::fromStdString("asse y [m] "));
    axisY->setMin(std::min(beamSeries->at(2).y(), apparentSeries->at(2).y()));
    axisY->setMax(std::max(beamSeries->at(0).y(), apparentSeries->at(0).y()));
    axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    beamSeries->attachAxis(axisY);
    apparentSeries->attachAxis(axisY);
    axisY->show();
}

void BeamInspectorChart::setAxisX()
{
    axisX = new QValueAxis();
    axisX->setTitleText("asse x [m]");
    axisX->setLabelFormat("%g");
    axisX->setMin(std::min(beamSeries->at(0).x(), apparentSeries->at(0).x()));
    axisX->setMax(std::max(beamSeries->at(0).x(), apparentSeries->at(0).x()));

    chart->addAxis(axisX, Qt::AlignBottom);
    beamSeries->attachAxis(axisX);
    apparentSeries->attachAxis(axisX);
}

void BeamInspectorChart::setVectorsForSeries(std::vector<std::pair<double, double>> _beamVector, std::vector<std::pair<double, double>> _apparentVector)
{
    beamVector=_beamVector;
    apparentVector=_apparentVector;
}

void BeamInspectorChart::updateChart()
{
    delete chart;
    chart=new Chart();
    beamSeries=new QtCharts::QLineSeries();
    apparentSeries=new QtCharts::QLineSeries();
    beamSeries->setName(tr("Diametro del waist"));
    beamSeries ->setColor(QColor::fromRgb(255, 0, 0));
    apparentSeries=new QtCharts::QLineSeries();
    apparentSeries->setName(tr("Diametro della sorgente apparente"));
    apparentSeries ->setColor(QColor::fromRgb(0, 255, 0));
    buildDataSeries();
    //chart->removeAxis(axisX);
    //chart->removeAxis(axisY);
    setAxisX();
    setAxisY();
}

void BeamInspectorChart::buildDataSeries()
{
    beamSeries=buildDataSerie(beamVector, beamSeries);
    apparentSeries=buildDataSerie(apparentVector, apparentSeries);
}

QtCharts::QChart *BeamInspectorChart::getBeamChartObject()const
{
    return chart;
}

BeamInspectorChart::~BeamInspectorChart()
{

}
