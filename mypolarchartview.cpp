#include "mypolarchartview.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>

const qreal MyPolarChartView::angularMin = 0;
const qreal MyPolarChartView::angularMax = 360;
const qreal MyPolarChartView::radialMin = 0;
const qreal MyPolarChartView::radialMax = 1000;

MyPolarChartView::MyPolarChartView(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0)
      //m_tooltip(0)
{
    setDragMode(QGraphicsView::NoDrag);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // chart
    m_chart = new QPolarChart();
    m_chart->setMinimumSize(640, 480);
    m_chart->setTitle("Zona sgombero");
    m_chart->legend()->hide();
    QLineSeries *series = new QLineSeries;
    series->append(60, 1000);
    series->append(0, 0);
    series->append(120, 1000);
    m_chart->addSeries(series);

    radialAxis = new QValueAxis();
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");
    radialAxis->setRange(radialMin, radialMax);
    m_chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);

    angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    angularAxis->setRange(angularMin, angularMax);
    m_chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);

    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("ρ: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("θ: ");

    this->setMouseTracking(true);
}

void MyPolarChartView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
    }
    QGraphicsView::resizeEvent(event);
}

void MyPolarChartView::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("ρ: %1").arg(m_chart->mapToValue(event->pos()).y()));
    m_coordY->setText(QString("θ: %1").arg(m_chart->mapToValue(event->pos()).x()));
    QGraphicsView::mouseMoveEvent(event);
}

