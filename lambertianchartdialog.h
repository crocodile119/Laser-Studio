#ifndef LAMBERTIANCHARTDIALOG_H
#define LAMBERTIANCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "lambertianchartview.h"
#include "ui_lambertianchartdialog.h"
#include "diffusionhazard.h"
#include "exendedchartview.h"
#include "reflector.h"


class Reflector;


QT_BEGIN_NAMESPACE
namespace Ui { class LambertianChartDialog; }
QT_END_NAMESPACE

class LambertianChartDialog : public QDialog, private Ui::LambertianChartDialog
{
    Q_OBJECT

public:
    LambertianChartDialog(Reflector *reflector, bool thema, QWidget *parent = 0);
    ~LambertianChartDialog();
    Ui::LambertianChartDialog *ui;
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

    void on_pushButton_clicked();

private:
    LambertianChartView *polarChartView;
    ExendedChartView *exendedChartView;
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
    bool thema;
    std::vector<std::pair<double, double>> dataVector;
};
#endif // WETCHARTDIALOG_H
