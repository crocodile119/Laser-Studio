#ifndef FRESNELCHARTDIALOG_H
#define FRESNELCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "fresnelchartview.h"

#include "reflector.h"


QT_BEGIN_NAMESPACE
namespace Ui { class FresnelChartDialog; }
QT_END_NAMESPACE

class FresnelChartDialog : public QDialog
{
    Q_OBJECT

public:
    FresnelChartDialog(Reflector *reflector, QWidget *parent = nullptr);
    ~FresnelChartDialog();

private slots:

    double getDistance() const;
    double getDNRO() const;
    double getRefraction()const;

    void setLabelDistance();
    void setLabelDNRO();
    void setLabelRefraction();
    void setLabelHazard();
    void setLabelPositioning();
    void setRefraction(const double);

private:
    Ui::FresnelChartDialog *ui;
    FresnelChartView *polarChartView;    
    double distance;
    double dnro;
    double refraction;
    double divergence;
    double maxElement;
    int positioning;
    int correctPositioning;
    double positioningElement;
    Reflector *reflector;
    std::vector<std::pair<double, double>> dataVector;

};
#endif // FRESNELCHART_H
