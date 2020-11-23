#ifndef LAMBERTIANCHARTDIALOG_H
#define LAMBERTIANCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "lambertianchartview.h"
#include "diffusionhazard.h"
#include "reflector.h"


class Reflector;


QT_BEGIN_NAMESPACE
namespace Ui { class LambertianChartDialog; }
QT_END_NAMESPACE

class LambertianChartDialog : public QDialog
{
    Q_OBJECT

public:
    LambertianChartDialog(Reflector *reflector, QWidget *parent = 0);
    ~LambertianChartDialog();
    LambertianChartView* getLambertianPointChart();
    QChartView* getLambertianExendedChart();

private slots:
    double getDistance() const;
    double getDNRO() const;
    double getDivergence()const;
    double getRefraction()const;
    void setExendedDiffusionChart();
    void setPointDiffusionChart();
    void setLabelDNRO();
    void setRefraction(const double);
    void setWidgetsVisible(bool);

private:
    Ui::LambertianChartDialog *ui;
    LambertianChartView *polarChartView;
    double distance;
    double dnro;
    double refraction;
    double divergence;
    double maxElement;
    double positioning;
    double correctPositioning;
    double positioningElement;
    double laserBeamDiameter;
    double laserEMP;
    double laserPowerErg;
    QChartView *chartView;
    Reflector *reflector;
    std::vector<std::pair<double, double>> dataVector;
};
#endif // WETCHARTDIALOG_H
