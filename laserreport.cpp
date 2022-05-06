#include "laserreport.h"
#include <QFileDialog>
#include <QTextDocument>
#include <fstream>

LaserReport::LaserReport(CentralWidget* _laserWindow, LaserPoint* _laserpoint, typeOfReport _myTypeOfReport):laserWindow(_laserWindow),
    laserpoint(_laserpoint),myTypeOfReport(_myTypeOfReport)
{
}

QTextDocument* LaserReport::buildReportDocument()
{
    textDocument=new QTextDocument;
    textDocument->setDocumentMargin(10);
    textDocument->setDocumentMargin(10);

    qDebug()<<"Le dimensioni del documento: " <<textDocument->pageSize();

    QImage mySceneImage = QImage("./sceneImg.png");

    QUrl Uri=QUrl("mydata://sceneImg.png");

    QTextImageFormat imageSceneFormat;
    imageSceneFormat.setName(Uri.toString());
    imageSceneFormat.setQuality(800);

    textDocument->addResource(QTextDocument::ImageResource,
        Uri, QVariant(mySceneImage));

    textDocument->setDefaultStyleSheet("h1, h2, h3, h4"
                    "{font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;}"
                    "table, th, td {font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                    "font-size: medium;"
                    "border-width: 1px;"
                    "border-color: #dddddd;"
                    "border-collapse: collapse;"
                    "text-align: left;}"
                    "th {"
                    "padding-top: 12px;"
                    "padding-bottom: 12px;"
                    "text-align: left;"
                    "background-color: #00c800;"
                    "color: #fafafa;}"

                    "td, th {padding: 8px;}"

                    "p {font-size: medium;}");

    adjustWidth();

    firstPageReport();
    laserAssessmentResults();
    classifierDetails();
    classifierResults();

    QTextCursor cursor(textDocument);
    myCursor=cursor;
    myCursor.insertHtml(htmlInstallationDescription());
    myCursor.insertHtml(htmlLaserInstallation());

    if(laserWindow->getAtmEffectsBool())
        myCursor.insertHtml(htmlMeteo());

    myCursor.insertImage(imageSceneFormat);
    myCursor.insertHtml(htmlInputData(kindOfLaser()));
    myCursor.insertHtml(htmlEyeExposition());
    myCursor.insertHtml(htmlSkinExposition());
    myCursor.insertHtml(htmlEffects());

    myCursor.insertHtml(htmlGoggleAssessment());
    myCursor.insertHtml(htmlGoggleDetails());
    QImage myGoggleImage = QImage("./chartViewImg.png");

    QUrl UriGoggle=QUrl("mydata://chartViewImg.png");

    QTextImageFormat imageGoggleFormat;
    imageGoggleFormat.setName(UriGoggle.toString());
    imageGoggleFormat.setQuality(800);

    textDocument->addResource(QTextDocument::ImageResource,
        UriGoggle, QVariant(myGoggleImage));
    myCursor.insertImage(imageGoggleFormat);

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
    {
        myCursor.insertHtml(htmlDGoggleDetails());
        QImage myDGoggleImage = QImage("./dChartViewImg.png");

        QUrl UriDGoggle=QUrl("mydata://dChartViewImg.png");

        QTextImageFormat imageDGoggleFormat;
        imageDGoggleFormat.setName(UriDGoggle.toString());
        imageDGoggleFormat.setQuality(800);

        textDocument->addResource(QTextDocument::ImageResource,
        UriDGoggle, QVariant(myDGoggleImage));

    myCursor.insertImage(imageDGoggleFormat);
    }

    myCursor.insertHtml(htmlClassifier());
    buidReflectorsDocumentPart();
    buidInspectorsDocumentPart();
    myCursor.insertHtml(htmlFootprints());
    myCursor.insertHtml(htmlBinoculars());
    myCursor.insertHtml(htmlSafetySigns());

    myCursor.insertHtml(htmlSymbols());
    return textDocument;
}

void LaserReport::adjustWidth()
{
    if(myTypeOfReport==PDF)
    correction="\"650\"";
    else
    if(myTypeOfReport==ODF)
    correction="\480\"";
}

void LaserReport::laserAssessmentResults()
{
    input.clear();
    output.clear();
    goggle.clear();
    effects.clear();
    skin.clear();

    QString powerErgStr= laserWindow->myDockControls->ui->powerErgControl->getTitle()+ "= " + QString::number(laserWindow->myDockControls->ui->powerErgControl->getScientificNumber(), 'e', 2);
    QString wavelengthStr= "&lambda; [nm]= " + QString::number(laserWindow->myDockControls->ui->wavelengthScrollBar->value());
    QString pulseStr= laserWindow->myDockControls->ui->pulseControl->getTitle()+ "= " + QString::number(laserWindow->myDockControls->ui->pulseControl->getScientificNumber(), 'e', 2);
    QString divergenceStr= "&phi; [mrad]= " +  QString::number(laserWindow->myDockControls->ui->divergenceControl->getScientificNumber(), 'e', 2);
    QString diameterStr= "a [mm]= " + QString::number(laserWindow->myDockControls->ui->beamDiameterControl->getScientificNumber(), 'e', 2);
    QString prfStr= laserWindow->myDockControls->ui->prfControl->getTitle()+ "= " + QString::number(laserWindow->myDockControls->ui->prfControl->getScientificNumber(), 'e', 2);
    QString teSkin="T<sub>cute</sub> [s]= " +QString::number(laserWindow->myDockControls->ui->T_SkinControl->getScientificNumber(),'f', 2);

    input.append(powerErgStr);
    input.append(wavelengthStr);
    input.append(pulseStr);
    input.append(divergenceStr);
    input.append(diameterStr);

    effects.append(laserWindow->myDockEffects->ui->tRadiationLabel->text()+":"+laserWindow->myDockEffects->ui->RadiationLabel->text());
    effects.append(laserWindow->myDockEffects->ui->tSkinDamageLabel->text()+":"+laserWindow->myDockEffects->ui->SkinDamageLabel->text());
    effects.append(laserWindow->myDockEffects->ui->tEyeDamageLabel->text()+":"+laserWindow->myDockEffects->ui->EyeDamageLabel->text());
    effects.append(laserWindow->myDockEffects->ui->tNoteLabel->text()+":"+laserWindow->myDockEffects->ui->NoteLabel->text());

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)
    {
        output.append(laserWindow->myDockResults->ui->conditions_Label->text()+"$"+laserWindow->myDockResults->ui->EMP_1st_Label->text());
        output.append("Dettaglio risultati:$ ");

        output.append(laserWindow->myDockResults->ui->tFormulaLabel->text()+"$"+laserWindow->myDockResults->ui->FormulaLabel->text());
        output.append(laserWindow->myDockResults->ui->tEMP_Label->text()+"$"+laserWindow->myDockResults->ui->EMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tExposureTimeLabel->text()+"$"+laserWindow->myDockResults->ui->ExposureTimeLabel->text());

        output.append(laserWindow->myDockResults->ui->tPowerErgLabel->text()+"$"+laserWindow->myDockResults->ui->PowerErgLabel->text());
        output.append(laserWindow->myDockEffects->ui->tCA_Label->text()+"$"+laserWindow->myDockEffects->ui->CA_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCB_Label->text()+"$"+laserWindow->myDockEffects->ui->CB_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCC_Label->text()+"$"+laserWindow->myDockEffects->ui->CC_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCE_Label->text()+"$"+laserWindow->myDockEffects->ui->CE_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT1_Label->text()+"$"+laserWindow->myDockEffects->ui->T1_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT2_Label->text()+"$"+laserWindow->myDockEffects->ui->T2_Label->text());
        output.append(laserWindow->myDockEffects->ui->tLimitingApertureLabel->text()+"$"+laserWindow->myDockEffects->ui->limitingApertureLabel->text());
        output.append(laserWindow->myDockResults->ui->tNOHDLabel->text()+"$"+laserWindow->myDockResults->ui->NOHDLabel->text());
        output.append(laserWindow->myDockResults->ui->tOD_FilterLabel->text()+"$"+laserWindow->myDockResults->ui->tOD_FilterLabel->text());

        skin.append(laserWindow->myDockSkin->ui->conditions_Label->text()+"$"+laserWindow->myDockSkin->ui->EMP_1st_Label->text());
        skin.append("Dettaglio risultati:$ ");
        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tNSHDLabel->text()+"$"+laserWindow->myDockSkin->ui->NSHDLabel->text());

        goggle.append(laserWindow->myDockGoggle->ui->kindOfLaserLabel->text()+": ");
        goggle.append(laserWindow->myDockGoggle->ui->tLaserOutputLabel->text()+":"+laserWindow->myDockGoggle->ui->laserOutputLabel->text());
        goggle.append("LB: " + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());

        input.append(teSkin);
    }

    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::PULSE)
    {
        output.append(laserWindow->myDockResults->ui->conditions_Label->text()+"$"+laserWindow->myDockResults->ui->EMP_1st_Label->text());
        output.append("Dettaglio risultati$ ");

        output.append(laserWindow->myDockResults->ui->tFormulaLabel->text()+"$"+laserWindow->myDockResults->ui->FormulaLabel->text());
        output.append(laserWindow->myDockResults->ui->tEMP_Label->text()+"$"+laserWindow->myDockResults->ui->EMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tExposureTimeLabel->text()+"$"+laserWindow->myDockResults->ui->ExposureTimeLabel->text());
        output.append(laserWindow->myDockResults->ui->tPowerErgLabel->text()+"$"+laserWindow->myDockResults->ui->PowerErgLabel->text());
        output.append(laserWindow->myDockEffects->ui->tCA_Label->text()+"$"+laserWindow->myDockEffects->ui->CA_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCB_Label->text()+"$"+laserWindow->myDockEffects->ui->CB_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCC_Label->text()+"$"+laserWindow->myDockEffects->ui->CC_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCE_Label->text()+"$"+laserWindow->myDockEffects->ui->CE_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT1_Label->text()+"$"+laserWindow->myDockEffects->ui->T1_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT2_Label->text()+"$"+laserWindow->myDockEffects->ui->T2_Label->text());
        output.append(laserWindow->myDockEffects->ui->tGammaLabel->text()+"$"+laserWindow->myDockEffects->ui->GammaLabel->text());
        output.append(laserWindow->myDockEffects->ui->tLimitingApertureLabel->text()+"$"+laserWindow->myDockEffects->ui->limitingApertureLabel->text());
        output.append(laserWindow->myDockResults->ui->tNOHDLabel->text()+"$"+laserWindow->myDockResults->ui->NOHDLabel->text());
        output.append(laserWindow->myDockResults->ui->tOD_FilterLabel->text()+"$"+laserWindow->myDockResults->ui->tOD_FilterLabel->text());

        skin.append(laserWindow->myDockSkin->ui->conditions_Label->text()+"$"+laserWindow->myDockSkin->ui->EMP_1st_Label->text());
        skin.append("Dettaglio risultati$ ");

        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tNSHDLabel->text()+"$"+laserWindow->myDockSkin->ui->NSHDLabel->text());

        goggle.append(laserWindow->myDockGoggle->ui->kindOfLaserLabel->text()+": ");
        goggle.append("Output: "+laserWindow->myDockGoggle->ui->laserOutputLabel->text());
        goggle.append("LB: " + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());
    }
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
    {
        output.append(laserWindow->myDockResults->ui->conditions_Label->text()+"$ ");

        QString firstCondition=QString("%1 $ %2")
        .arg("Prima condizione")
        .arg(laserWindow->myDockResults->ui->EMP_1st_Label->text());
        output.append(firstCondition);

        QString secondCondition=QString("%1 $ %2")
        .arg("Seconda condizione")
        .arg(laserWindow->myDockResults->ui->EMP_2nd_Label->text());
        output.append(secondCondition);

        QString thirdCondition=QString("%1 $ %2")
        .arg("Terza condizione")
        .arg(laserWindow->myDockResults->ui->EMP_3rd_Label->text());
        output.append(thirdCondition);

        output.append("Dettaglio risultati $ ");
        output.append(laserWindow->myDockResults->ui->tFormulaLabel->text()+"$"+laserWindow->myDockResults->ui->FormulaLabel->text());
        output.append(laserWindow->myDockResults->ui->tEMP_Label->text()+"$"+laserWindow->myDockResults->ui->EMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tFormulaMP_Label->text()+"$"+laserWindow->myDockResults->ui->FormulaMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tEMP_MP_Label->text().toLatin1()+"$"+laserWindow->myDockResults->ui->EMP_MP_Label->text().toLatin1());
        output.append(laserWindow->myDockResults->ui->tExposureTimeLabel->text()+"$"+laserWindow->myDockResults->ui->ExposureTimeLabel->text());
        output.append(laserWindow->myDockResults->ui->tPowerErgLabel->text()+"$"+laserWindow->myDockResults->ui->PowerErgLabel->text());

        output.append(laserWindow->myDockEffects->ui->tCA_Label->text()+"$"+laserWindow->myDockEffects->ui->CA_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCB_Label->text()+"$"+laserWindow->myDockEffects->ui->CB_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCC_Label->text()+"$"+laserWindow->myDockEffects->ui->CC_Label->text());
        output.append(laserWindow->myDockEffects->ui->tCE_Label->text()+"$"+laserWindow->myDockEffects->ui->CE_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT1_Label->text()+"$"+laserWindow->myDockEffects->ui->T1_Label->text());
        output.append(laserWindow->myDockEffects->ui->tT2_Label->text()+"$"+laserWindow->myDockEffects->ui->T2_Label->text());
        output.append(laserWindow->myDockEffects->ui->tLimitingApertureLabel->text()+"$"+laserWindow->myDockEffects->ui->limitingApertureLabel->text());
        output.append(laserWindow->myDockEffects->ui->tLimitingApertureMeanLabel->text()+"$"+laserWindow->myDockEffects->ui->limitingApertureMeanLabel->text());

        output.append(laserWindow->myDockResults->ui->tMeanPowerLabel->text()+"$"+laserWindow->myDockResults->ui->MeanPowerLabel->text());
        output.append(laserWindow->myDockResults->ui->tMeanIrradianceLabel->text()+"$"+laserWindow->myDockResults->ui->MeanIrradianceLabel->text());
        output.append(laserWindow->myDockResults->ui->tThermalEMP_Label->text()+"$"+laserWindow->myDockResults->ui->ThermalEMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tEMP_mean_Label->text()+"$"+laserWindow->myDockResults->ui->tEMP_mean_Label->text());
        output.append(laserWindow->myDockEffects->ui->tTminLabel->text()+"$"+laserWindow->myDockEffects->ui->TminLabel->text());
        output.append(laserWindow->myDockEffects->ui->tDeltaLabel->text()+"$"+laserWindow->myDockEffects->ui->deltaLabel->text());

        output.append(laserWindow->myDockResults->ui->tCP_Label->text()+"$"+laserWindow->myDockResults->ui->CP_Label->text());
        output.append(laserWindow->myDockResults->ui->tPulseNumberLabel->text()+"$"+laserWindow->myDockResults->ui->PulseNumberLabel->text());
        output.append(laserWindow->myDockResults->ui->tPulseNumberThLabel->text()+"$"+laserWindow->myDockResults->ui->PulseNumberLabel->text());
        output.append(laserWindow->myDockResults->ui->tminEMP_Label->text()+"$"+laserWindow->myDockResults->ui->minEMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tNOHDLabel->text()+"$"+laserWindow->myDockResults->ui->NOHDLabel->text());

        output.append(laserWindow->myDockResults->ui->tOD_FilterLabel->text()+"$"+laserWindow->myDockResults->ui->OD_FilterLabel->text());
        output.append(laserWindow->myDockResults->ui->tOD_MeanFilterLabel->text()+"$"+laserWindow->myDockResults->ui->OD_MeanFilterLabel->text());

        input.append(prfStr);
        input.append(teSkin);

        effects.append(laserWindow->myDockResults->ui->tCountingLabel->text()+":"+laserWindow->myDockResults->ui->CountingLabel->text());

        skin.append(laserWindow->myDockSkin->ui->conditions_Label->text()+"$ ");
        QString skinFirstCondition=QString("%1 $ %2")
        .arg("Prima condizione")
        .arg(laserWindow->myDockSkin->ui->EMP_1st_Label->text());
        skin.append(skinFirstCondition);

        QString skinSecondCondition=QString("%1 $ %2")
        .arg("Seconda condizione")
        .arg(laserWindow->myDockSkin->ui->EMP_2nd_Label->text());

        skin.append(skinSecondCondition);
        skin.append("Dettaglio risultati$ ");

        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinMP_Label->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinMP_Label->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_MP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_MP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tMeanPowerSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->MeanPowerSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tMeanIrradianceSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->MeanIrradianceSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tPulseNumberSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->PulseNumberSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tminEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->minEMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tNSHDLabel->text()+"$"+laserWindow->myDockSkin->ui->tNSHDLabel->text());

        goggle.append(laserWindow->myDockGoggle->ui->kindOfLaserLabel->text()+": ");
        goggle.append(laserWindow->myDockGoggle->ui->tLaserOutputLabel->text()+":"+laserWindow->myDockGoggle->ui->laserOutputLabel->text());
        goggle.append("LB:" + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tn_maxLabel->text()+":"+laserWindow->myDockGoggle->ui->n_maxLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tMeanPowerLabel->text()+":"+laserWindow->myDockGoggle->ui->meanPowerLabel->text());
        goggle.append("LB effetti medi: " + laserWindow->myDockGoggle->ui->scaleNumberDLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tCoefficient_kLabel->text()+":"+laserWindow->myDockGoggle->ui->coefficient_kLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tCoefficient_kiLabel->text()+":"+laserWindow->myDockGoggle->ui->coefficient_kiLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tNumberOfPulseLabel->text()+":"+laserWindow->myDockGoggle->ui->numberOfPulseLabel->text());
    }
    qDebug() << output;
}


void LaserReport::classifierDetails()
{
    classifierDetailsOutput.clear();

    QString spEffects;

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)
        spEffects="Criterio della potenza : ";
    else
        spEffects="Criterio dell'impulso : ";

    classifierDetailsOutput.append(spEffects);

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)
    {
        laserWindow->myDockControls->leaExpressions_CW();

        QString LEA_Classe1Str= "LEA Classe 1 e 1M  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_CW()[0]);
        QString LEA_Classe2Str= "LEA Classe 2 e 2M  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_CW()[1]);
        QString LEA_Classe3RStr= "LEA Classe 3R  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_CW()[2]);
        QString LEA_Classe3BStr= "LEA Classe 3B  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_CW()[3]);

        classifierDetailsOutput.append(LEA_Classe1Str);
        classifierDetailsOutput.append(LEA_Classe2Str);
        classifierDetailsOutput.append(LEA_Classe3RStr);
        classifierDetailsOutput.append(LEA_Classe3BStr);
    }
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::PULSE)
    {
        laserWindow->myDockControls->leaExpressions_SP();

        QString LEA_Classe1Str= "LEA Classe 1 e 1M  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP()[0]);
        QString LEA_Classe2Str= "LEA Classe 2 e 2M  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP()[1]);
        QString LEA_Classe3RStr= "LEA Classe 3R  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP()[2]);
        QString LEA_Classe3BStr= "LEA Classe 3B  :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP()[3]);

        classifierDetailsOutput.append(LEA_Classe1Str);
        classifierDetailsOutput.append(LEA_Classe2Str);
        classifierDetailsOutput.append(LEA_Classe3RStr);
        classifierDetailsOutput.append(LEA_Classe3BStr);
    }
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
    {
        laserWindow->myDockControls->leaExpressions_MP();

        QString LEA_Classe1_SP_MultiPulseStr= "LEA Classe 1 e 1M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP_MultiPulse()[0]);
        QString LEA_Classe2_SP_MultiPulseStr= "LEA Classe 2 e 2M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP_MultiPulse()[1]);
        QString LEA_Classe3R_SP_MultiPulseStr= "LEA Classe 3R :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP_MultiPulse()[2]);
        QString LEA_Classe3B_SP_MultiPulseStr= "LEA Classe 3B :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_SP_MultiPulse()[3]);

        QString effects_MeanStr="Criterio della potenza media : ";
        QString LEA_Classe1_MeanStr= "LEA Classe 1 e 1M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Mean()[0]);
        QString LEA_Classe2_MeanStr= "LEA Classe 2 e 2M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Mean()[1]);
        QString LEA_Classe3R_MeanStr= "LEA Classe 3R :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Mean()[2]);
        QString LEA_Classe3B_MeanStr= "LEA Classe 3B :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Mean()[3]);

        classifierDetailsOutput.append(LEA_Classe1_SP_MultiPulseStr);
        classifierDetailsOutput.append(LEA_Classe2_SP_MultiPulseStr);
        classifierDetailsOutput.append(LEA_Classe3R_SP_MultiPulseStr);
        classifierDetailsOutput.append(LEA_Classe3B_SP_MultiPulseStr);

        classifierDetailsOutput.append(effects_MeanStr);
        classifierDetailsOutput.append(LEA_Classe1_MeanStr);
        classifierDetailsOutput.append(LEA_Classe2_MeanStr);
        classifierDetailsOutput.append(LEA_Classe3R_MeanStr);
        classifierDetailsOutput.append(LEA_Classe3B_MeanStr);

        if(laserWindow->myDockControls->isThermal_LaserCLass())
        {
            QString effects_thermalStr="Criterio dell'impulso termico";

            if(!laserWindow->myDockControls->isHF_LaserCLass())
                effects_thermalStr+=" : ";
            else
                effects_thermalStr+=" ad alta frequenza di ripetizione : ";

            QString LEA_Classe1_ThrmalStr= "LEA Classe 1 e 1M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Thermal()[0]);
            QString LEA_Classe2_ThrmalStr= "LEA Classe 2 e 2M :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Thermal()[1]);
            QString LEA_Classe3R_ThrmalStr= "LEA Classe 3R :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Thermal()[2]);
            QString LEA_Classe3B_ThrmalStr= "LEA Classe 3B :" + QString::fromStdString(laserWindow->myDockControls->getLeaExpressions_Thermal()[3]);

            classifierDetailsOutput.append(effects_thermalStr);
            classifierDetailsOutput.append(LEA_Classe1_ThrmalStr);
            classifierDetailsOutput.append(LEA_Classe2_ThrmalStr);
            classifierDetailsOutput.append(LEA_Classe3R_ThrmalStr);
            classifierDetailsOutput.append(LEA_Classe3B_ThrmalStr);
        }
    }
}

void LaserReport::classifierResults()
{
    classifierResultsOutput.clear();


    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)
    {
        QString timeBaseStr ="Base dei tempi :" + laserWindow->myDockControls->getLEA_DataVector().at(0);
        classifierResultsOutput.append(timeBaseStr);
    }

    if((laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)||(laserWindow->myDockControls->laserOperation()==DockControls::operation::PULSE))
    {
        QString couplingFactor1str= "&eta;<sub>1</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(8);
        QString couplingFactor3str= "&eta;<sub>3</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(9);
        QString apertureDiam1str= "a<sub>1</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(10);
        QString apertureDiam3str= "a<sub>3</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(11);
        QString apertureDist1str= "d<sub>1</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(12);
        QString apertureDist3str= "d<sub>3</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(13);
        QString beamAperture1str= "a<sub>b1</sub> :"  + laserWindow->myDockControls->getLEA_DataVector().at(14);
        QString beamAperture3str= "a<sub>b3</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(15);
        QString formulaLEAstr= "Formula LEA :" + laserWindow->myDockControls->getLEA_DataVector().at(2);
        QString LEAstr= laserWindow->myDockControls->getLEA_DataVector().at(1) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(3);
        QString powerErgCond1LEAstr= laserWindow->myDockControls->getLEA_DataVector().at(4) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(5);
        QString powerErgCond3LEAstr= laserWindow->myDockControls->getLEA_DataVector().at(6) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(7);
        QString classStr= "Classe sistema :" + laserWindow->myDockControls->getLEA_DataVector().at(16);

        classifierResultsOutput.append(couplingFactor1str);
        classifierResultsOutput.append(couplingFactor3str);
        classifierResultsOutput.append(apertureDiam1str);
        classifierResultsOutput.append(apertureDiam3str);
        classifierResultsOutput.append(apertureDist1str);
        classifierResultsOutput.append(apertureDist3str);

        classifierResultsOutput.append(beamAperture1str);
        classifierResultsOutput.append(beamAperture3str);
        classifierResultsOutput.append(formulaLEAstr);
        classifierResultsOutput.append(LEAstr);
        classifierResultsOutput.append(powerErgCond1LEAstr);
        classifierResultsOutput.append(powerErgCond3LEAstr);
        classifierResultsOutput.append(classStr);
    }
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
    {
        QString pulseEffects="Criterio dell'impulso : ";
        QString couplingFactor1str= "&eta;<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(15);
        QString couplingFactor3str= "&eta;<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(16);
        QString apertureDiam1str= "a<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(17);
        QString apertureDiam3str= "a<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(18);
        QString apertureDist1str= "d<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(19);
        QString apertureDist3str= "d<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(20);
        QString beamAperture1str= "a<sub>b1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(21);
        QString beamAperture3str= "a<sub>b3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(22);
        QString formulaLEAstr= "Formula LEA :" + laserWindow->myDockControls->getLEA_DataVector().at(9);
        QString LEAstr= laserWindow->myDockControls->getLEA_DataVector().at(8) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(10);
        QString powerErgCond1LEAstr=laserWindow->myDockControls->getLEA_DataVector().at(11) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(12);
        QString powerErgCond3LEAstr=laserWindow->myDockControls->getLEA_DataVector().at(13) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(14);
        QString classStr= "Classe parziale :" + laserWindow->myDockControls->getLEA_DataVector().at(23);

        QString meanPowerEffects="Criterio della potenza media : ";
        QString meanPowerLabelStr= "P<sub>m</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(4);
        QString Te_Str=  "T<sub>e</sub> :"+ laserWindow->myDockControls->getLEA_DataVector().at(1);

        QString couplingFactor1str_2= "&eta;<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(31);
        QString couplingFactor3str_2= "&eta;<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(32);
        QString apertureDiam1str_2= "a<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(33);
        QString apertureDiam3str_2= "a<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(34);
        QString apertureDist1str_2= "d<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(35);
        QString apertureDist3str_2= "d<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(36);
        QString beamAperture1str_2= "a<sub>b1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(37);
        QString beamAperture3str_2= "a<sub>b3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(38);
        QString formulaLEAstr_2= "Formula LEA :" + laserWindow->myDockControls->getLEA_DataVector().at(25);

        QString LEAstr_2= laserWindow->myDockControls->getLEA_DataVector().at(24) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(26);
        QString powerErgCond1LEAstr_2=laserWindow->myDockControls->getLEA_DataVector().at(27) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(28);
        QString powerErgCond3LEAstr_2=laserWindow->myDockControls->getLEA_DataVector().at(29) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(30);
        QString classStr_2= "Classe parziale :" + laserWindow->myDockControls->getLEA_DataVector().at(39);

        classifierResultsOutput.append(pulseEffects);
        classifierResultsOutput.append(couplingFactor1str);
        classifierResultsOutput.append(couplingFactor3str);
        classifierResultsOutput.append(apertureDiam1str);
        classifierResultsOutput.append(apertureDiam3str);
        classifierResultsOutput.append(apertureDist1str);
        classifierResultsOutput.append(apertureDist3str);

        classifierResultsOutput.append(beamAperture1str);
        classifierResultsOutput.append(beamAperture3str);
        classifierResultsOutput.append(formulaLEAstr);
        classifierResultsOutput.append(LEAstr);
        classifierResultsOutput.append(powerErgCond1LEAstr);
        classifierResultsOutput.append(powerErgCond3LEAstr);

        classifierResultsOutput.append(meanPowerEffects);
        classifierResultsOutput.append(meanPowerLabelStr);
        classifierResultsOutput.append(Te_Str);
        classifierResultsOutput.append(couplingFactor1str_2);
        classifierResultsOutput.append(couplingFactor3str_2);
        classifierResultsOutput.append(apertureDiam1str_2);
        classifierResultsOutput.append(apertureDiam3str_2);
        classifierResultsOutput.append(apertureDist1str_2);
        classifierResultsOutput.append(apertureDist3str_2);

        classifierResultsOutput.append(beamAperture1str_2);
        classifierResultsOutput.append(beamAperture3str_2);
        classifierResultsOutput.append(formulaLEAstr_2);
        classifierResultsOutput.append(LEAstr_2);
        classifierResultsOutput.append(powerErgCond1LEAstr_2);
        classifierResultsOutput.append(powerErgCond3LEAstr_2);

        if(laserWindow->myDockControls->isThermal_LaserCLass())
        {
            QString thermalEffects="Criterio dell'impulso per gli effetti termici : ";
            QString couplingFactor1str_3= "&eta;<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(47);
            QString couplingFactor3str_3= "&eta;<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(48);
            QString apertureDiam1str_3= "a<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(49);
            QString apertureDiam3str_3= "a<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(50);
            QString apertureDist1str_3= "d<sub>1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(51);
            QString apertureDist3str_3= "d<sub>3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(52);
            QString beamAperture1str_3= "a<sub>b1</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(53);
            QString beamAperture3str_3= "a<sub>b3</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(54);
            QString formulaLEAstr_3= "Formula LEA :" + laserWindow->myDockControls->getLEA_DataVector().at(41);
            QString LEAstr_3= laserWindow->myDockControls->getLEA_DataVector().at(40) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(42);
            QString powerErgCond1LEAstr_3=laserWindow->myDockControls->getLEA_DataVector().at(43) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(44);
            QString powerErgCond3LEAstr_3=laserWindow->myDockControls->getLEA_DataVector().at(45) + " :" + laserWindow->myDockControls->getLEA_DataVector().at(46);

            QString Ti_Str= "T<sub>i</sub> :" +  laserWindow->myDockControls->getLEA_DataVector().at(2);
            QString Ti_prf_Str= "T<sub>i</sub> &times; PRF :" + laserWindow->myDockControls->getLEA_DataVector().at(3);
            QString C5_Str=  "C<sub>5</sub> :" + laserWindow->myDockControls->getLEA_DataVector().at(7);
            QString pulseNumberLabel_Str= "N :" +  laserWindow->myDockControls->getLEA_DataVector().at(5);
            QString countingLabel_Str= "Conteggio :" + laserWindow->myDockControls->getLEA_DataVector().at(6);

            classifierResultsOutput.append(thermalEffects);
            classifierResultsOutput.append(Ti_Str);
            classifierResultsOutput.append(Ti_prf_Str);
            classifierResultsOutput.append(C5_Str);
            classifierResultsOutput.append(pulseNumberLabel_Str);
            classifierResultsOutput.append(countingLabel_Str);

            classifierResultsOutput.append(couplingFactor1str_3);
            classifierResultsOutput.append(couplingFactor3str_3);
            classifierResultsOutput.append(apertureDiam1str_3);
            classifierResultsOutput.append(apertureDiam3str_3);
            classifierResultsOutput.append(apertureDist1str_3);
            classifierResultsOutput.append(apertureDist3str_3);

            classifierResultsOutput.append(beamAperture1str_3);
            classifierResultsOutput.append(beamAperture3str_3);
            classifierResultsOutput.append(formulaLEAstr_3);
            classifierResultsOutput.append(LEAstr_3);
            classifierResultsOutput.append(powerErgCond1LEAstr_3);
            classifierResultsOutput.append(powerErgCond3LEAstr_3);
        }
    }
}

void LaserReport::setSelectionRect(const QRect & _rect)
{
    previewRect=_rect;
}

void LaserReport::firstPageReport()
{
    laser.clear();
    reflectors.clear();

    //leggo i valori riguradanti laserpoint

    QString laserPositionStr= QString("Posizione [x, y] : (%1,%2)")
                                      .arg(laserpoint->pos().x())
                                      .arg(laserpoint->pos().y());

    double qualityFactor=BeamInspector::getQualityFactor();
    QString qualityFactorString="M<sup>2</sup>: " +QString::number(qualityFactor);

    bool noFeasible;
    DockControls::operation laserOperation=laserWindow->myDockControls->laserOperation();
    if(laserOperation==DockControls::operation::MULTI_PULSE)
        noFeasible=(laserWindow->myDockControls->getPRF()*laserWindow->myDockControls->getPulseWidth()>=1);
    else
        noFeasible=false;

    double rayleighDistance=BeamInspector::getRayleighDistance();
    QString rayleighDistanceString="z<sub>R</sub>: " +QString::number(rayleighDistance, 'e', 2);

    QString aperturaStr ="Apertura zona di sgombero: " +
            QString::number(laserpoint->getAperture())+"&deg;";

    QString laserPillowZone= QString("Ampiezza zona cuscinetto [gradi] : %1")
                                      .arg(laserpoint->getPillow());

    int installation=laserpoint->getInstallationIndex();
            QString installationStr="Tipo installazione: ";

    switch(installation)
    {
        case (0):
        installationStr+="Installazione stabile";
        break;

        case (1):
        installationStr+="Installazione poco stabile";
        break;

        case (2):
        installationStr+="Installazione mobile";
        break;

        case (3):
        installationStr+="Assenza di piattaforma";
        break;
    }

    QString environment="Ambiente:";

    if(isIndoor())
        environment+=" laboratorio";
    else
        environment+=" poligono";

    QString armAttenuationString;
    QString scintillationString;

    if(isIndoor())
    {
        armAttenuationString= "Effetti Atmosferici: non applicabile";
        scintillationString= "Scintillazione: non applicabile";
    }
    else
    {
        if(laserWindow->getAtmEffectsBool())
            armAttenuationString= "Effetti Atmosferici: valutati";
        else
            armAttenuationString= "Effetti Atmosferici: non valutati";

        if(laserWindow->getScintillationBool())
            scintillationString= "Scintillazione: valutata";
        else
            scintillationString= "Scintillazione: non valutata";
    }


    QString filterStr;
    QString transmittanceStr;

    if(laserpoint->isFilterOn())
    {
        filterStr="Filtro montato su ottica: presente";
        transmittanceStr="Transmittanza: "+ QString::number(laserpoint->getTransmittance());
    }

    else
    filterStr="Filtro montato su ottica: assente";

    if(qualityFactor<1 || noFeasible)
    {
        QString noPhysicalString="Fascio fisicamente non realizzabile :dr";
        laser.append(noPhysicalString);
    }

    laser.append(laserPositionStr);
    laser.append(qualityFactorString);
    laser.append(rayleighDistanceString);
    laser.append(aperturaStr);

    if(laserpoint->isFilterOn())
        laser.append(transmittanceStr);

    laser.append(installationStr);
    laser.append(environment);
    laser.append(laserPillowZone);
    laser.append(filterStr);
    laser.append(armAttenuationString);
    laser.append(scintillationString);

    //leggo i valori riguradanti i riflettori
}

void LaserReport::reflectorsValuation()
{
    QString inHazardArea;
    reflectors.clear();

    if(reflector->getMaxElement()==0)
        inHazardArea="No";
    else
       inHazardArea="Si";

    reflectors.append("Posizione [x, y]: " + QString("(%1,%2)")
                          .arg(reflector->pos().x())
                          .arg(reflector->pos().y()));
    reflectors.append("Nell'area di rischio: "+ inHazardArea);
    reflectors.append("Tipo di riflettore: " + reflector->getReflectorKindString());
    reflectors.append("&phi; [mrad]: " + QString::number(reflector->getDivergence()));
    reflectors.append("L [m]: " + QString::number(reflector->getReflectorDistance(),'f',1));
    reflectors.append("&rho;: " + QString::number(reflector->getMaterialCoeff()));
    reflectors.append("P [gradi]: " + QString::number(reflector->getPositioning()));
    reflectors.append("DRO<sub>r</sub> [m]: "+ QString::number(reflector->getPositioningElement(),'f',1));

    if(reflector->getReflectorKind()!=MIRROR_TARGET)
        reflectors.append("DRO<sub>r max</sub> [m]: "+ QString::number(reflector->getMaxElement(),'f',1));
}

void LaserReport::safetySignsValuation()
{
    safetySigns.append("Segnale: "+ safetySign->getSafetySignKind());
    safetySigns.append("Posizione [x, y]: " + QString("(%1,%2)")
                          .arg(safetySign->pos().x())
                          .arg(safetySign->pos().y()));
}

void LaserReport::binocularsValuation()
{
    double wavelength=laserWindow->myDockControls->getWavelength();
    if((wavelength>=400)&&(wavelength<=1400))
    {
        binoculars.append("Posizione [x, y]: " + QString("(%1,%2)")
                          .arg(binocular->pos().x())
                          .arg(binocular->pos().y()));
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("DNROE [m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("L [m]: " + QString::number(binocular->getBinocularDistance()));
        binoculars.append("M<sub>x</sub>: " + QString::number(binocular->getMagnification()));
        binoculars.append("τ: " + QString::number(binocular->getTransmissionCoeff(),'f',1));
        binoculars.append("d<sub>0</sub> [mm]: " + QString::number(binocular->get_D0()));
        binoculars.append("d<sub>b</sub> [mm]: " + QString::number(binocular->get_Db()));
        binoculars.append("K: " + QString::number(binocular->getOpticalGain()));
        binoculars.append("Effetti nel'impiego: "+ binocular->getBinocularEffects());
     }
    else if(((wavelength>=320)&&(wavelength<400))||((wavelength>1400)&&(wavelength<=4500)))
    {
        binoculars.append("Posizione [x, y]: " + QString("(%1,%2)")
                          .arg(binocular->pos().x())
                          .arg(binocular->pos().y()));
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("M<sub>x</sub>: " + QString::number(binocular->getMagnification()));
        binoculars.append("DNROE[m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("L [m]: " + QString::number(binocular->getBinocularDistance()));
        binoculars.append("τ: " + QString::number(binocular->getTransmissionCoeff(),'f',1));
        binoculars.append("d<sub>b</sub> [mm]: " + QString::number(binocular->get_Db()));
        binoculars.append("K<sub>o</sub>: " + QString::number(binocular->getOpticalGain()));
        binoculars.append("Effetti nel'impiego: "+ binocular->getBinocularEffects());
    }
    else
    {
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("DRNOE [m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("L [m]: " + QString::number(binocular->getBinocularDistance()));
    }
}

void LaserReport::inspectorsValuation()
{
    inspectors.append("Posizione [x, y]: " + QString("(%1,%2)")
                     .arg(beamInspector->pos().x())
                     .arg(beamInspector->pos().y()));
    inspectors.append("Descrizione: " + beamInspector->getDescription());
    inspectors.append("L [m]: " + QString::number(beamInspector->getInspectorDistance(),'e', 2));
    inspectors.append("φ<sub>orizzontale</sub>: " + QString::number(beamInspector->getLinkInspectorPhase(),'e', 2));

    QString fieldType;
    if(beamInspector->isFarField())
        fieldType="Campo lontano";
    else
        fieldType="Campo vicino";

    inspectors.append("Propagazione: " + fieldType);

    if(beamInspector->isInZone())
    {
        QString D_b="D<sub>b</sup>[m]: " + QString::number(beamInspector->getSpotDiameter(),'e', 2);
        inspectors.append(D_b);
        QString safePosition="Effetti: posizione pericolosa";
        if(beamInspector->isRetinalHazard())
        {
            inspectors.append("Danno retinico (400 ≤ λ ≤ 1400 nm) :dr");
            double farFieldRatio=beamInspector->getRayleighDistance()/beamInspector->getInspectorDistance();
            inspectors.append("z<sub>R</sub>/L: " + QString::number(farFieldRatio, 'e', 2));
            inspectors.append("f<sub></sub> [m]: " + QString::number(beamInspector->get_fm(),'e', 2));
            inspectors.append("d<sub>r</sub> [μm]: " + QString::number(beamInspector->get_d_r(),'e', 2));
            inspectors.append("d<sub>ff</sub> [μm]: " + QString::number(beamInspector->get_d_r_FarField(),'e', 2));
            inspectors.append("e%: " + QString::number(beamInspector->getPercentError(),'f', 2));
            inspectors.append("r<sub>s</sub> [m]: " + QString::number(beamInspector->getCurvatureRadius(),'e', 2));
            inspectors.append("d<sub>s</sub> [mm]: " + QString::number(beamInspector->get_d_s(),'e', 2));
            inspectors.append("α [mrad]: " + QString::number(beamInspector->get_alpha_r(),'e', 2));
            inspectors.append("C<sub>E</sub>: " + QString::number(beamInspector->getCE(),'f', 2));
            if(!beamInspector->isFarField())
            {
                if(beamInspector->isLongExposure())
                {
                    inspectors.append(QString::fromStdString(beamInspector->getLongExposureEMP_Sort())+
                                      QString::fromStdString(beamInspector->getLongExposureEMP_Unit())+ ": "+
                                      QString::number(beamInspector->getLongExposureEMP()));
                    inspectors.append("Formula EMP per esposizioni lunghe: "+QString::fromStdString(beamInspector->getFormula()));
                    inspectors.append("T<sub>1</sub>: "+QString::number(beamInspector->getT1()));
                    inspectors.append("T<sub>2</sub>: "+QString::number(beamInspector->getT2()));
                    inspectors.append("Note: "+QString::fromStdString(beamInspector->getNotes()));

                    if(beamInspector->getLaserOperation()==DockControls::operation::MULTI_PULSE)
                        inspectors.append("Effetto prevalente: "+QString::fromStdString(beamInspector->getMainEffect()));
                }
                else
                {
                inspectors.append(QString::fromStdString(beamInspector->getEMP_Sort())+
                                  QString::fromStdString(beamInspector->getEMP_Unit())+ ": "
                                + QString::number(beamInspector->getAugmentedEMP(),'e', 2));
                }
                if(beamInspector->isSafePosition())
                    safePosition="Effetti: L'EMP è aumentato dal fattore C<sub>E</sub> ad un valore tollerabile, possibile posizione sicura";
            }
        }
        inspectors.append(safePosition);
    }
    else
    {
        if(beamInspector->isOutOfLaserAperture())
        {
            if(beamInspector->getInspectorDistance()<beamInspector->getAttenuatedDNRO())
                inspectors.append("Effetti: fascio non accessibile, possibile posizione sicura");
            else
                inspectors.append("Effetti: fascio non accessibile inoltre la distanza è maggiore della DNRO, possibile posizione sicura");
        }
        else
            inspectors.append("Effetti: distanza maggiore della DNRO, possibile posizione sicura");
    }
}


void LaserReport::footprintsValuation()
{
    footprints.append("Posizione [x, y]: " + QString("(%1,%2)")
                          .arg(footprint->pos().x())
                          .arg(footprint->pos().y()));
    footprints.append("Dimensioni [m X m]: "+ QString::number(footprint->getRectangle().rect().width()) + " X "
                          + QString::number(footprint->getRectangle().rect().height()));
}

QString LaserReport::htmlInstallationDescription()
{
    QString html;

    html +="<h1>Laser Report</h1>\n"
           "<table width="+correction+">\n"

            "<tr><th colspan=\"2\">Descrizione installazione</th>\n"

                   "<tr><td bgcolor=\"#fbfbfb\"><b>Forza Armata</b></td>\n"
                   "<td>"+laserWindow->getForce()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Ente</b></td>\n"
                   "<td>"+laserWindow->getCustomer()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Nominativo UASL</b></td>\n"
                   "<td>"+laserWindow->get_UASL()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Assistenti UASL</b></td>\n"
                   "<td>"+laserWindow->get_UASL_Assistant()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Descrizione apparcchiatura laser</b></td>\n"
                   "<td>"+laserWindow->getLaserDescription()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Descrizione dei luoghi</b></td>\n"
                   "<td>"+laserWindow->getPlaceDescription()+"</td></tr>\n"
                   "<tr><td bgcolor=\"#fbfbfb\"><b>Data valutazione</b></td>\n"
                   "<td>"+laserWindow->getCompilingDate().toString(QString("dd.MM.yyyy"))+"</td></tr>\n"
            "</table><br>\n";

    return html;
}

QString LaserReport::htmlLaserInstallation()
{
    QString html;

    html +="<table width="+correction+">\n"
            "<tr><th colspan=\"2\">Installazione del punto laser</th>\n";

    foreach (QString entry, laser)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

        if(body=="dr")
            html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
            html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b>\n</td>\n"
                   "<td>" + body + "</td>\n</tr>";
    }
    html += "</table><br>\n";

    return html;
}

QString LaserReport::kindOfLaser()
{
    QString kindOfLaser;

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::CONTINUOS_WAVE)
        kindOfLaser="Laser Countinuos Wave";
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::PULSE)
        kindOfLaser="Laser impulsato";
    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
        kindOfLaser="Laser ad impulsi multipli";

    return kindOfLaser;
}

QString LaserReport::htmlInputData(const QString & kindOfLaser)
{
    QString html;

    html += "<table width="+correction+">\n"
            "<tr>\n<th colspan=\"2\">Dati di ingresso</th>\n</tr>\n"
            "<tr>\n<td bgcolor=\"#fbfbfb\"><b>Tipo di laser</b></td>\n"
                           "<td>" + kindOfLaser + "</td>\n</tr>";

    foreach (QString entry, input)
    {
        QStringList fields = entry.split("=");
        QString title = fields[0];
        QString body = fields[1];

        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
               "<td>" + body + "</td>\n</tr>";
    }

    html +="</table><br>\n";
    return html;
}

QString LaserReport::htmlEyeExposition()
{
    QString html;

    html += "<table width="+correction+">\n"
            "<tr>\n<th colspan=\"2\">Valutazione D.Lgs. 81/2008  - Esposizione dell'occhio</th>\n</tr>\n";

    foreach (QString entry, output)
    {
        QStringList fields = entry.split("$");
        QString title = fields[0];
        QString body = fields[1];

        if(body==" ")
            html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
               "<td>" + body + "</td>\n</tr>\n";
    }

    html +="\n</table><br>\n";

    return html;
}

QString LaserReport::htmlSkinExposition()
{
    QString html;

    html += "<table width="+correction+">\n"
            "<tr>\n<th colspan=\"2\">Valutazione D.Lgs. 81/2008 - Esposizione della cute</th>\n</tr>\n";

    foreach (QString entry, skin)
    {
        QStringList fields = entry.split("$");
        QString title = fields[0];
        QString body = fields[1];

        if(body==" ")
            html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
               "<td>" + body + "</td>\n</tr>\n";
    }

    html +="\n</table><br>\n";

    return html;
}

QString LaserReport::htmlEffects()
{
    QString html;

    html +="<table width="+correction+">\n"
           "<tr>\n<th colspan=\"2\">Effetti</th>\n</tr>\n";

    foreach (QString entry, effects)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b>\n</td>\n"
               "<td>" + body + "</td>\n</tr>\n";
    }
    html +="\n</table><br>\n";

    return html;
}

QString LaserReport::htmlGoggleAssessment()
{
    QString html;

    html +="<table width="+correction+">\n"
            "<tr>\n<th colspan=\"2\">Dispositivi protettori</tr>\n</th>\n";

    foreach (QString entry, goggle)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

    if(body==" ")
        html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
    else
        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b>\n</td>\n"
               "<td>" + body + "</td>\n</tr>";
    }

    html +="\n</tbody>\n</table><br><br><br><br><br>\n";

    return html;
}

QString LaserReport::htmlGoggleDetails()
{
    QString html;

    html.clear();

    const vector< pair <int,double> >dataVector=laserWindow->myDockControls->getGoggleDataVect();
    const int scale=laserWindow->myDockControls->getGoggleScaleNumber();
    const std::string unit=laserWindow->myDockControls->getGoggleLimitsUnit();

    html += "<table width="+correction+">\n";
    html +=  "<tr>\n<th>Numero di scala</th>\n"
             "<th>Valore limite "+ QString::fromStdString(unit)+ "</th>\n</tr>";


   vector< pair <int,double> >::const_iterator constIterator; // const_iterator
   // display vector elements using const_iterator

   for ( constIterator = dataVector.begin(); constIterator != dataVector.end(); ++constIterator )
    {
        if(constIterator->first==scale)
            html += "<tr><td><b>LB" + QString::number(constIterator->first) + "</b></td>\n<td><b>"
            + QString::number(constIterator->second) +"</b></td></tr>\n";
       else
            html += "<tr><td>LB" + QString::number(constIterator->first) + "</td>\n<td>"
            + QString::number(constIterator->second) +"</td></tr>\n";
       }

   html += "\n</table><br>\n";

   return html;
}

QString LaserReport::htmlDGoggleDetails()
{
    QString html;

    html.clear();

    const vector< pair <int,double> >dataVector=laserWindow->myDockControls->getDGoggleDataVect();
    const int scale=laserWindow->myDockControls->getDGoggleScaleNumber();
    const std::string unit=laserWindow->myDockControls->getDGoggleLimitsUnit();

    html += "<table width="+correction+">\n";
    html +=  "<tr>\n<th>Numero di scala</th>\n"
             "<th>Valore limite "+ QString::fromStdString(unit)+ "</th>\n</tr>";


   vector< pair <int,double> >::const_iterator constIterator; // const_iterator
   // display vector elements using const_iterator

   for ( constIterator = dataVector.begin(); constIterator != dataVector.end(); ++constIterator )
    {
       if(constIterator->first==scale)
       html += "<tr><td><b>LB" + QString::number(constIterator->first) + "</b></td>\n<td><b>"
            + QString::number(constIterator->second) +"</b></td></tr>\n";
       else
       html += "<tr><td>LB" + QString::number(constIterator->first) + "</td>\n<td>"
            + QString::number(constIterator->second) +"</td></tr>\n";
    }

   html += "\n</table><br>\n";

   return html;
}

void LaserReport::buidReflectorsDocumentPart()
{
    QString html;

    if(!myReflectors.empty())
    {
        html +="<br><h2>Elementi riflettori presenti nell'area di sgombero</h2>";
        QList<Reflector*>::iterator myIterator; // iterator
        myIterator = myReflectors.begin();
        int i =1;

        while (myIterator != myReflectors.end() )
        {
            reflector=*myIterator;
            myCursor.insertHtml(htmlReflectors(i));

            if(reflector->getReflectorKind()!=MIRROR_TARGET)
            {
                QUrl Uri=QUrl("mydata://"+reflectorsImageName[i-1]);

                QTextImageFormat imageFormat;
                imageFormat.setName(Uri.toString());
                imageFormat.setQuality(800);

                textDocument->addResource(QTextDocument::ImageResource,
                    Uri, QVariant(reflectorsGraphImage[i-1]));

                myCursor.insertImage(imageFormat);
            }
        ++i;
        ++myIterator;
        }
    }
}

QString LaserReport::htmlReflectors(const int & number)
{
    QString html;
    QString htmlImage;

    reflectorsValuation();

    if(reflector->getReflectorKind()==WET_TARGET)
    {
        if(reflector->getOpticalDiameter()!=0)
        {
            htmlImage+="<h3>Coefficienti di riflessione da superficie bagnata</h3>\n";
            htmlImage+=printSpecularReflectorCoefficients(reflector->getRho_sVect());

            htmlImage+="<h3>Distanza di sicurezza in funzione dell'angolo di riflessione (valori non nulli)</h3>\n";
            htmlImage+=printReflectorTable(reflector->getZsVect());
        }
    }
    else if(reflector->getReflectorKind()==GLASS_TARGET)
    {
        if(reflector->getOpticalDiameter()!=0)
        {
            htmlImage+="<h3>Coefficienti di riflessione da superficie di vetro ottico</h3>\n";
            htmlImage+=printSpecularReflectorCoefficients(reflector->getRho_sVect());

            htmlImage+="<h3>Distanza di sicurezza in funzione dell'angolo di riflessione (valori non nulli)</h3>\n";
            htmlImage+=printReflectorTable(reflector->getZsVect());
        }
    }
    else if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
    {
        if(reflector->getOpticalDiameter()!=0)
        {
            htmlImage+="<h3>Distanza di sicurezza in funzione dell'angolo di riflessione (valori non nulli)</h3>\n";
            htmlImage+=printReflectorTable(reflector->getZsVect());
        }
    }
    html +="<table width="+correction+">\n"
             "<tr><th colspan=\"2\">Riflettore n. "+ QString::number(number) +"</th></tr>\n";

    foreach (QString entry, reflectors)
    {
        QStringList fields = entry.split(": ");
        QString title = fields[0];
        QString body = fields[1];

        html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                   "<td>" + body + "</td>\n</tr>\n";
    }

    html +="\n</table><br>\n";
    html +=htmlImage+"\n<hr><br><br>\n";

    return html;
}

QString LaserReport::printReflectorTable( vector< pair <double,double> > myVector)
{
    QString html;

    html += "<table width="+correction+">\n";
    html +=  "<tr>\n<th>Angolo [gradi]</th>\n"
             "<th>Distanza sicurezza [m]</th>\n</tr>";


   vector< pair <double,double> >::const_iterator constIterator; // const_iterator
   // display vector elements using const_iterator
   int step=5;

   /*************************************************************************************************
    * La funzioone sd::advance non riconosce la fine del ciclo for se il numero di elementi non è   *
    * un multiplo dello step. Per tale motivo verifico la dimensione con il modulo. Se il valore è  *
    * diverso da zeo elimino tanti elementi dal  vettore (elementi inziali con pop_back) quanto è   *
    * il valore del modulo tanto la funzione dei coefficienti di riflessione è singolare in 0°      *
    * e si pone pari a 0.                                                                           *
    *************************************************************************************************/

   int vectorCount=myVector.size()%step;
   if(vectorCount!=0)
   {
       for(int i=0; i<vectorCount; i++)
            myVector.pop_back();
   }
   for ( constIterator = myVector.begin(); constIterator != myVector.end(); std::advance(constIterator, step ))
   {
       if(constIterator->second!=0)
       {
       html += "<tr>"
               "<td>" + QString::number(constIterator->first) + "</td>\n"
               "<td>" + QString::number(constIterator->second) +"</td>\n"
               "</tr>";
       }
    }

       html += "\n</table><br>\n";

       return html;
}

QString LaserReport::printSpecularReflectorCoefficients( vector< pair <double,double> > myVector)
{
    QString html;

    html += "<table width="+correction+">\n";
    html +=  "<tr>\n<th>Angolo [gradi]</th>\n"
             "<th>Coefficiente di riflessione &rho;<sub>s</sub></th>\n</tr>";


   vector< pair <double,double> >::const_iterator constIterator; // const_iterator
   // display vector elements using const_iterator
   int step=10;

   /*************************************************************************************************
    * La funzioone sd::advance non riconosce la fine del ciclo for se il numero di elementi non è   *
    * un multiplo dello step. Per tale motivo verifico la dimensione con il modulo. Se il valore è  *
    * diverso da zeo elimino tanti elementi dal  vettore (elementi inziali con pop_back) quanto è   *
    * il valore del modulo tanto la funzione dei coefficienti di riflessione è singolare in 0°      *
    * e si pone pari a 0.                                                                           *
    *************************************************************************************************/

   int vectorCount=myVector.size()%step;
   if(vectorCount!=0)
   {
       for(int i=0; i<vectorCount; i++)
            myVector.pop_back();
   }

   for ( constIterator = myVector.begin(); constIterator != myVector.end(); std::advance(constIterator, step))
   {
       if(constIterator->second!=0)
       {
       html += "<tr>"
               "<td>" + QString::number(constIterator->first) + "</td>\n"
               "<td>" + QString::number(constIterator->second) +"</td>\n"
               "</tr>";
       }
    }

       html += "\n</table><br>\n";

       return html;
}

QString LaserReport::htmlFootprints()
{
    QString html;

    if(!myFootprints.empty())
    {
        html +="<br><h2>Ingombri presenti nell'area</h2>";

        QList<FootprintObject*>::iterator myIterator; // iterator
        myIterator = myFootprints.begin();
        int i =1;
        while(myIterator != myFootprints.end())
        {
            footprint=*myIterator;
            footprints.clear();
            footprintsValuation();

            html +=
            "<table width="+correction+">\n"
            "<tr><th colspan=\"2\">Ingombro n. "+ QString::number(i) +"</th></tr>\n";

            foreach (QString entry, footprints)
            {
                QStringList fields = entry.split(": ");
                QString title = fields[0];
                QString body = fields[1];

                html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                       "<td>" + body + "</td>\n</tr>\n";
            }

        html +="\n</table><br>\n";
        ++myIterator;
    ++i;
        }
    }
    return html;
}

void LaserReport::buidInspectorsDocumentPart()
{
    QString html;

    if(!myBeamInspectors.empty())
    {
        html +="<br><h2>Segnaposti presenti nell'area</h2>";
        QList<BeamInspector*>::iterator myIterator; // iterator
        myIterator = myBeamInspectors.begin();
        int i =1;

        while (myIterator != myBeamInspectors.end() )
        {
            beamInspector=*myIterator;
            myCursor.insertHtml(htmlInspectors(i));

            if((beamInspector->isRetinalHazard())&&(beamInspector->isInZone()))
            {
                QUrl Uri=QUrl("mydata://"+inspectorsImageName[i-1]);

                QTextImageFormat imageFormat;
                imageFormat.setName(Uri.toString());
                imageFormat.setQuality(800);

                textDocument->addResource(QTextDocument::ImageResource,
                    Uri, QVariant(inspectorsGraphImage[i-1]));

                myCursor.insertImage(imageFormat);
            }
        ++i;
        ++myIterator;
        }
    }
}

QString LaserReport::htmlInspectors(const int & number)
{
    QString html;

    inspectorsValuation();

    html += "<table width="+correction+">\n"
            "<tr><th colspan=\"2\">Segnaposto n. "+ QString::number(number) +"</th></tr>\n";

    foreach (QString entry, inspectors)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

        if(body=="dr")
           html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
           html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                  "<td>" + body + "</td>\n</tr>\n";
    }

    html +="\n</table><br>\n";

    return html;
}

QString LaserReport::htmlBinoculars()
{
    QString html;

    if(!myBinoculars.empty())
    {
        html +="<br><h2>Dispositivi ottici presenti</h2>";

        QList<Binocular*>::iterator myIterator; // iterator
        myIterator = myBinoculars.begin();
        int i =1;

        while(myIterator != myBinoculars.end())
        {
            binocular=*myIterator;
            binoculars.clear();
            binocularsValuation();

            html +=
            "<table width="+correction+">\n"
             "<tr><th colspan=\"2\">Dispositivo ottico n. "+ QString::number(i) +"</th></tr>\n";

            foreach (QString entry, binoculars) {
            QStringList fields = entry.split(": ");
            QString title = fields[0];
            QString body = fields[1];

            html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                   "<td>" + body + "</td>\n</tr>\n";
            }

            html +="\n</table><br>\n";
            ++myIterator;
          ++i;
        }
    }
    return html;
}

QString LaserReport::htmlSafetySigns()
{
    QString html;

    if(!mySafetySigns.empty())
    {
        QList<SafetySignItem*>::iterator myIterator; // iterator
        myIterator = mySafetySigns.begin();
        int i =1;

        html +=
        "<table width="+correction+">\n"
        "<tr><th colspan=\"2\">Segnaletica di sicurezza installata</th></tr>\n";

        while(myIterator != mySafetySigns.end())
        {
            safetySign=*myIterator;
            safetySigns.clear();
            safetySignsValuation();

            html +=
            "<tr><td colspan=\"2\"><i>Cartello n.<i> "+ QString::number(i) +"</td></tr>\n";
            foreach (QString entry, safetySigns) {
            QStringList fields = entry.split(": ");
            QString title = fields[0];
            QString body = fields[1];

            html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                   "<td>" + body + "</td>\n</tr>\n";
            }
            ++myIterator;
          ++i;
        }
        html +="\n</table><br>\n";
    }
    return html;
}

QString LaserReport::htmlClassifier()
{
    QString html;

    html +="<br><h2>Valutazione della Classe secondo la procedura semplificata CEI EN 60825-1</h2>\n";

    QImage myLaserWarning = QImage("./laser_warning.png");
    QUrl Uri=QUrl("mydata://laser_warning.png");

    textDocument->addResource(QTextDocument::ImageResource,
        Uri, QVariant(myLaserWarning));

    QString classStr=laserWindow->myDockControls->ui->class_Label->text();

    html +="<table width="+correction+">\n"
           "<tr><th colspan=\"2\">Livelli di Emissione Accessibili valutati</th>\n";

    foreach (QString entry, classifierDetailsOutput)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

        if(body==" ")
            html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
            html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
               "<td>" + body + "</td>\n</tr>\n";
    }

    html += "</table><br>\n";
    html +="<table width="+correction+">\n"
               "<tr><th colspan=\"2\">Risultato valutazione</th>\n";

    foreach (QString entry, classifierResultsOutput)
    {
        QStringList fields = entry.split(":");
        QString title = fields[0];
        QString body = fields[1];

        if(body==" ")
                html +="<tr>\n<td colspan=\"2\"><i>" + title + "</i></td>\n</tr>\n";
        else
            html +="<tr>\n<td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                   "<td>" + body + "</td>\n</tr>\n";
    }
        if(classStr!="Classe 1")
            html +="<tr>\n<td colspan=\"2\"><img style= width: 40px; height: 25px;\"\n"
        "alt=\"Attenzione\" title=\"Attenzione\"\n"
        "src=\"mydata://laser_warning.png\">  Attenzione, verificare la presenza delle etichette di sicurezza</td>\n</tr>\n";

         html += "</table><br>\n";

    return html;
}

QString LaserReport::htmlMeteo()
{
    QString htmlMeteo;

    htmlMeteo.clear();

    htmlMeteo += "<table width="+correction+">\n";
    htmlMeteo += "<tr><th colspan=\"2\" rowspan=\"1\">Condizioni meteo</th></tr>\n";

    htmlMeteo += "<tr>"
                 "<td bgcolor=\"#fbfbfb\"><b>Visibilità [m]</td>\n"
                 "<td>" + QString::number(laserWindow->getMeteoRange()) +"</td>\n"
                 "</tr>"
                 "<tr>"
                 "<td bgcolor=\"#fbfbfb\"><b>μ [m<sup>-1</sup>]</td>\n"
                 "<td>" + QString::number(laserWindow->getAtmoshericEffectsCoefficient())+"</td>\n"
                 "</tr>"
                 "<tr>"
                 "<td bgcolor=\"#fbfbfb\"><b>A</td>\n"
                 "<td>" + QString::number(laserWindow->getA_Coefficient()) +"</td>\n"
                 "</tr>";


     htmlMeteo += "\n</table><br>\n";

     return htmlMeteo;
}

void LaserReport::setReflectorsList(const QList<Reflector*> &_myReflectors)
{
    myReflectors=_myReflectors;
}

void LaserReport::setFootprintsList(const QList<FootprintObject*> &_myFootprints)
{
   myFootprints=_myFootprints;
}

void LaserReport::setBinocularsList(const QList<Binocular*> &_myBinoculars)
{
    myBinoculars=_myBinoculars;
}

void LaserReport::setSafetySignsList(const QList<SafetySignItem*> &_mySafetySigns)
{
    mySafetySigns=_mySafetySigns;
}

void LaserReport::setBeamInspectorsList(const QList<BeamInspector*> &_myBeamInspectors)
{
    myBeamInspectors=_myBeamInspectors;
}

void LaserReport::setReflectorsFilenameList(const QStringList &_reflectorsImageName)
{
    reflectorsImageName=_reflectorsImageName;
}

void LaserReport::setReflectorsGraphImageList(const QList<QImage> & _reflectorsGraphImage)
{
    reflectorsGraphImage=_reflectorsGraphImage;
}

void LaserReport::setInspectorsFilenameList(const QStringList & _inspectorsImageName)
{
    inspectorsImageName=_inspectorsImageName;
}

void LaserReport::setInspectorsGraphImageList(const QList<QImage> & _inspectorsGraphImage)
{
    inspectorsGraphImage=_inspectorsGraphImage;
}

void LaserReport::setIndoor(bool _indoor)
{
    indoor=_indoor;
}

bool LaserReport::isIndoor()
{
    return indoor;
}

QString LaserReport::htmlSymbols()
{
    QString html;

    html +=
"   <br><h2>Significato dei simboli impiegati</h2>"
"   <table width="+correction+">\n"
"    <tbody>"
"    <tr>"
"   <th style=\"width: 20%;\">Simbolo</th><th>Significato</th>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Dispositivo laser</i></td>"
"    </tr>"
"    <tr>"
"   <td><b>P</b> [W]</td><td  style=\"text-align: left;\">Potenza ottica emessa dal dispositivo</td>"
"    </tr>"
"    <tr>"
"   <td><b>Q</b> [J]</td><td  style=\"text-align: left;\">Energia di un impulso</td>"
"    </tr>"
"    <tr>"
"    <td><b>t</b> [s]</td><td  style=\"text-align: left;\">Durata di un impulso</td>"
"    </tr>"
"    <tr>"
"    <td><b>&phi;</b> [mrad]</td><td  style=\"text-align: left;\">Divergenza del fascio laser</td>"
"    </tr>"
"    <tr>"
"    <td><b>a</b> [mm]</td><td  style=\"text-align: left;\">Diametro del fascio laser corrispondente al 63% della potenza ottica</td>"
"    </tr>"
"    <tr>"
"    <td><b>&alpha;</b> [mrad]</td><td  style=\"text-align: left;\">Angolo sotteso dalla pi&ugrave; piccola superficie apparente</td>"
"    </tr>"
"    <tr>"
"    <td><b>PRF</b> [Hz]</td><td  style=\"text-align: left;\">Frequenza di ripetizione degli impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>&lambda;</b> [nm]</td><td  style=\"text-align: left;\">Lunghezza d'onda</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Parametri per il calcolo dell'esposizione massima permessa (rif. D.Lgs 81/2008).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>A</sub></b></td><td  style=\"text-align: left;\">Coefficiente di correzione spettrale per assorbimento retinico</td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>B</sub></b></td><td  style=\"text-align: left;\">Coefficiente correzione spettrale per danno fotochimico</td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>C</sub></b></td><td  style=\"text-align: left;\">Coefficiente di correzione spettrale per assorbimento pre-retinico</td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>E</sub></b></td><td  style=\"text-align: left;\">Coefficiente di correzione per superficie estese</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>1</sub></b> [s]</td><td  style=\"text-align: left;\">Tempo limite per esposizioni termiche</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>2</sub></b> [s]</td><td  style=\"text-align: left;\">Tempo di break-point di visione del fascio</td>"
"    </tr>"
"    <tr>"
"    <td><b>&gamma;</b> [mrad]</td><td  style=\"text-align: left;\">Campo di vista di misurazione</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>min</sub></b> [s]</td><td  style=\"text-align: left;\">Durata al di sotto della quale i gruppi di impulsi vengono sommati tra loro</td>"
"    </tr>"
"    <tr>"
"    <td><b>δ</b></td><td  style=\"text-align: left;\">Duty Cycle</td>"
"    </tr>"
"    <tr>"
"    <td><b>a<sub>L</sub></b> [mm]</td><td style=\"text-align: left;\">Apertura limite</td>"
"    </tr>"
"    <tr>"
"    <td><b>a<sub>L mean</sub></b> [mm]</td><td style=\"text-align: left;\">Apertura limite per la durata del treno di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Esposizione massima permessa nel funzionamento ad onda continua e ad impulso (rif. D.Lgs 81/2008).</i></td>"
"    <tr>"
"    <td><b>E</b> [W/m<sup>2</sup>]<br><b>H</b> [J/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Esposizione massima permessa </td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>e</sub></b> [s]</td><td  style=\"text-align: left;\">Durata di esposizione dell'occhio nel funzionamento ad onda continua</td>"
"    </tr>"
"    <tr>"
"    <td><b>OD Filtro</b></td><td  style=\"text-align: left;\">Minimo valore della densit&agrave; ottica necessario per ottenere in uscita emissioni non pericolose</td>"
"    </tr>"
"    <tr>"
"    <td><b>DNRO</b> [m]</td><td  style=\"text-align: left;\">Distanza Nominale di Rischio Ottico (rif. CEI EN 60825-1 2017).</td>"
"    </tr>"
"    <tr>"
"   <td><b>t<sub>cute</sub></b> [s]</td><td  style=\"text-align: left;\">Durata di esposizione della pelle nel funzionamento ad onda continua</td>"
"    </tr>"
"    <tr>"
"    <td><b>DNRP</b> [m]</td><td  style=\"text-align: left;\">Distanza Nominale di Rischio per la pelle (rif. CEI EN 60825-1 2017).</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Esposizione massima permessa per l'occhio nel funzionamento ad impulsi multipli (rif. D.Lgs 81/2008).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>P<sub>m</sub></b> [W/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Potenza media emessa da un dispositivo laser nel caso di emissione multipla di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>m</sub></b> [W/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Irradianza media emessa da un dispositivo laser nel caso di emissione multipla di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>N<sub>th</sub></b></td><td  style=\"text-align: left;\">Numero di impulsi impiegato per il calcolo degli effetti cumulativi termici</td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>p</sub></b></td><td  style=\"text-align: left;\">Fattore di correzione termica cumulativo</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>Te</sub></b> [W/m<sup>2</sup>]<br><b>H<sub>Te</sub></b> [J/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Esposizione massima permessa corrispondente al tempo di esposizione T<sub>e</sub></td>"
"    </tr>"
"    <tr>"
"    <td><b>N</b></td><td  style=\"text-align: left;\">Numero di impulsi impiegato per il calcolo degli effetti medi</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>mean</sub></b> [W/m<sup>2</sup>]<br><b>H<sub>mean</sub></b> [J/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Esposizione massima permessa media del singolo impulso riferita al tempo di esposizione T<sub>e</sub></td>"
"    </tr>"
"    <tr>"
"    <td><b>OD Filtro</b></td><td  style=\"text-align: left;\">Minimo valore della densit&agrave; ottica per gli effetti cumulativi termici"
"         necessario per ottenere in uscita emissioni non pericolose</td>"
"    </tr>"
"    <tr>"
"    <td><b>OD Filtro Medio</b></td><td  style=\"text-align: left;\">Minimo valore della densit&agrave; ottica per la compontente media della radiazione per laser ad impulsi multipli"
"     necessario per ottenere in uscita emissioni non pericolose</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>th</sub></b> [W/m<sup>2</sup>]<br><b>H<sub>th</sub></b> [J/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Esposizione massima permessa media corrispondente al tempo di esposizione T<sub>e</sub> per gli effetti cumulativi termici</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>min</sub></b> [W/m<sup>2</sup>]<br><b>H<sub>min</sub></b> [J/m<sup>2</sup>]</td><td  style=\"text-align: left;\">Valore minimo dell'esposizione massima permessa</td>"
"    </tr>"
"    <tr>"
"    <td><b>DNRO</b> [m]</td><td  style=\"text-align: left;\">Distanza Nominale di Rischio Ottico (rif. CEI EN 60825-1 2017).</td>"
"    </tr>"
"    <tr>"
"    <td><b>t<sub>cute</sub></b> [s]</td><td  style=\"text-align: left;\">Durata di esposizione della pelle nel funzionamento ad onda continua</td>"
"    </tr>"
"    <tr>"
"    <td><b>DNRP</b> [m]</td><td  style=\"text-align: left;\">Distanza Nominale di Rischio per la pelle (rif. CEI EN 60825-1 2017).</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Parametri per il calcolo della marcatura dei protettori ottici (rif. UNI EN 207).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>LB</b></td><td  style=\"text-align: left;\">Marcatura</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>b</sub></b> [s]</td><td  style=\"text-align: left;\">Base dei tempi</td>"
"    </tr>"
"    <tr>"
"    <td><b>E</b> [W/m<sup>2</sup>]<br><b>H</b> [J/m<sup>2</sup>]</td><td style=\"text-align: left;\">Irradianza/Esposizione radiante emessa dal dispositivo laser in corrispondenza dell'apertura</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>c</sub></b> [W/m<sup>2</sup>]<br><b>H<sub>c</sub></b> [J/m<sup>2</sup>]</td><td style=\"text-align: left;\">Irradianza/Esposizione radiante emessa dal dispositivo laser in corrispondenza dell'apertura relativo agli effetti cumulativi</td>"
"    </tr>"
"    <tr>"
"    <td><b>k</b><td  style=\"text-align: left;\">Fattore di correzione termica cumulativo</td>"
"    </tr>"
"    <tr>"
"    <td><b>k<sub>Ti</sub><td  style=\"text-align: left;\">Fattore di correzione termica cumulativo per effetti in alta frequenza</td>"
"    </tr>"
"    <tr>"
"    <td><b>&nu;<sub>max</sub></sub><td style=\"text-align: left;\">Frequenza limite di ripetizione degli impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>P<sub>m</sub></b> [W/m<sup>2</sup>]</td><td style=\"text-align: left;\">Potenza media emessa da un dispositivo laser con emissione multipla di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>E<sub>m</sub></b> [W/m<sup>2</sup>]</td><td style=\"text-align: left;\">Irradianza media emessa da un dispositivo laser con emissione multipla di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>N</b></td><td  style=\"text-align: left;\">Numero di impulsi impiegato per il calcolo degli effetti cumulativi termici</td>"
"    </tr>";
    if(!myReflectors.empty())
    {
        html+=
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Riflettori (riff. JSP 390 2015; CEI EN 60825-4 2007).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>laser</sub></b> [m]</td><td style=\"text-align: left;\">Distanza dal dispositivo laser</td>"
"    </tr>"
"    <tr>"
"    <td><b>DRO<sub>r</sub></b> [m]</td><td style=\"text-align: left;\">Distanza di rischio ottico del riflettore</td>"
"    </tr>"
"    <tr>"
"    <td><b>n</sub></b></td><td style=\"text-align: left;\">indice di rifrazione</td>"
"    </tr>"
"    <tr>"
"    <td><b>&psi;</b> [gradi]</td><td style=\"text-align: left;\">Angolo di riflessione</td>"
"    </tr>"
"    <tr>"
"    <td><b>&psi;<sub>R</sub></b> [gradi]</td><td style=\"text-align: left;\">Angolo di osservazione del riflettore</td>"
"    </tr>"
"    <tr>"
"    <td><b>&tau;</sub></b></td><td style=\"text-align: left;\">Coefficiente di trasmissione</td>"
"    </tr>"
"    <tr>"
"    <td><b>&rho;</sub></b></td><td style=\"text-align: left;\">Coefficiente di assorbimento</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Riflettore diffondente (riff. JSP 390 2015; CEI EN 60825-4 2007).</i></td>"
"    <tr>"
"    <td><b>&theta;<sub>L</sub></b> [rad]</td><td style=\"text-align: left;\">Angolo rispetto al dispositivo laser, positivo se orario</td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>b</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del fascio al riflettore</td>"
"    </tr>"
"    <tr>"
"    <td><b>K</b></td><td style=\"text-align: left;\">Termine constante indipendente da &alpha;<sub>i</sub></td>"
"    </tr>"
"    <tr>"
"    <td><b>x<sub>0</sub></b></td><td style=\"text-align: left;\">Valore iniziale impiegato per la risoluzione dell'equazione trascendente per il calcolo di &alpha;<sub>i</sub></td>"
"    </tr>"
"    <tr>"
"    <td><b>&alpha;<sub>i</sub></b> [mrad]</td><td style=\"text-align: left;\">Indicatore definito come il rapporto tra gli angoli &alpha; ed &alpha;<sub>min</sub> (nel caso di superficie estesa media risulta pari a C<sub>E</sub>)</td>"
"    </tr>"
"    <tr>"
"    <td><b>&alpha;<sub>d</sub></b> [mrad]</td><td style=\"text-align: left;\">Angolo sotteso dall'immagine della riflessione alla distanza di rischio</td>"
"    </tr>";
    }
    if(!myBeamInspectors.empty())
    {
        html+=
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Segnaposto di ispezione (riff. CEI EN 60825-1 2017;  Galbiati - Evaluation of the apparent source in laser safety - Journal of Laser Applications, Vol. 13, No 4, August 2001).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>L</b> [m]</td><td style=\"text-align: left;\">Distanza dal dispositivo laser</td>"
"    </tr>"
"    <tr>"
"    <td><b>z<sub>R</sub></b> [m]</td><td style=\"text-align: left;\">Distanza di Rayleigh</td>"
"    </tr>"
"    <tr>"
"    <td><b>M<sup>2</sup></b></td><td style=\"text-align: left;\">Parametro di qualit&agrave; del fascio</td>"
"    </tr>"
"    <tr>"
"    <td><b>&phi;<sub>inspector</sub></b> [gradi]</td><td style=\"text-align: left;\">Angolo rispetto al dispositivo, positivo laser se orario</td>"
"    </tr>"
"    <tr>"
"    <td><b>z<sub>R</sub>/L</b> [m]</td><td style=\"text-align: left;\">Distanza di Rayleigh relativa</td>"
"    </tr>"
"    <tr>"
"    <td><b>f<sub>m</sub></b> [m]</td><td style=\"text-align: left;\">Minimo valore della distanza focale del cristallino</td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>r</sub></b> [&mu;m]</td><td style=\"text-align: left;\">Diametro dell'immagine retinica</td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>ff</sub></b> [&mu;m]</td><td style=\"text-align: left;\">Diametro dell'immagine retinica in ipotesi di campo lontano</td>"
"    </tr>"
"    <tr>"
"    <td><b>&epsilon;</b></td><td style=\"text-align: left;\">Errore percentuale che si commette nel valutare il diametro dell'immagine retinica assumendo valida l'ipotesi di campo lontano</td>"
"    </tr>"
"    <tr>"
"    <td><b>r<sub>s</sub></b> [mm]</td><td  style=\"text-align: left;\">Distanza della minima sorgente apparente dalla superficie corneale</td>"
"    </tr>";
    }
    if(!myBinoculars.empty())
    {
        html+=
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Dispositivi ottici (rif. CEI EN 60825-1 2017).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>DNROE</b> [m]</td><td style=\"text-align: left;\">Distanza Nominale di Rischio Ottico Esteso</td>"
"    </tr>"
"    <tr>"
"    <td><b>&tau;</b></td><td style=\"text-align: left;\">Coefficiente di trasmissione</td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>b</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del fascio sull'ottica</td>"
"    </tr>"
"    <tr>"
"    <td><b>L</b> [m]</td><td style=\"text-align: left;\">Distanza dal punto laser</td>"
"    </tr>"
"    <tr>"
"    <td><b>d<sub>0</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro della lente</td>"
"    </tr>"
"    <tr>"
"    <td><b>K</sub></b></td><td style=\"text-align: left;\">Guadagno ottico</td>"
"    </tr>";
}
    if(!mySafetySigns.empty())
    {
     QImage laser_sign = QImage("./laser_sign.png");
     QUrl UriSign1=QUrl("mydata://laser_sign.png");

     QImage forbidden_sign = QImage("./forbidden_sign.png");
     QUrl UriSign2=QUrl("mydata://forbidden_sign.png");

     QImage glass_sign = QImage("./glass_sign.png");
     QUrl UriSign3=QUrl("mydata://glass_sign.png");

     QImage skin_sign = QImage("./skin_sign.png");
     QUrl UriSign4=QUrl("mydata://skin_sign.png");

     textDocument->addResource(QTextDocument::ImageResource,
     UriSign1, QVariant(laser_sign));

     textDocument->addResource(QTextDocument::ImageResource,
     UriSign2, QVariant(forbidden_sign));

     textDocument->addResource(QTextDocument::ImageResource,
     UriSign3, QVariant(glass_sign));

     textDocument->addResource(QTextDocument::ImageResource,
     UriSign4, QVariant(skin_sign));

     html+=
            "    <tr>"
            "    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Segnaletica di sicurezza.</i></td>"
            "    </tr>"
            "    <tr>"
            "    <td> <img style= width: 40px; height: 40px;\"\n"
            "    alt=\"Divieto\" title=\"Divieto\"\n"
            "    src=\"mydata://forbidden_sign.png\">"
            "    </td>"
            "    <td style=\"text-align: left; vertical-align:middle;\">Pericolo radiazione laser></td>"
            "    </tr>"
            "    <tr>"
            "    <td> <img style= width: 40px; height: 36px;\"\n"
            "    alt=\"Pericolo\" title=\"Pericolo\"\n"
            "    src=\"mydata://laser_sign.png\">"
            "    </td>"
            "    <td style=\"text-align: left; vertical-align:middle;\">Divieto di accesso</td>"
            "    </tr>"
            "    <tr>"
            "    <td> <img style= width: 40px; height: 40px;\"\n"
            "    alt=\"Obbligo\" title=\"Obbligo\"\n"
            "    src=\"mydata://glass_sign.png\">"
            "    </td>"
            "    <td style=\"text-align: left; vertical-align:middle;\">Obbligo occhiali</td>"
            "    </tr>"
            "    <tr>"
            "    <td> <img style= width: 40px; height: 40px;\"\n"
            "    alt=\"Obbligo\" title=\"Obbligo\"\n"
            "    src=\"mydata://skin_sign.png\">"
            "    </td>"
            "    <td style=\"text-align: left; vertical-align:middle;\">Obbligo protezione per la pelle</td>"
            "    </tr>";
    }

    html+=
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Classificazione semplificata (rif. CEI EN 60825-1 2017).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>b</sub></b> [s]</td><td style=\"text-align: left;\">Base dei tempi</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>e</sub></b> [s]</td><td style=\"text-align: left;\">Durata del treno per gli effetti cumulativi</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>i</sub></b> [s]</td><td style=\"text-align: left;\">Durata al di sotto della quale i gruppi di impulsi vengono sommati tra loro</td>"
"    </tr>"
"    <tr>"
"    <td><b>T<sub>i</sub> &times; PRF</b></td><td style=\"text-align: left;\">Termine compensativo dell'energia dell'impulso nel caso di funzionamento in alta frequenza</td>"
"    </tr>"
"    <tr>"
"    <td><b>P<sub>m</sub></b> [W]</td><td style=\"text-align: left;\">Potenza media emessa da un dispositivo laser nel caso di emissione multipla di impulsi</td>"
"    </tr>"
"    <tr>"
"    <td><b>P<sub>Acc 1</sub></b> [W]</td><td style=\"text-align: left;\">Potenza accessibile relativa alla 1<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td><b>P<sub>Acc 3</sub></b> [W]</td><td style=\"text-align: left;\">Potenza accessibile relativa alla 3<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td><b>Q<sub>Acc 1</sub></b> [J]</td><td style=\"text-align: left;\">Energia dell'impulso accessibile relativa alla 1<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td><b>Q<sub>Acc 1</sub></b> [J]</td><td style=\"text-align: left;\">Energia dell'impulso accessibile relativa alla 3<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td><b>C<sub>5</sub></b></td><td style=\"text-align: left;\">Fattore di correzione termica cumulativo</td>"
"    </tr>"
"    <tr>"
"    <td><b>&eta;<sub>1</sub></b></td><td style=\"text-align: left;\">Parametro di accoppiamento relativo alla 1<sup>a</sup> condizione</td>"
"    </tr>"
"    <tr>"
"    <td><b>&eta;<sub>3</sub></b></td><td style=\"text-align: left;\">Parametro di accoppiamento relativo alla 3<sup>a</sup> condizione</td>"
"    </tr>"
"    <tr>"
"    <td><b>a<sub>1</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del diaframma relativo alla 1<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td><b>a<sub>3</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del diaframma relativo alla 3<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"   <td><b>d<sub>1</sub></b> [mm]</td><td style=\"text-align: left;\">Distanza dal diaframma relativa alla 1<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"   <td><b>d<sub>3</sub></b> [mm]</td><td style=\"text-align: left;\">Distanza dal diaframma relativa alla 3<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"   <td><b>a<sub>b1</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del fascio al diaframma nella 1<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"   <td><b>a<sub>b3</sub></b> [mm]</td><td style=\"text-align: left;\">Diametro del fascio al diaframma nella 3<sup>a</sup> condizione di misura</td>"
"    </tr>"
"    <tr>"
"    <td  style=\"text-align: left;\" colspan=\"2\" rowspan=\"1\"><i><br>Condizioni meteo (riff. CEI EN 60825-1 1998; SMD-W-001 2017).</i></td>"
"    </tr>"
"    <tr>"
"    <td><b>&mu;</b></td><td style=\"text-align: left;\">Coefficiente di attenuazione atmosferica ad una data lunghezza d'onda &lambda;</td>"
"    </tr>"
"    <tr>"
"    <td><b>V</b>[km]</td><td style=\"text-align: left;\">Distanza di visibilit&agrave;</td>"
"    </tr>"
"    <tr>"
"    <td><b>A</b></td><td style=\"text-align: left;\">Coefficiente di valore pari a 0,585 V<sup>0,33</sup>"
"    </tr>"
"    </tbody>"
"    </table>";

    return html;
}
