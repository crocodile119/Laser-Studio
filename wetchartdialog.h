#ifndef WETCHARTDIALOG_H
#define WETCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "wetchartview.h"
#include "reflector.h"


class Reflector;


QT_BEGIN_NAMESPACE
namespace Ui { class WetChartDialog; }
QT_END_NAMESPACE

class WetChartDialog : public QDialog
{
    Q_OBJECT

public:
    WetChartDialog(Reflector *reflector, QWidget *parent = 0);
    ~WetChartDialog();

private slots:
    double getDistance() const;
    double getDNRO() const;
    double getDivergence()const;
    double getRefraction()const;

    void setLabelDistance();
    void setLabelDNRO();
    void setLabelDivergence();
    void setLabelRefraction();
    void setLabelHazard();
    void setLabelPositioning();
    void setRefraction(const double);

private:
    Ui::WetChartDialog *ui;
    WetChartView *polarChartView;
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
#endif // WETCHARTDIALOG_H
