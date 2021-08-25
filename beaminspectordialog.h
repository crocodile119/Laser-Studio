#ifndef BEAMINSPECTORDIALOG_H
#define BEAMINSPECTORDIALOG_H

#include "ui_beaminspectordialog.h"
#include "beaminspector.h"

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

private slots:


private:
    BeamInspector *beamInspector;
};

#endif
