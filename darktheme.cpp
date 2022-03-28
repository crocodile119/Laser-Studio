#include "mainwindow.h"

void MainWindow::setGuiDarkTheme()
{
    if(theme)
    {
        setStyleSheet(tr("QWidget{background-color:#555555;}"
                         "QWidget{color:#f0f0f0;}"
                         "QMenu::item:selected{background-color:#b5922d;}"
                         "QMenu::item:selected{color:#f0f0f0;}"
                         "QComboBox::item:selected{background-color:#b5922d;}"
                         "QComboBox::item:selected{color:#f0f0f0;}"
                         "QMenu::item:checked{background-color:#666666;}"
                         "QMenuBar::item:selected{color:#f0f0f0;}"
                         "QMenuBar::item:selected{background-color:#b5922d;}"
                         "QToolBar{background-color:#555555;}"
                         "QToolBar QToolButton:checked{background-color:#666666;}"
                         "QTextEdit{background-color:#f0f0f0;}"
                         "QGridLayout{color:#f0f0f0;}"
                         "QGraphicsView{background-color:#f0f0f0;}"));

        laserWindow->setStyleSheet(tr("QWidget {background-color: #555555;}\n"
                         "QLabel {background: none;}\n"
                         "QMenu::item:selected{background-color:#b5922d;}"
                         "QMenu::item:selected{color:#f0f0f0;}"
                         "QGraphicsView {background-color:#f0f0f0;}"));
        laserWindow->getView()->slider()->setStyleSheet(tr(
                          "QSlider::handle:vertical {background: #e0e0e0}\n"
                          "QSlider::sub-page:vertical{background: #555555; border: 1px solid black}"
                          "QSlider::add-page:vertical{background: #b5922d}"));


        laserWindow->myDockControls->setDarkChartTheme(true);
        laserWindow->myDockEffects->ui->tCA_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCB_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCC_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCE_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tT1_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tT2_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tLimitingApertureLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tLimitingApertureMeanLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tGammaLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tNoteLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tRadiationLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tSkinDamageLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tEyeDamageLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tDeltaLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tTminLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->label_3->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->label_4->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_1st_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
         laserWindow->myDockSkin->ui->tEMP_2nd_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->conditions_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tFormulaSkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tNSHDLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tminEMP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tMeanIrradianceSkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tPulseNumberSkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tFormulaSkinMP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tPowerErgSkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tMeanPowerSkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_MP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->conditions_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_1st_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_2nd_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_3rd_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tFormulaLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tNOHDLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tMeanIrradianceLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tExposureTimeLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_mean_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPowerErgLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tCP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tminEMP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tOD_FilterLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tMeanPowerLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPulseNumberLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tFormulaMP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tThermalEMP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tCountingLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_MP_Label->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tOD_MeanFilterLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPulseNumberThLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tScaleNumberLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tNumberOfPulseLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tLaserOutputLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tCoefficient_kiLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tTimeBaseLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tCoefficient_kLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tn_maxLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tScaleNumberDLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tMeanPowerLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tDLaserOutputLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tDTimeBaseLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->divergenceControl->setDarkColor(true);
        laserWindow->myDockControls->ui->powerErgControl->setDarkColor(true);
        laserWindow->myDockControls->ui->pulseControl->setDarkColor(true);
        laserWindow->myDockControls->ui->beamDiameterControl->setDarkColor(true);
        laserWindow->myDockControls->ui->prfControl->setDarkColor(true);
        laserWindow->myDockControls->ui->wavelengthScrollBar->setDarkColor(true);
        laserWindow->myDockControls->ui->T_SkinControl->setDarkColor(true);
        laserWindow->myDockControls->ui->teControl->setDarkColor(true);
     }
    else
    {
        setStyleSheet(tr(""));
        laserWindow->setStyleSheet(tr(""));
        laserWindow->getView()->slider()->setStyleSheet(tr(
                        "QSlider::handle:vertical{background: #e0e0e0}\n"
                        "QSlider::sub-page:vertical{background: #d0d0d0; border: 1px solid grey}"
                        "QSlider::add-page:vertical{background: #00c800}"));
        laserWindow->myDockControls->setDarkChartTheme(false);
        laserWindow->myDockEffects->ui->tCA_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCB_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCC_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tCE_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tT1_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tT2_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tLimitingApertureLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tLimitingApertureMeanLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tGammaLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tNoteLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tRadiationLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tSkinDamageLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tEyeDamageLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tDeltaLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockEffects->ui->tTminLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->label_3->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->label_4->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_1st_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
         laserWindow->myDockSkin->ui->tEMP_2nd_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->conditions_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tFormulaSkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tNSHDLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tminEMP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tMeanIrradianceSkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tPulseNumberSkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tFormulaSkinMP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tPowerErgSkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tMeanPowerSkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockSkin->ui->tEMP_MP_SkinLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->conditions_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_1st_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_2nd_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_3rd_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tFormulaLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tNOHDLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tMeanIrradianceLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tExposureTimeLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_mean_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPowerErgLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tCP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tminEMP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tOD_FilterLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tMeanPowerLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPulseNumberLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tFormulaMP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tThermalEMP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tCountingLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tEMP_MP_Label->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tOD_MeanFilterLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockResults->ui->tPulseNumberThLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tScaleNumberLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tNumberOfPulseLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tLaserOutputLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tCoefficient_kiLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tTimeBaseLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tCoefficient_kLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tn_maxLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tScaleNumberDLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tMeanPowerLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tDLaserOutputLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockGoggle->ui->tDTimeBaseLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
        "color: #fafafa;"
        "border: 0px solid grey;"
        "border-radius: 8px;"
        "padding: 3px;"
        "margin-right: 10px;}"));
        laserWindow->myDockControls->ui->divergenceControl->setDarkColor(false);
        laserWindow->myDockControls->ui->powerErgControl->setDarkColor(false);
        laserWindow->myDockControls->ui->pulseControl->setDarkColor(false);
        laserWindow->myDockControls->ui->beamDiameterControl->setDarkColor(false);
        laserWindow->myDockControls->ui->prfControl->setDarkColor(false);
        laserWindow->myDockControls->ui->wavelengthScrollBar->setDarkColor(false);
        laserWindow->myDockControls->ui->T_SkinControl->setDarkColor(false);
        laserWindow->myDockControls->ui->teControl->setDarkColor(false);
    }
}

