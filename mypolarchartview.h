#ifndef MYPOLARCHARTVIEW_H
#define MYPOLARCHARTVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QPolarChart>
#include <QtCharts/QValueAxis>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MyPolarChartView: public QGraphicsView
{
    Q_OBJECT

public:
    MyPolarChartView(QWidget *parent = 0);
    static const qreal radialMin;
    static const qreal radialMax;
    static const qreal angularMin;
    static const qreal angularMax;

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    QPolarChart *m_chart;
    QValueAxis *angularAxis;
    QValueAxis *radialAxis;
};

#endif
