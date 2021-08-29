#ifndef BEAMINSPECTORDIALOG_H
#define BEAMINSPECTORDIALOG_H

#include "ui_beaminspectordialog.h"
#include "beaminspector.h"
#include "chartview.h"
#include "beaminspectorchart.h"
#include <vector>
#include <utility>
#include <QApplication>

namespace Ui {
class BeamInspectorDialog;
}

class BeamInspectorDialog : public QDialog, private Ui::BeamInspectorDialog
{
    Q_OBJECT

public:
    BeamInspectorDialog(BeamInspector *_beamInspector, QWidget *parent = 0);
    ~BeamInspectorDialog();
    Ui::BeamInspectorDialog *ui;
    void setUpBeamInspector();
    void setUpChart();
    std::vector<std::pair <double,double> > beamDiameterVector();
    std::vector<std::pair <double,double> > apparentSourceDiameterVector();

private slots:
    void on_xSpinBox_valueChanged(double arg1);
    void on_ySpinBox_valueChanged(double arg1);

private:
    BeamInspector *beamInspector;
    std::vector<std::pair<double, double>> beamVector;
    std::vector<std::pair<double, double>> retinalVector;
};

#endif
