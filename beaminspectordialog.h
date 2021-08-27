#ifndef BEAMINSPECTORDIALOG_H
#define BEAMINSPECTORDIALOG_H

#include "ui_beaminspectordialog.h"
#include "beaminspector.h"
#include "chartview.h"
#include "beaminspectorchart.h"
#include <vector>
#include <utility>

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
    std::vector<std::pair <double,double> > beamDiameterVector();
    std::vector<std::pair <double,double> > apparentSourceDiameterVector();

private slots:


private:
    BeamInspector *beamInspector;
    QPointF laserPos;
    double rotation;
    BeamInspectorChart *beamInspectorChart;
    std::vector<std::pair<double, double>> beamVector;
    std::vector<std::pair<double, double>> retinalVector;
};

#endif
