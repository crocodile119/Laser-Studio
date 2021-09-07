#include <QtGui>
#include <QColorDialog>
#include "beaminspectordialog.h"

BeamInspectorDialog::BeamInspectorDialog(BeamInspector *_beamInspector, QWidget *parent)
    : QDialog(parent), beamInspector(_beamInspector)
{
    setUpWidget();

    if(isRetinalHazard())
        setUpChart();

    setUpBeamInspector();
    xSpinBox->setValue(beamInspector->pos().x());
    ySpinBox->setValue(beamInspector->pos().y());

    connect(xSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_xSpinBox_valueChanged(double)));
    connect(ySpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_ySpinBox_valueChanged(double)));
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
    apertureDistanceLabel->setText(QString::number(beamInspector->getInspectorDistance()));
    rayleighDistanceLabel->setText(QString::number(beamInspector->getRayleighDistance()));
    qualityFactorLabel->setText(QString::number(beamInspector->getQualityFactor()));
    spotDiameterLabel->setText(QString::number(beamInspector->getSpotDiameter()));
    descriptionTextEdit->setPlainText(beamInspector->getDescription());
    double farFieldRatio=beamInspector->getRayleighDistance()/beamInspector->getInspectorDistance();

    if(beamInspector->isFarField())
        farFieldLabel->setText(tr("Campo lontano"));
    else
        farFieldLabel->setText(tr("Campo vicino"));

    inspectorPhaseLabel->setText(QString::number(beamInspector->getLinkInspectorPhase()));

    if(isRetinalHazard())
    {
        farFieldRatioLabel->setText(QString::number(farFieldRatio));
        eyeFocusLengthLabel->setText(QString::number(beamInspector->get_fm()));
        curvatureRadiusLabel->setText(QString::number(beamInspector->getCurvatureRadius()));
        double d_r_micron=beamInspector->get_d_r()*1e+06;
        retinalDiameterLabel->setText(QString::number(d_r_micron));
        alphaLabel->setText(QString::number(beamInspector->get_alpha_r()));
        double d_r_FarField_micron=beamInspector->get_d_r_FarField()*1e+06;
        ffRetinalDiameterLabel->setText(QString::number(d_r_FarField_micron));
        errorLabel->setText(QString::number(beamInspector->getPercentError())+" %");
        CE_Label->setText(QString::number(beamInspector->getCE()));
        apparentSourceLabel->setText(QString::number(beamInspector->get_d_s()));
        EMP_Label->setVisible(!beamInspector->isFarField());
        tEMP_Label->setVisible(!beamInspector->isFarField());

        Formula_Label->setVisible(beamInspector->isLongExposure());
        tFormula_Label->setVisible(beamInspector->isLongExposure());
        T1_Label->setVisible(beamInspector->isLongExposure());
        tT1_Label->setVisible(beamInspector->isLongExposure());
        T2_Label->setVisible(beamInspector->isLongExposure());
        tT2_Label->setVisible(beamInspector->isLongExposure());
        Notes_Label->setVisible(beamInspector->isLongExposure());
        tNotes_Label->setVisible(beamInspector->isLongExposure());
        MainMultiPulseEffect_Label->setVisible((beamInspector->isLongExposure())&&
                    (beamInspector->getLaserOperation()==DockControls::operation::MULTI_PULSE));
        tMainMultiPulseEffect_Label->setVisible((beamInspector->isLongExposure())&&
                    (beamInspector->getLaserOperation()==DockControls::operation::MULTI_PULSE));

        if(!beamInspector->isFarField())
        {
            if(beamInspector->isLongExposure())
            {
                EMP_Label->setText(QString::number(beamInspector->getLongExposureEMP(), 'e', 2));
                tEMP_Label->setText(QString::fromStdString(beamInspector->getLongExposureEMP_Sort()+beamInspector->getLongExposureEMP_Unit()));
                Formula_Label->setText(QString::fromStdString(beamInspector->getFormula()));
                T1_Label->setText(QString::number(beamInspector->getT1()));
                T2_Label->setText(QString::number(beamInspector->getT2()));
                Notes_Label->setText(QString::fromStdString(beamInspector->getNotes()));

                if(beamInspector->getLaserOperation()==DockControls::operation::MULTI_PULSE)
                    MainMultiPulseEffect_Label->setText(QString::fromStdString(beamInspector->getMainEffect()));
            }
            else
            {
                EMP_Label->setText(QString::number(beamInspector->getAugmentedEMP(), 'e', 2));
                tEMP_Label->setText(QString::fromStdString(beamInspector->getEMP_Sort()+beamInspector->getEMP_Unit()));
                Formula_Label->setText("");
                tFormula_Label->setText("");
                T1_Label->setText("");
                T2_Label->setText("");
                Notes_Label->setText("");
                MainMultiPulseEffect_Label->setText("");
            }
        }
    }
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
    chartGridLayout->addWidget(beamChartView, 0, 1);
    beamChartView->setToolTip("+: Zoom in "
                              "\n-: Zoom out"
                              "\nbarra spaziatrice: reimposta lo zoom"
                              "\nfreccia a destra : muovi a destra"
                              "\nfreccia a sinistra : muovi a sinistra"
                              "\nfreccia in alto: muovi in alto"
                              "\nfreccia in basso: muovi in basso");
}

bool BeamInspectorDialog::isRetinalHazard()
{
    double wavelength=beamInspector->getWavelength();
    bool retinalHazard=(wavelength>=400.0)&&(wavelength<=1400.0);
    return retinalHazard;
}

BeamInspector* BeamInspectorDialog::getBeamInspector()const
{
    return beamInspector;
}

void BeamInspectorDialog::setUpWidget()
{
    setObjectName(QString::fromUtf8("beamInspectorDialog"));

    setMaximumSize(QSize(16777215, 16777215));

    QFont font;
    font.setPointSize(8);
    setFont(font);

    QFont boldFont;
    boldFont.setPointSize(8);
    boldFont.setBold(true);
    setFont(boldFont);

    QString nameStyle="QLabel {background-color: #00c800}\n"
                      "QLabel {color: #fafafa}\n"
                      "QLabel {border: 0px solid grey}\n"
                      "QLabel {border-radius: 8px}\n"
                      "QLabel {padding: 3px}\n"
                      "QLabel {margin-left: 10px}";

    QString valueStyle="QLabel {background-color: #fafafa}\n"
                       "QLabel {color: #000000}\n"
                       "QLabel {border: 0px solid grey}\n"
                       "QLabel {border-radius: 8px}\n"
                       "QLabel {padding: 3px}\n"
                       "QLabel {margin-right: 10px}";

    QString textEditStyle="QTextEdit {background-color: #fafafa}\n"
                          "QTextEdit {color: #000000}\n"
                          "QTextEdit {border: 0px solid grey}\n"
                          "QTextEdit {border-radius: 8px}\n"
                          "QTextEdit {padding: 3px}\n"
                          "QTextEdit {margin-left: 10px}";

    setStyleSheet(QString::fromUtf8("QDoubleSpinBox{background: none}\n"
                                    "QCheckBox{background: none}\n"
                                    "QFrame{border solid 1px #f0f0f0}"));
        
    dockGridLayout = new QGridLayout(this);
    dockGridLayout->setObjectName(QString::fromUtf8("dockGridLayout"));
    chartGridLayout = new QGridLayout();
    chartGridLayout->setObjectName(QString::fromUtf8("chartGridLayout"));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    if(isRetinalHazard())
        dockGridLayout->addWidget(buttonBox, 4, 1, 1, 1);
    else
        dockGridLayout->addWidget(buttonBox, 2, 1, 1, 1);

    beamPointGroupBox = new QGroupBox(this);
    beamPointGroupBox->setObjectName(QString::fromUtf8("beamPointGroupBox"));
    beamPointGroupBox->setMaximumSize(QSize(247, 16777215));
    beamPointGroupBox->setFont(font);
    beamPointGridLayout = new QGridLayout(beamPointGroupBox);
    beamPointGridLayout->setObjectName(QString::fromUtf8("beamPointGridLayout"));

    tXLabel = new QLabel(beamPointGroupBox);
    tXLabel->setObjectName(QString::fromUtf8("tXLabel"));
    tXLabel->setFont(boldFont);
    tXLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tXLabel, 0, 0, 1, 1);

    xSpinBox = new QDoubleSpinBox(beamPointGroupBox);
    xSpinBox->setObjectName(QString::fromUtf8("xSpinBox"));
    xSpinBox->setFont(font);
    xSpinBox->setStyleSheet(QString::fromUtf8("QDoubleSpinBox{color: #000000}\n"));
    xSpinBox->setMinimum(-1000000.000000000000000);
    xSpinBox->setMaximum(1000000.000000000000000);
    xSpinBox->setSingleStep(0.1);
    xSpinBox->setValue(0.0);
    beamPointGridLayout->addWidget(xSpinBox, 0, 1, 1, 1);

    tYLabel = new QLabel(beamPointGroupBox);
    tYLabel->setObjectName(QString::fromUtf8("tYLabel"));
    tYLabel->setFont(boldFont);
    tYLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tYLabel, 1, 0, 1, 1);

    ySpinBox = new QDoubleSpinBox(beamPointGroupBox);
    ySpinBox->setObjectName(QString::fromUtf8("ySpinBox"));
    ySpinBox->setFont(font);
    ySpinBox->setStyleSheet(QString::fromUtf8("QDoubleSpinBox{color: #000000}\n"));
    ySpinBox->setMinimum(-1000000.000000000000000);
    ySpinBox->setMaximum(1000000.000000000000000);
    ySpinBox->setSingleStep(0.1);
    ySpinBox->setValue(0.0);
    beamPointGridLayout->addWidget(ySpinBox, 1, 1, 1, 1);

    tApertureDistanceLabel = new QLabel(beamPointGroupBox);
    tApertureDistanceLabel->setObjectName(QString::fromUtf8("tApertureDistanceLabel"));
    tApertureDistanceLabel->setFont(boldFont);
    tApertureDistanceLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tApertureDistanceLabel, 2, 0, 1, 1);

    apertureDistanceLabel = new QLabel(beamPointGroupBox);
    apertureDistanceLabel->setObjectName(QString::fromUtf8("apertureDistanceLabel"));
    apertureDistanceLabel->setFont(font);
    apertureDistanceLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(apertureDistanceLabel, 2, 1, 1, 1);

    tRayleighDistanceLabel = new QLabel(beamPointGroupBox);
    tRayleighDistanceLabel->setObjectName(QString::fromUtf8("tRayleighDistanceLabel"));
    tRayleighDistanceLabel->setFont(boldFont);
    tRayleighDistanceLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tRayleighDistanceLabel, 3, 0, 1, 1);

    rayleighDistanceLabel = new QLabel(beamPointGroupBox);
    rayleighDistanceLabel->setObjectName(QString::fromUtf8("rayleighDistanceLabel"));
    rayleighDistanceLabel->setFont(font);
    rayleighDistanceLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(rayleighDistanceLabel, 3, 1, 1, 1);

    tSpotDiameterLabel = new QLabel(beamPointGroupBox);
    tSpotDiameterLabel->setObjectName(QString::fromUtf8("tSpotDiameterLabel"));
    tSpotDiameterLabel->setFont(boldFont);
    tSpotDiameterLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tSpotDiameterLabel, 4, 0, 1, 1);

    spotDiameterLabel = new QLabel(beamPointGroupBox);
    spotDiameterLabel->setObjectName(QString::fromUtf8("spotDiameterLabel"));
    spotDiameterLabel->setFont(font);
    spotDiameterLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(spotDiameterLabel, 4, 1, 1, 1);

    tQualityFactorLabel = new QLabel(beamPointGroupBox);
    tQualityFactorLabel->setObjectName(QString::fromUtf8("tQualityFactorLabel"));
    tQualityFactorLabel->setFont(boldFont);
    tQualityFactorLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tQualityFactorLabel, 5, 0, 1, 1);

    qualityFactorLabel = new QLabel(beamPointGroupBox);
    qualityFactorLabel->setObjectName(QString::fromUtf8("qualityFactorLabel"));
    qualityFactorLabel->setFont(font);
    qualityFactorLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(qualityFactorLabel, 5, 1, 1, 1);

    tInspectorPhaseLabel = new QLabel(beamPointGroupBox);
    tInspectorPhaseLabel->setObjectName(QString::fromUtf8("tInspectorPhaseLabel"));
    tInspectorPhaseLabel->setFont(boldFont);
    tInspectorPhaseLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tInspectorPhaseLabel, 6, 0, 1, 1);

    inspectorPhaseLabel = new QLabel(beamPointGroupBox);
    inspectorPhaseLabel->setObjectName(QString::fromUtf8("inspectorPhaseLabel"));
    inspectorPhaseLabel->setFont(font);
    inspectorPhaseLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(inspectorPhaseLabel, 6, 1, 1, 1);

    tFarFieldLabel = new QLabel(beamPointGroupBox);
    tFarFieldLabel->setObjectName(QString::fromUtf8("tFarFieldLabel"));
    tFarFieldLabel->setFont(boldFont);
    tFarFieldLabel->setStyleSheet(nameStyle);
    beamPointGridLayout->addWidget(tFarFieldLabel, 7, 0, 1, 1);

    farFieldLabel = new QLabel(beamPointGroupBox);
    farFieldLabel->setObjectName(QString::fromUtf8("farFieldLabel"));
    farFieldLabel->setFont(font);
    farFieldLabel->setStyleSheet(valueStyle);
    beamPointGridLayout->addWidget(farFieldLabel, 7, 1, 1, 1);

    tDescriptionTextEdit = new QLabel(beamPointGroupBox);
    tDescriptionTextEdit->setObjectName(QString::fromUtf8("tDescriptionTextEdit"));
    tDescriptionTextEdit->setMaximumSize(QSize(16777215, 36));
    tDescriptionTextEdit->setFont(boldFont);
    tDescriptionTextEdit->setStyleSheet(nameStyle);
    tDescriptionTextEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    if(isRetinalHazard())
        beamPointGridLayout->addWidget(tDescriptionTextEdit, 8, 0, 1, 2);

    descriptionTextEdit = new QTextEdit(beamPointGroupBox);
    descriptionTextEdit->setObjectName(QString::fromUtf8("descriptionTextEdit"));
    descriptionTextEdit->setFont(font);
    descriptionTextEdit->setStyleSheet(textEditStyle);

    if(isRetinalHazard())
        beamPointGridLayout->addWidget(descriptionTextEdit, 10, 0, 1, 2);

    if(isRetinalHazard())
        dockGridLayout->addWidget(beamPointGroupBox, 0, 0, 1, 1);
    else
        dockGridLayout->addWidget(beamPointGroupBox, 0, 0, 2, 1);

    if(!isRetinalHazard())
    {
        dockGridLayout->addWidget(tDescriptionTextEdit, 0, 1, 1, 1);
        dockGridLayout->addWidget(descriptionTextEdit, 1, 1, 1, 1);
    }

    if(isRetinalHazard())
    {
        dockGridLayout->addLayout(chartGridLayout, 0, 1, 1, 1);

        retinalGroupBox = new QGroupBox(this);
        retinalGroupBox->setObjectName(QString::fromUtf8("retinalGroupBox"));
        retinalGroupBox->setFont(font);
        retinalGridLayout = new QGridLayout(retinalGroupBox);
        retinalGridLayout->setObjectName(QString::fromUtf8("retinalGridLayout"));

        tFarFieldRatioLabel = new QLabel(retinalGroupBox);
        tFarFieldRatioLabel->setObjectName(QString::fromUtf8("tFarFieldRatioLabel"));
        tFarFieldRatioLabel->setFont(boldFont);
        tFarFieldRatioLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tFarFieldRatioLabel, 0, 0, 1, 1);

        farFieldRatioLabel = new QLabel(retinalGroupBox);
        farFieldRatioLabel->setObjectName(QString::fromUtf8("farFieldRatioLabel"));
        farFieldRatioLabel->setFont(font);
        farFieldRatioLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(farFieldRatioLabel, 0, 1, 1, 1);

        tEyeFocusLengthLabel = new QLabel(retinalGroupBox);
        tEyeFocusLengthLabel->setObjectName(QString::fromUtf8("tEyeFocusLengthLabel"));
        tEyeFocusLengthLabel->setFont(boldFont);
        tEyeFocusLengthLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tEyeFocusLengthLabel, 0, 2, 1, 1);

        eyeFocusLengthLabel = new QLabel(retinalGroupBox);
        eyeFocusLengthLabel->setObjectName(QString::fromUtf8("eyeFocusLengthLabel"));
        eyeFocusLengthLabel->setFont(font);
        eyeFocusLengthLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(eyeFocusLengthLabel, 0, 3, 1, 1);

        tRetinalDiameterLabel = new QLabel(retinalGroupBox);
        tRetinalDiameterLabel->setObjectName(QString::fromUtf8("tRetinalDiameterLabel"));
        tRetinalDiameterLabel->setFont(boldFont);
        tRetinalDiameterLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tRetinalDiameterLabel, 0, 4, 1, 1);

        retinalDiameterLabel = new QLabel(retinalGroupBox);
        retinalDiameterLabel->setObjectName(QString::fromUtf8("retinalDiameterLabel"));
        retinalDiameterLabel->setFont(font);
        retinalDiameterLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(retinalDiameterLabel, 0, 5, 1, 1);

        tRetinalDiameterLabel_2 = new QLabel(retinalGroupBox);
        tRetinalDiameterLabel_2->setObjectName(QString::fromUtf8("tRetinalDiameterLabel_2"));
        tRetinalDiameterLabel_2->setFont(boldFont);
        tRetinalDiameterLabel_2->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tRetinalDiameterLabel_2, 0, 6, 1, 1);

        ffRetinalDiameterLabel = new QLabel(retinalGroupBox);
        ffRetinalDiameterLabel->setObjectName(QString::fromUtf8("ffRetinalDiameterLabel"));
        ffRetinalDiameterLabel->setFont(font);
        ffRetinalDiameterLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(ffRetinalDiameterLabel, 0, 7, 1, 1);

        tErrorLabel = new QLabel(retinalGroupBox);
        tErrorLabel->setObjectName(QString::fromUtf8("tErrorLabel"));
        tErrorLabel->setFont(boldFont);
        tErrorLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tErrorLabel, 0, 8, 1, 1);

        errorLabel = new QLabel(retinalGroupBox);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setFont(font);
        errorLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(errorLabel, 0, 9, 1, 1);

        tCurvatureRadiusLabel = new QLabel(retinalGroupBox);
        tCurvatureRadiusLabel->setObjectName(QString::fromUtf8("tCurvatureRadiusLabel"));
        tCurvatureRadiusLabel->setFont(boldFont);
        tCurvatureRadiusLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tCurvatureRadiusLabel, 1, 0, 1, 1);

        curvatureRadiusLabel = new QLabel(retinalGroupBox);
        curvatureRadiusLabel->setObjectName(QString::fromUtf8("curvatureRadiusLabel"));
        curvatureRadiusLabel->setFont(font);
        curvatureRadiusLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(curvatureRadiusLabel, 1, 1, 1, 1);

        tApparentSourceLabel = new QLabel(retinalGroupBox);
        tApparentSourceLabel->setObjectName(QString::fromUtf8("tApparentSourceLabel"));
        tApparentSourceLabel->setFont(boldFont);
        tApparentSourceLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tApparentSourceLabel, 1, 2, 1, 1);

        apparentSourceLabel = new QLabel(retinalGroupBox);
        apparentSourceLabel->setObjectName(QString::fromUtf8("apparentSourceLabel"));
        apparentSourceLabel->setFont(font);
        apparentSourceLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(apparentSourceLabel, 1, 3, 1, 1);

        tAlphaLabel = new QLabel(retinalGroupBox);
        tAlphaLabel->setObjectName(QString::fromUtf8("tAlphaLabel"));
        tAlphaLabel->setFont(boldFont);
        tAlphaLabel->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tAlphaLabel, 1, 4, 1, 1);

        alphaLabel = new QLabel(retinalGroupBox);
        alphaLabel->setObjectName(QString::fromUtf8("alphaLabel"));
        alphaLabel->setFont(font);
        alphaLabel->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(alphaLabel, 1, 5, 1, 1);

        tCE_Label = new QLabel(retinalGroupBox);
        tCE_Label->setObjectName(QString::fromUtf8("tCE_Label"));
        tCE_Label->setFont(boldFont);
        tCE_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tCE_Label, 1, 6, 1, 1);

        CE_Label = new QLabel(retinalGroupBox);
        CE_Label->setObjectName(QString::fromUtf8("CE_Label"));
        CE_Label->setFont(font);
        CE_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(CE_Label, 1, 7, 1, 1);

        tEMP_Label = new QLabel(retinalGroupBox);
        tEMP_Label->setObjectName(QString::fromUtf8("tEMP_Label"));
        tEMP_Label->setFont(boldFont);
        tEMP_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tEMP_Label, 1, 8, 1, 1);

        EMP_Label = new QLabel(retinalGroupBox);
        EMP_Label->setObjectName(QString::fromUtf8("EMP_Label"));
        EMP_Label->setFont(font);
        EMP_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(EMP_Label, 1, 9, 1, 1);

        tFormula_Label = new QLabel(retinalGroupBox);
        tFormula_Label->setObjectName(QString::fromUtf8("tFormula_Label"));
        tFormula_Label->setFont(boldFont);
        tFormula_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tFormula_Label, 2, 0, 1, 1);

        Formula_Label = new QLabel(retinalGroupBox);
        Formula_Label->setObjectName(QString::fromUtf8("Formula_Label"));
        Formula_Label->setFont(font);
        Formula_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(Formula_Label, 2, 1, 1, 1);

        tT1_Label = new QLabel(retinalGroupBox);
        tT1_Label->setObjectName(QString::fromUtf8("tT1_Label"));
        tT1_Label->setFont(boldFont);
        tT1_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tT1_Label, 2, 2, 1, 1);

        T1_Label = new QLabel(retinalGroupBox);
        T1_Label->setObjectName(QString::fromUtf8("T1_Label"));
        T1_Label->setFont(font);
        T1_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(T1_Label, 2, 3, 1, 1);

        tT2_Label = new QLabel(retinalGroupBox);
        tT2_Label->setObjectName(QString::fromUtf8("tT2_Label"));
        tT2_Label->setFont(boldFont);
        tT2_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tT2_Label, 2, 4, 1, 1);

        T2_Label = new QLabel(retinalGroupBox);
        T2_Label->setObjectName(QString::fromUtf8("T2_Label"));
        T2_Label->setFont(font);
        T2_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(T2_Label, 2, 5, 1, 1);

        tNotes_Label = new QLabel(retinalGroupBox);
        tNotes_Label->setObjectName(QString::fromUtf8("tNotes_Label"));
        tNotes_Label->setFont(boldFont);
        tNotes_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tNotes_Label, 2, 6, 1, 1);

        Notes_Label = new QLabel(retinalGroupBox);
        Notes_Label->setObjectName(QString::fromUtf8("Notes_Label"));
        Notes_Label->setFont(font);
        Notes_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(Notes_Label, 2, 7, 1, 1);

        tMainMultiPulseEffect_Label = new QLabel(retinalGroupBox);
        tMainMultiPulseEffect_Label->setObjectName(QString::fromUtf8("tMainMultiPulseEffect_Label"));
        tMainMultiPulseEffect_Label->setFont(boldFont);
        tMainMultiPulseEffect_Label->setStyleSheet(nameStyle);
        retinalGridLayout->addWidget(tMainMultiPulseEffect_Label, 2, 8, 1, 1);

        MainMultiPulseEffect_Label = new QLabel(retinalGroupBox);
        MainMultiPulseEffect_Label->setObjectName(QString::fromUtf8("MainMultiPulseEffect_Label"));
        MainMultiPulseEffect_Label->setFont(font);
        MainMultiPulseEffect_Label->setStyleSheet(valueStyle);
        retinalGridLayout->addWidget(MainMultiPulseEffect_Label, 2, 9, 1, 1);

        dockGridLayout->addWidget(retinalGroupBox, 3, 0, 1, 2);
}

        QWidget::setTabOrder(xSpinBox, ySpinBox);

        QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

        setWindowTitle("Segnaposto di ispezione");
        beamPointGroupBox->setTitle("Fascio nel punto");
        tSpotDiameterLabel->setText("d <sub>b</sub> [mm]");
        tDescriptionTextEdit->setText("Descrizione");
        tRayleighDistanceLabel->setText("z<sub>R</sub> [m]");
        tApertureDistanceLabel->setText("L [m]");
        tInspectorPhaseLabel->setText("&phi;<sub>Inspector</sub>[rad]");
        tQualityFactorLabel->setText("M<sup>2</sup>");
        tXLabel->setText("X:");
        tYLabel->setText("Y:");
        tFarFieldLabel->setText("Propagazione");

        if(isRetinalHazard())
        {
            retinalGroupBox->setTitle("Dati relativi al danno retinico e alla sorgente apparente");
            tFarFieldRatioLabel->setText("z <sub>r</sub>/L");
            tErrorLabel->setText("errore");
            tEyeFocusLengthLabel->setText("f <sub>m</sub> [m]");
            tRetinalDiameterLabel->setText("d <sub>r</sub> [&mu;m]");
            tRetinalDiameterLabel_2->setText("d <sub>ff</sub> [&mu;m]");
            tCurvatureRadiusLabel->setText("r<sub>s</sub> [m]");
            tApparentSourceLabel->setText("d<sub>s</sub>[mm]");
            tAlphaLabel->setText("<p>&alpha;[mrad]</p>");
            tCE_Label->setText("C<sub>E</sub>");
            if(beamInspector->isLongExposure())
            {
                tFormula_Label->setText("Esposizione lunga");
                tT1_Label->setText("T<sub>1</sub>");
                tT2_Label->setText("T<sub>2</sub>");
                tNotes_Label->setText("note");
                if(beamInspector->getLaserOperation()==DockControls::operation::MULTI_PULSE)
                    tMainMultiPulseEffect_Label->setText("Effetto prevalente");
            }
        }

        resize(sizeHint());
}
