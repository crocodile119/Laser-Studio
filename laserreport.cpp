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
        output.append(laserWindow->myDockResults->ui->tNOHDLabel->text()+"$"+laserWindow->myDockResults->ui->NOHDLabel->text());
        output.append(laserWindow->myDockResults->ui->tOD_FilterLabel->text()+"$"+laserWindow->myDockResults->ui->tOD_FilterLabel->text());

        skin.append(laserWindow->myDockSkin->ui->conditions_Label->text()+"$"+laserWindow->myDockSkin->ui->EMP_1st_Label->text());
        skin.append("Dettaglio risultati:$ ");
        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tNSHDLabel->text()+"$"+laserWindow->myDockSkin->ui->NSHDLabel->text());

        goggle.append(laserWindow->myDockGoggle->ui->kindOfLaserLabel->text()+": ");
        goggle.append(laserWindow->myDockGoggle->ui->tLaserOutputLabel->text()+":"+laserWindow->myDockGoggle->ui->laserOutputLabel->text());
        goggle.append("Numero di scala: " + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());

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
        output.append(laserWindow->myDockResults->ui->tNOHDLabel->text()+"$"+laserWindow->myDockResults->ui->NOHDLabel->text());
        output.append(laserWindow->myDockResults->ui->tOD_FilterLabel->text()+"$"+laserWindow->myDockResults->ui->tOD_FilterLabel->text());

        skin.append(laserWindow->myDockSkin->ui->conditions_Label->text()+"$"+laserWindow->myDockSkin->ui->EMP_1st_Label->text());
        skin.append("Dettaglio risultati$ ");

        skin.append(laserWindow->myDockSkin->ui->tFormulaSkinLabel->text()+"$"+laserWindow->myDockSkin->ui->FormulaSkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tEMP_SkinLabel->text()+"$"+laserWindow->myDockSkin->ui->EMP_SkinLabel->text());
        skin.append(laserWindow->myDockSkin->ui->tNSHDLabel->text()+"$"+laserWindow->myDockSkin->ui->NSHDLabel->text());

        goggle.append(laserWindow->myDockGoggle->ui->kindOfLaserLabel->text()+": ");
        goggle.append("Output: "+laserWindow->myDockGoggle->ui->laserOutputLabel->text());
        goggle.append("Numero di scala: " + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());
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

        output.append(laserWindow->myDockResults->ui->tMeanPowerLabel->text()+"$"+laserWindow->myDockResults->ui->MeanPowerLabel->text());
        output.append(laserWindow->myDockResults->ui->tMeanIrradianceLabel->text()+"$"+laserWindow->myDockResults->ui->MeanIrradianceLabel->text());
        output.append(laserWindow->myDockResults->ui->tThermalEMP_Label->text()+"$"+laserWindow->myDockResults->ui->ThermalEMP_Label->text());
        output.append(laserWindow->myDockResults->ui->tEMP_mean_Label->text()+"$"+laserWindow->myDockResults->ui->tEMP_mean_Label->text());
        output.append(laserWindow->myDockEffects->ui->tTminLabel->text()+"$"+laserWindow->myDockEffects->ui->TminLabel->text());

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
        goggle.append("Numero di scala:" + laserWindow->myDockGoggle->ui->scaleNumberLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tn_maxLabel->text()+":"+laserWindow->myDockGoggle->ui->n_maxLabel->text());
        goggle.append(laserWindow->myDockGoggle->ui->tMeanPowerLabel->text()+":"+laserWindow->myDockGoggle->ui->meanPowerLabel->text());
        goggle.append("Numero di scala componente continua: " + laserWindow->myDockGoggle->ui->scaleNumberDLabel->text());
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

    QString couplingFactor1str= "Fattore di accoppiamento 1<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor1_Label->text();
    QString couplingFactor3str= "Fattore di accoppiamento 3<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor3_Label->text();
    QString apertureDiam1str= "Diametro diaframma 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam1_Label->text();
    QString apertureDiam3str=  "Diametro diaframma 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam3_Label->text();
    QString apertureDist1str=  "Distanza apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist1_Label->text();
    QString apertureDist3str=  "Diametro apertura 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist3_Label->text();
    QString beamAperture1str=  "Dimensione del fascio all'apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->beamAperture1_Label->text();
    QString beamAperture3str= "Dimensione del fascio all'apertura 3<sup>a</sup> condizione :"+ laserWindow->myDockLea->ui->beamAperture3_Label->text();
    QString formulaLEAstr= laserWindow->myDockLea->ui->tFormulaLEA_Label->text() + " :" + laserWindow->myDockLea->ui->formulaLEA_Label->text();
    QString LEAstr= laserWindow->myDockLea->ui->tLEA_Label->text() + " :" + laserWindow->myDockLea->ui->LEA_Label->text();
    QString powerErgCond1LEAstr= laserWindow->myDockLea->ui->tCond1LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond1LEA_Label->text();
    QString powerErgCond3LEAstr= laserWindow->myDockLea->ui->tCond3LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond3LEA_Label->text();
    QString classStr= "Classe sistema :" + laserWindow->myDockLea->ui->class_Label->text();

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

    if(laserWindow->myDockControls->laserOperation()==DockControls::operation::PULSE)
    {
        QString timeBaseStr ="Base dei tempi :" + laserWindow->myDockLea->ui->timeBase_Label->text();
        classifierResultsOutput.append(timeBaseStr);
    }

    else if(laserWindow->myDockControls->laserOperation()==DockControls::operation::MULTI_PULSE)
    {
        QString meanPowerEffects="Criterio della potenza media : ";
        QString meanPowerLabelStr= laserWindow->myDockLea->ui->tCond3LEA_Label->text() + " :" + laserWindow->myDockLea->ui->MeanPowerLabel->text();
        QString Te_Str= laserWindow->myDockLea->ui->tTe_Label->text() + " :" + laserWindow->myDockLea->ui->Te_Label->text();

        QString couplingFactor1str_2= "Fattore di accoppiamento 1<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor1_Label_2->text();
        QString couplingFactor3str_2= "Fattore di accoppiamento 3<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor3_Label_2->text();
        QString apertureDiam1str_2= "Diametro diaframma 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam1_Label_2->text();
        QString apertureDiam3str_2=  "Diametro diaframma 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam3_Label_2->text();
        QString apertureDist1str_2=  "Distanza apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist1_Label_2->text();
        QString apertureDist3str_2=  "Diametro apertura 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist3_Label_2->text();
        QString beamAperture1str_2=  "Dimensione del fascio all'apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->beamAperture1_Label_2->text();
        QString beamAperture3str_2= "Dimensione del fascio all'apertura 3<sup>a</sup> condizione :"+ laserWindow->myDockLea->ui->beamAperture3_Label_2->text();
        QString formulaLEAstr_2= laserWindow->myDockLea->ui->tFormulaLEA_Label->text() + " :" + laserWindow->myDockLea->ui->FormulaLEA_Label_2->text();
        QString LEAstr_2= laserWindow->myDockLea->ui->tLEA_Label->text() + " :" + laserWindow->myDockLea->ui->LEA_Label_2->text();
        QString powerErgCond1LEAstr_2= laserWindow->myDockLea->ui->tCond1LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond1LEA_Label_2->text();
        QString powerErgCond3LEAstr_2= laserWindow->myDockLea->ui->tCond3LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond3LEA_Label_2->text();
        QString classStr_2= "Classe sistema :" + laserWindow->myDockLea->ui->class_Label->text();

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
            QString thermalEffects="Criterio dell'impulso effetti termici : ";
            QString couplingFactor1str_3= "Fattore di accoppiamento 1<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor1_Label_3->text();
            QString couplingFactor3str_3= "Fattore di accoppiamento 3<sup>a</sup> condizione :" +  laserWindow->myDockLea->ui->couplingFactor3_Label_3->text();
            QString apertureDiam1str_3= "Diametro diaframma 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam1_Label_3->text();
            QString apertureDiam3str_3=  "Diametro diaframma 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDiam3_Label_3->text();
            QString apertureDist1str_3=  "Distanza apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist1_Label_3->text();
            QString apertureDist3str_3=  "Diametro apertura 3<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->apertureDist3_Label_3->text();
            QString beamAperture1str_3=  "Dimensione del fascio all'apertura 1<sup>a</sup> condizione :" + laserWindow->myDockLea->ui->beamAperture1_Label_3->text();
            QString beamAperture3str_3= "Dimensione del fascio all'apertura 3<sup>a</sup> condizione :"+ laserWindow->myDockLea->ui->beamAperture3_Label_3->text();
            QString formulaLEAstr_3= laserWindow->myDockLea->ui->tFormulaLEA_Label->text() + " :" + laserWindow->myDockLea->ui->FormulaLEA_Label_3->text();
            QString LEAstr_3= laserWindow->myDockLea->ui->tLEA_Label->text() + " :" + laserWindow->myDockLea->ui->LEA_Label_3->text();
            QString powerErgCond1LEAstr_3= laserWindow->myDockLea->ui->tCond1LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond1LEA_Label_3->text();
            QString powerErgCond3LEAstr_3= laserWindow->myDockLea->ui->tCond3LEA_Label->text() + " :" + laserWindow->myDockLea->ui->cond3LEA_Label_3->text();

            QString Ti_Str= laserWindow->myDockLea->ui->tTi_Label->text() + " :" + laserWindow->myDockLea->ui->Ti_Label->text();
            QString Ti_prf_Str= laserWindow->myDockLea->ui->tTi_prf_Label->text() + " :" + laserWindow->myDockLea->ui->Ti_prf_Label->text();
            QString C5_Str= laserWindow->myDockLea->ui->tC5_Label->text() + " :" + laserWindow->myDockLea->ui->C5_Label->text();
            QString pulseNumberLabel_Str= laserWindow->myDockLea->ui->tPulseNumberLabel->text() + " :" + laserWindow->myDockLea->ui->PulseNumberLabel->text();
            QString countingLabel_Str= laserWindow->myDockLea->ui->tCountingLabel->text() + " :" + laserWindow->myDockLea->ui->CountingLabel->text();

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

    QString laserPositionStr= QString("Posizione [m,m] : (%1,%2)")
                                      .arg(laserpoint->pos().x())
                                      .arg(laserpoint->pos().y());

    double qualityFactor=BeamInspector::getQualityFactor();
    QString qualityFactorString="M<sup>2</sup>: " +QString::number(qualityFactor);

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

    reflectors.append("Posizione [m, m]: " + QString("(%1,%2)")
                          .arg(reflector->pos().x())
                          .arg(reflector->pos().y()));
    reflectors.append("Nell'area di rischio: "+ inHazardArea);
    reflectors.append("Tipo di riflettore: " + reflector->getReflectorKindString());
    reflectors.append("Divergenza laser [mrad]: " + QString::number(reflector->getDivergence()));
    reflectors.append("Distanza del riflettore [m]: " + QString::number(reflector->getReflectorDistance(),'f',1));
    reflectors.append("Coefficiente di riflessione: " + QString::number(reflector->getMaterialCoeff()));
    reflectors.append("Posizionamento [gradi]: " + QString::number(reflector->getPositioning()));
    reflectors.append("Distanza di sicurezza ottica dal riflettore [m]: "+ QString::number(reflector->getPositioningElement(),'f',1));

    if(reflector->getReflectorKind()!=MIRROR_TARGET)
        reflectors.append("Distanza di sicurezza ottica massima dal riflettore [m]: "+ QString::number(reflector->getMaxElement(),'f',1));
}

void LaserReport::binocularsValuation()
{
    double wavelength=laserWindow->myDockControls->getWavelength();
    if((wavelength>=400)&&(wavelength<=1400))
    {
        binoculars.append("Posizione [m, m]: " + QString("(%1,%2)")
                          .arg(binocular->pos().x())
                          .arg(binocular->pos().y()));
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("Distanza di Rischio Ottico Estesa [m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("Distanza dal punto laser [m]: " + QString::number(binocular->getBinocularDistance()));
        binoculars.append("Amplificazione ottica M: " + QString::number(binocular->getMagnification()));
        binoculars.append("Coefficiente di trasmissione ottica τ: " + QString::number(binocular->getTransmissionCoeff(),'f',1));
        binoculars.append("Diametro della lente D<sub>0</sub> [mm]: " + QString::number(binocular->get_D0()));
        binoculars.append("Diametro dello spot sulla lente D<sub>b</sub> [mm]: " + QString::number(binocular->get_Db()));
        binoculars.append("Guadagno ottico K: " + QString::number(binocular->getOpticalGain()));
        binoculars.append("Effetti nel'impiego: "+ binocular->getBinocularEffects());
     }
    else if(((wavelength>=320)&&(wavelength<400))||((wavelength>1400)&&(wavelength<=4500)))
    {
        binoculars.append("Posizione [m, m]: " + QString("(%1,%2)")
                          .arg(binocular->pos().x())
                          .arg(binocular->pos().y()));
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("Amplificazione ottica M: " + QString::number(binocular->getMagnification()));
        binoculars.append("Distanza di Rischio Ottico Estesa [m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("Distanza dal punto laser [m]: " + QString::number(binocular->getBinocularDistance()));
        binoculars.append("Amplificazione ottica M: " + QString::number(binocular->getMagnification()));
        binoculars.append("Coefficiente di trasmissione ottica τ: " + QString::number(binocular->getTransmissionCoeff(),'f',1));
        binoculars.append("Diametro dello spot sulla lente D<sub>b</sub> [mm]: " + QString::number(binocular->get_Db()));
        binoculars.append("Guadagno ottico K: " + QString::number(binocular->getOpticalGain()));
        binoculars.append("Effetti nel'impiego: "+ binocular->getBinocularEffects());
    }
    else
    {
        binoculars.append("Descrizione: " + binocular->getDescription());
        binoculars.append("Distanza di Rischio Ottico Estesa [m]: " + QString::number(binocular->getExendedOpticalDiameter()));
        binoculars.append("Distanza dal punto laser [m]: " + QString::number(binocular->getBinocularDistance()));
    }
}

void LaserReport::inspectorsValuation()
{
    inspectors.append("Posizione [m, m]: " + QString("(%1,%2)")
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
                inspectors.append(QString::fromStdString(beamInspector->getEMP_Sort())+
                                  QString::fromStdString(beamInspector->getEMP_Unit())+ ": "
                                + QString::number(beamInspector->getAugmentedEMP(),'e', 2));
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
    footprints.append("Posizione [m, m]: " + QString("(%1,%2)")
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

        html +="<tr><td bgcolor=\"#fbfbfb\"><b>" + title + "</b></td>\n"
                "<td>" + body + "</td></tr>\n";
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
        QString title = fields[0].toHtmlEscaped();
        QString body = fields[1].toHtmlEscaped();

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
        QList<pair<Reflector*, int>>::iterator myIterator; // iterator
        myIterator = myReflectors.begin();
        int i =1;

        while (myIterator != myReflectors.end() )
        {
            reflector=myIterator->first;
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
            htmlImage+="<h3>Coefficienti di riflessione da superficie di vetro</h3>\n";
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
        QString title = fields[0].toHtmlEscaped();
        QString body = fields[1].toHtmlEscaped();

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
        QList<pair<BeamInspector*, int>>::iterator myIterator; // iterator
        myIterator = myBeamInspectors.begin();
        int i =1;

        while (myIterator != myBeamInspectors.end() )
        {
            beamInspector=myIterator->first;
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
    QString htmlImage;

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

        QList<pair<Binocular*, int>>::iterator myIterator; // iterator
        myIterator = myBinoculars.begin();
        int i =1;

        while(myIterator != myBinoculars.end())
        {
            binocular=myIterator->first;
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

QString LaserReport::htmlClassifier()
{
    QString html;   

    html +="<br><h2>Valutazione della Classe secondo la procedura semplificata CEI EN 60825-1</h2>\n";

    QImage myLaserWarning = QImage("./laser_warning.png");
    QUrl Uri=QUrl("mydata://laser_warning.png");

    textDocument->addResource(QTextDocument::ImageResource,
        Uri, QVariant(myLaserWarning));

    QString classStr=laserWindow->myDockLea->ui->class_Label->text();

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

void LaserReport::setReflectorsList(const QList<pair<Reflector*, int>> &_myReflectors)
{
    myReflectors=_myReflectors;
}

void LaserReport::setFootprintsList(const QList<FootprintObject*> &_myFootprints)
{
   myFootprints=_myFootprints;
}

void LaserReport::setBinocularsList(const QList<pair<Binocular*, int>> &_myBinoculars)
{
    myBinoculars=_myBinoculars;
}

void LaserReport::setBeamInspectorsList(const QList<pair<BeamInspector*, int>> &_myBeamInspectors)
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
