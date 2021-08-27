#include <QtGui>
#include <QColorDialog>
#include "beaminspectordialog.h"

BeamInspectorDialog::BeamInspectorDialog(BeamInspector *_beamInspector, QPointF _laserPos,
                                         QWidget *parent)
    : QDialog(parent), ui(new Ui::BeamInspectorDialog), beamInspector(_beamInspector), laserPos(_laserPos)
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
    ui->apparentSourceLabel->setText(QString::number(beamInspector->get_d_s()));

    if(beamInspector->isFarField())
        ui->farFieldLabel->setText(tr("Campo lontano"));
    else
        ui->farFieldLabel->setText(tr("Campo vicino"));

    ui->inspectorPhaseLabel->setText(QString::number(beamInspector->getLinkInspectorPhase()));

    rotation=beamInspector->getLinkInspectorPhase();
    beamInspectorChart = new BeamInspectorChart(this, beamInspector->getRayleighDistance(),
                                                beamDiameterVector(), apparentSourceDiameterVector(),
                                                laserPos, rotation);

    QtCharts::QChart* beamChartObject=beamInspectorChart->getBeamChartObject();

    beamChartObject->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    beamChartObject->setMinimumSize(240, 130);
    beamChartObject->setAnimationOptions(QChart::SeriesAnimations);

    ChartView *beamChartView = new ChartView(beamChartObject);
    ui->chartGridLayout->addWidget(beamChartView, 0, 1);

}

std::vector<std::pair <double,double> > BeamInspectorDialog::beamDiameterVector()
{
    std::vector< std::pair <double,double> > dataBeamDiameterVector;
    double beamDiameter=beamInspector->getBeamDiameter();

    dataBeamDiameterVector.push_back( std::make_pair(0.0, beamDiameter/2));
    dataBeamDiameterVector.push_back( std::make_pair(0.0, 0.0));
    dataBeamDiameterVector.push_back( std::make_pair(0.0, -beamDiameter/2));

    return dataBeamDiameterVector;
}

std::vector<std::pair <double,double> > BeamInspectorDialog::apparentSourceDiameterVector()
{
    std::vector< std::pair <double,double> > dataApparentSourceDiameterVector;
    double apparentSourceDiameter=beamInspector->get_d_s();
    double apparentSourceAbscissa=pow(beamInspector->getRayleighDistance(),2)/beamInspector->getInspectorDistance();
    dataApparentSourceDiameterVector.push_back( std::make_pair(-apparentSourceAbscissa, apparentSourceDiameter/2));
    dataApparentSourceDiameterVector.push_back( std::make_pair(-apparentSourceAbscissa, 0.0));
    dataApparentSourceDiameterVector.push_back( std::make_pair(-apparentSourceAbscissa, -apparentSourceDiameter/2));

    std::vector<std::pair <double,double> >::const_iterator constIterator; // const_iterator
    // display vector elements using const_iterator
    for ( constIterator = dataApparentSourceDiameterVector.begin();
          constIterator != dataApparentSourceDiameterVector.end(); ++constIterator )
    {
         qDebug() << "Coordinata x: " << constIterator->first << " Coordinata y: " <<  constIterator->second;
    }

    return dataApparentSourceDiameterVector;
}
BeamInspectorDialog::~BeamInspectorDialog()
{
    delete ui;
}
