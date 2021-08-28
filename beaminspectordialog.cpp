#include <QtGui>
#include <QColorDialog>
#include "beaminspectordialog.h"

BeamInspectorDialog::BeamInspectorDialog(BeamInspector *_beamInspector, QWidget *parent)
    : QDialog(parent), ui(new Ui::BeamInspectorDialog), beamInspector(_beamInspector)
{
    ui->setupUi(this);
    setUpChart();
    ui->xSpinBox->setValue(beamInspector->x());
    ui->ySpinBox->setValue(beamInspector->y());
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

    return dataApparentSourceDiameterVector;
}
BeamInspectorDialog::~BeamInspectorDialog()
{
    delete ui;
}

void BeamInspectorDialog::on_xSpinBox_valueChanged(double arg1)
{
    beamInspector->setPos(QPointF(arg1, beamInspector->pos().y()));
    beamInspector->inspectorUpdate();
    setUpBeamInspector();
    setUpChart();
}

void BeamInspectorDialog::on_ySpinBox_valueChanged(double arg1)
{
    beamInspector->setPos(QPointF(beamInspector->pos().x(), arg1));
    beamInspector->inspectorUpdate();
    setUpBeamInspector();
    setUpChart();
}

void BeamInspectorDialog::setUpBeamInspector()
{
    ui->apertureDistanceLabel->setText(QString::number(beamInspector->getInspectorDistance()));
    ui->rayleighDistanceLabel->setText(QString::number(beamInspector->getRayleighDistance()));
    ui->qualityFactorLabel->setText(QString::number(beamInspector->getQualityFactor()));
    ui->spotDiameterLabel->setText(QString::number(beamInspector->getSpotDiameter()));
    ui->descriptionTextEdit->setPlainText(beamInspector->getDescription());
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
}

void BeamInspectorDialog::setUpChart()
{
    BeamInspectorChart *beamInspectorChart=new BeamInspectorChart(this, beamInspector->getRayleighDistance(),
                           beamDiameterVector(), apparentSourceDiameterVector());

    QtCharts::QChart* beamChartObject;
    beamChartObject=beamInspectorChart->getBeamChartObject();
    beamChartObject->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    beamChartObject->setMinimumSize(240, 130);

    ChartView *beamChartView = new ChartView(beamChartObject);
    beamChartView ->setObjectName("beamChartView");
    ui->chartGridLayout->addWidget(beamChartView, 0, 1);
    beamChartView->setToolTip("+: Zoom in "
                              "\n-: Zoom out"
                              "\nbarra spaziatrice: reimposta lo zoom"
                              "\nfreccia a destra : muovi a destra"
                              "\nfreccia a sinistra : muovi a sinistra"
                              "\nfreccia in alto: muovi in alto"
                              "\nfreccia in basso: muovi in basso");
}

void BeamInspectorDialog::on_toolButton_clicked()
{
    QWidget* pWid=static_cast<QWidget *>(parentWidget());
    ChartView* beamChartView=pWid->findChild<ChartView *>("beamChartView");
    beamChartView->chart()->zoomOut();
}

