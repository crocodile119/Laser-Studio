#include "lambertianchartdialog.h"
#include "ui_lambertianchartdialog.h"
#include "lambertianchartview.h"
#include <QDebug>
#include <cmath>

LambertianChartDialog::LambertianChartDialog(Reflector *reflector, bool _thema, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::LambertianChartDialog)
{
    ui->setupUi(this);

    this->reflector=reflector;
    thema=_thema;
    distance=reflector->getReflectorDistance();
    dnro=reflector->getOpticalDiameter();
    divergence=reflector->getDivergence();
    refraction=reflector->getRefration_n();
    dataVector=reflector->getZsVect();
    maxElement=reflector->getMaxElement();

    laserBeamDiameter=reflector->getLaserBeamDiameter();
    laserEMP=reflector->getLaserEMP();
    laserPowerErg=reflector->getLaserPowerErg();
    positioning=reflector->getPositioning();
    positioningElement=reflector->getPositioningElement();
    correctPositioning=reflector->getCorrectPositioning();

    /**************************************************************************************************************************
    * Nell'ipotesi di reflessioni diffuse da superficie non puntiforme l'angolo alpha non può essere maggiore di PI_GRECO     *
    * (corrispondente a metà angolo solido), quindi l'argomento del coseno è al più pari a PI_GRECO/2.                        *
    * Ciò significa che:                                                                                                      *
    *   - quando l'angolo alpha è uguale a PI_GRECO, la funzione x/(1-cos(x/2)) assume valore PI_GRECO;                       *
    *   - la funzione ha un asintoto verticale in 0 per + infinito;                                                           *
    *   - la funzione è inoltre crescente.                                                                                    *
    * Ne consegue che affinchè possa essere applicata l'equazione per il calcolo della diffusione da superficie estesa è      *
    * necessario che l'angolo ricavato sia maggiore o al più uguale a PI_GRECO in caso contrario l'algoritmo per il             *
    * calcolo della soluzione certamente non converge qualsiasi sia il valore di prova.                                       *                                                                        *
    ***************************************************************************************************************************/

    setLabelDNRO();

    if(!reflector->isExendedDiffusion())
    {
        setPointDiffusionChart();

        if(reflector->getPositioningElement()>0)
            ui->kindLabel->setText("Puntiforme");
        else
            ui->kindLabel->setText("Non pericolosa");

        bool DNRO_scientNot;
        DNRO_scientNot=reflector->getOpticalDiameter()>1.0e+03;

        if(DNRO_scientNot)
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'e', 2));
        else
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'f', 1));

        ui->opticalDistanceLabel->setText(QString::number(reflector->getPositioningElement(),'e', 2));
        ui->spotDiameterLabel->setText(QString::number(reflector->getSpotDiameter(),'e', 2));
        ui->alphaLabel->setText(QString::number(reflector->getAlpha(),'e', 2));

        double positiveCorrectPositioning;
        if(reflector->getCorrectPositioning()<0)
            positiveCorrectPositioning=round(reflector->getCorrectPositioning()+360.0);
        else
            positiveCorrectPositioning=reflector->getCorrectPositioning();

        ui->constPosLabel->setText(QString::number(positiveCorrectPositioning, 'f', 1));
        ui->tConstPosLabel->setText("&psi;<sub>R</sub> [gradi]");

        setWidgetsVisible(false);
    }
    else
    {
        if(reflector->getReflectorHazardDistance()>0)
            ui->kindLabel->setText("Estesa");
        else
            ui->kindLabel->setText("Non pericolosa");

        bool DNRO_scientNot;

        DNRO_scientNot=reflector->getOpticalDiameter()>1.0e+03;
        if(DNRO_scientNot)
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'e', 2));
        else
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'f', 1));

        ui->opticalDistanceLabel->setText(QString::number(reflector->getReflectorHazardDistance(),'e', 2));

        setWidgetsVisible(true);
        ui->constPosLabel->setText(QString::number(reflector->getConstant(),'e', 2));
        ui->tConstPosLabel->setText("K");

        ui->spotDiameterLabel->setText(QString::number(reflector->getSpotDiameter(),'e', 2));
        ui->tSpotDiameterLabel->setText("d<sub>b</sub> [mm]");

        ui->alphaLabel->setText(QString::number(reflector->getAlpha(),'e', 2));
        ui->tAlphaLabel->setText("α<sub>d</sub> [mrad]");

        ui->CE_Label->setText(QString::number(reflector->getCE()));
        ui->tCE_Label->setText("C<sub>E</sub>");

        ui->surfaceLabel->setText(reflector->getKindOfSurface());
        ui->tSurfaceLabel->setText("Tipo di superficie");

        ui->newRapLabel->setText(QString::number(reflector->getNewRapSolution()));
        ui->tNewRapLabel->setText("x<sub>0</sub>");

        ui->indicatorLabel->setText(QString::number(reflector->getAlphaIndicator()));
        ui->tIndicatorLabel->setText("α<sub>i</sub>");
        //Per questo grafico non occore una nuova classe basta una funzione
        setExendedDiffusionChart();
    }
}

LambertianChartDialog::~LambertianChartDialog()
{
    delete ui;
}

double LambertianChartDialog::getRefraction()const
{
    return refraction;
}

void LambertianChartDialog::setRefraction(const double _refraction)
{
    refraction=_refraction;
}

double LambertianChartDialog::getDNRO() const
{
    return dnro;
}

double LambertianChartDialog::getDistance()const
{
    return distance;
}

double LambertianChartDialog::getDivergence()const
{
    return divergence;
}

void LambertianChartDialog::setLabelDNRO()
{
    bool DNRO_scientNot;
    DNRO_scientNot=dnro>1.0e+03;
    if(DNRO_scientNot)
        ui->DNRO_Label->setText(QString::number(dnro,'e', 2));
    else
        ui->DNRO_Label->setText(QString::number(dnro,'f', 1));
}

void LambertianChartDialog::setExendedDiffusionChart()
{
    double reflectorDistance=reflector->getReflectorHazardDistance();

    exendedChartView = new ExendedChartView(0, reflectorDistance);
    exendedChartView->setRenderHint(QPainter::Antialiasing);
    if(thema)
    {
        exendedChartView->setStyleSheet("QGraphicsView{background-color:#fafafa;}");
        exendedChartView->setChartBackgroundBrush(QColor::fromRgb(250, 250, 250));
    }
    else
    {
        exendedChartView->setStyleSheet("QGraphicsView{background-color:#ffffff;}");
        exendedChartView->setChartBackgroundBrush(QColor::fromRgb(255, 255, 255));
    }

    ui->gridLayout->addWidget(exendedChartView, 1, 0, Qt::AlignTop);
    resize(sizeHint());
}

void LambertianChartDialog::setPointDiffusionChart()
{
    if(maxElement>0)
    {
        polarChartView = new LambertianChartView(0, dataVector, maxElement, correctPositioning);
        polarChartView->setRenderHint(QPainter::Antialiasing);        
        if(thema)
        {
            polarChartView->setStyleSheet("QGraphicsView{background-color:#fafafa;}");
            polarChartView->setChartBackgroundBrush(QColor::fromRgb(250, 250, 250));
        }
        else
        {
            polarChartView->setStyleSheet("QGraphicsView{background-color:#ffffff;}");
            polarChartView->setChartBackgroundBrush(QColor::fromRgb(255, 255, 255));
        }
        ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignTop);
        resize(sizeHint());
    }
    else
    {
        ui->chartWidget->setVisible(false);
        resize(400,200);
    }
}

LambertianChartView* LambertianChartDialog::getLambertianPointChart()
{
    return polarChartView;
}

QChartView* LambertianChartDialog::getLambertianExendedChart()
{
    return chartView;
}

void LambertianChartDialog::setWidgetsVisible(bool visible)
{
    ui->indicatorLabel->setVisible(visible);
    ui->tIndicatorLabel->setVisible(visible);

    ui->alphaLabel->setVisible(visible);
    ui->tAlphaLabel->setVisible(visible);

    ui->CE_Label->setVisible(visible);
    ui->tCE_Label->setVisible(visible);

    ui->surfaceLabel->setVisible(visible);
    ui->tSurfaceLabel->setVisible(visible);

    ui->newRapLabel->setVisible(visible);
    ui->tNewRapLabel->setVisible(visible);
}

void LambertianChartDialog::on_pushButton_clicked()
{
    close();
}
