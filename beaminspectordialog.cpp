#include <QtGui>
#include <QColorDialog>

#include "beaminspectordialog.h"

BeamInspectorDialog::BeamInspectorDialog(BeamInspector *_beamInspector, QWidget *parent)
    : QDialog(parent), ui(new Ui::BeamInspectorDialog), beamInspector(_beamInspector)
{
    ui->setupUi(this);
    ui->xSpinBox->setValue(beamInspector->x());
    ui->ySpinBox->setValue(beamInspector->y());
    ui->apertureDistanceLabel->setText(QString::number(beamInspector->getInspectorDistance()));
    ui->rayleighDistanceLabel->setText(QString::number(beamInspector->getRayleighDistance()));
    ui->qualityFactorLabel->setText(QString::number(beamInspector->getQualityFactor()));
    ui->descriptionTextEdit->setPlainText(beamInspector->getDescription());
    ui->spotDiameterLabel->setText(QString::number(beamInspector->getSpotDiameter()));
    double farFieldRatio=beamInspector->getRayleighDistance()/beamInspector->getInspectorDistance();
    ui->farFieldRatioLabel->setText(QString::number(farFieldRatio));
    ui->eyeFocusLengthLabel->setText(QString::number(beamInspector->get_fm()));
    ui->curvatureRadiusLabel->setText(QString::number(beamInspector->getCurvatureRadius()));
    double d_r_micron=beamInspector->get_d_r()*1e+06;
    ui->retinalDiameterLabel->setText(QString::number(d_r_micron));
    ui->alphaLabel->setText(QString::number(beamInspector->get_alpha_r()));
    double d_r_FarField_micron=beamInspector->get_d_r_FarField()*1e+06;
    ui->ffRetinalDiameterLabel->setText(QString::number(d_r_FarField_micron));
    ui->errorLabel->setText(QString::number(beamInspector->getPercentError())+" %");
    ui->CE_Label->setText(QString::number(beamInspector->getCE()));

    if(beamInspector->isFarField())
        ui->farFieldLabel->setText(tr("Campo lontano"));
    else
        ui->farFieldLabel->setText(tr("Campo vicino"));
}

BeamInspectorDialog::~BeamInspectorDialog()
{
    delete ui;
}
