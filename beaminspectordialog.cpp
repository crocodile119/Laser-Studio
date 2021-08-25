#include <QtGui>
#include <QColorDialog>

#include "beaminspectordialog.h"

BeamInspectorDialog::BeamInspectorDialog(BeamInspector *_beamInspector, QWidget *parent)
    : QDialog(parent), ui(new Ui::BeamInspectorDialog), beamInspector(_beamInspector)
{
    ui->setupUi(this);

    ui->xSpinBox->setValue(beamInspector->x());
    ui->ySpinBox->setValue(beamInspector->y());
}

BeamInspectorDialog::~BeamInspectorDialog()
{
    delete ui;
}
