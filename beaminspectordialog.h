#ifndef BEAMINSPECTORDIALOG_H
#define BEAMINSPECTORDIALOG_H

#include "beaminspector.h"
#include "chartview.h"
#include "beaminspectorchart.h"
#include <vector>
#include <utility>
#include <QDialog>
#include <QApplication>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

class BeamInspectorDialog : public QDialog
{
    Q_OBJECT

public:
    BeamInspectorDialog(BeamInspector *_beamInspector, bool _dark, QWidget *parent = 0);
    ~BeamInspectorDialog();
    void setUpBeamInspector();
    void setUpChart();
    BeamInspector* getBeamInspector()const;
    bool isRetinalHazard();
    std::vector<std::pair <double,double> > beamDiameterVector();
    std::vector<std::pair <double,double> > apparentSourceDiameterVector();
    QTextEdit *descriptionTextEdit;
    QDoubleSpinBox *xSpinBox;
    QDoubleSpinBox *ySpinBox;

private slots:
    void on_xSpinBox_valueChanged(double arg1);
    void on_ySpinBox_valueChanged(double arg1);

private:
    void setUpWidget();
    BeamInspector *beamInspector;
    std::vector<std::pair<double, double>> beamVector;
    std::vector<std::pair<double, double>> retinalVector;
    bool dark;

	//widgets
    QWidget *beamInspectorDialog;
	QGridLayout *dockGridLayout;
    QGridLayout *chartGridLayout;
    QGroupBox *retinalGroupBox;
    QGridLayout *retinalGridLayout;
    QLabel *farFieldRatioLabel;
    QLabel *tFarFieldRatioLabel;
    QLabel *tErrorLabel;
    QLabel *errorLabel;
    QLabel *tEyeFocusLengthLabel;
    QLabel *eyeFocusLengthLabel;
    QLabel *tRetinalDiameterLabel;
    QLabel *retinalDiameterLabel;
    QLabel *tRetinalDiameterLabel_2;
    QLabel *ffRetinalDiameterLabel;
    QLabel *tCurvatureRadiusLabel;
    QLabel *curvatureRadiusLabel;
    QLabel *tApparentSourceLabel;
    QLabel *apparentSourceLabel;
    QLabel *tAlphaLabel;
    QLabel *alphaLabel;
    QLabel *tCE_Label;
    QLabel *CE_Label;
    QLabel *tEMP_Label;
    QLabel *EMP_Label;
    QLabel *tFormula_Label;
    QLabel *Formula_Label;
    QLabel *tT1_Label;
    QLabel *T1_Label;
    QLabel *tT2_Label;
    QLabel *T2_Label;
    QLabel *tNotes_Label;
    QLabel *Notes_Label;
    QLabel *tMainMultiPulseEffect_Label;
    QLabel *MainMultiPulseEffect_Label;

    QDialogButtonBox *buttonBox;
    QGroupBox *beamPointGroupBox;
    QGridLayout *beamPointGridLayout;
    QLabel *apertureDistanceLabel;
    QLabel *tFarFieldLabel;
    QLabel *tApertureDistanceLabel;
    QLabel *tRayleighDistanceLabel;

    QLabel *tSpotDiameterLabel;
    QLabel *tQualityFactorLabel;
    QLabel *tDescriptionTextEdit;
    QLabel *spotDiameterLabel;
    QLabel *tInspectorPhaseLabel;
    QLabel *qualityFactorLabel;
    QLabel *inspectorPhaseLabel;
    QLabel *tXLabel;
    QLabel *tYLabel;
    QLabel *rayleighDistanceLabel;
    QLabel *farFieldLabel;

};

#endif
