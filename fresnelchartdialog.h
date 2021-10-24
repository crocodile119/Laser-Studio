#ifndef FRESNELCHARTDIALOG_H
#define FRESNELCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "ui_fresnelchartdialog.h"
#include "fresnelchartview.h"

#include "reflector.h"


QT_BEGIN_NAMESPACE
namespace Ui { class FresnelChartDialog; }
QT_END_NAMESPACE

class FresnelChartDialog : public QDialog, private Ui::FresnelChartDialog
{
    Q_OBJECT

public:
    FresnelChartDialog(Reflector *reflector, bool _thema, QWidget *parent = nullptr);
    ~FresnelChartDialog();
    Ui::FresnelChartDialog *ui;

private slots:

    double getDistance() const;
    double getDNRO() const;
    double getRefraction()const;

    void setLabelDistance();
    void setLabelDNRO();
    void setLabelRefraction();
    void setLabelHazard();
    void setLabelCorrectPositioning();
    void setRefraction(const double);

    void on_pushButton_clicked();

private:
    FresnelChartView *polarChartView;    
    double distance;
    double dnro;
    double refraction;
    double divergence;
    double maxElement;
    int positioning;
    double correctPositioning;
    double positioningElement;
    Reflector *reflector;
    bool thema;
    std::vector<std::pair<double, double>> dataVector;

};
#endif // FRESNELCHART_H
