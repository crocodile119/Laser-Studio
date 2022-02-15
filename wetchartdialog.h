#ifndef WETCHARTDIALOG_H
#define WETCHARTDIALOG_H

#include <QWidget>
#include <QDialog>
#include "wetchartview.h"
#include "ui_wetchartdialog.h"
#include "reflector.h"


class Reflector;


QT_BEGIN_NAMESPACE
namespace Ui { class WetChartDialog; }
QT_END_NAMESPACE

class WetChartDialog : public QDialog, private Ui::WetChartDialog
{
    Q_OBJECT

public:
    WetChartDialog(Reflector *reflector, bool _thema, QWidget *parent = 0);
    ~WetChartDialog();
    Ui::WetChartDialog *ui;
    void setStyleSheet();

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
    void setLabelCorrectPositioning();
    void setRefraction(const double);

    void on_pushButton_clicked();

private:
    WetChartView *polarChartView;
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
#endif // WETCHARTDIALOG_H
