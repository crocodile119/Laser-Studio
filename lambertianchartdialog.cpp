#include "lambertianchartdialog.h"
#include "ui_lambertianchartdialog.h"
#include "lambertianchartview.h"
#include <QDebug>

LambertianChartDialog::LambertianChartDialog(Reflector *reflector, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::LambertianChartDialog)
{
    ui->setupUi(this);

    this->reflector=reflector;

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
        ui->kindLabel->setText("Puntiforme");

        bool DNRO_scientNot;
        DNRO_scientNot=reflector->getOpticalDiameter()>1.0e+03;

        if(DNRO_scientNot)
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'e', 2));
        else
            ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'f', 1));

        ui->distanceLabel->setText(QString::number(reflector->getPositioningElement(),'e', 2));
        ui->spotDiameterLabel->setText(QString::number(reflector->getSpotDiameter(),'e', 2));
        ui->alphaLabel->setText(QString::number(reflector->getAlpha(),'e', 2));
        ui->constPosLabel->setText(QString::number(reflector->getPositioning()));
        ui->label_34->setText("Posizionamento [gradi]");

        setWidgetsVisible(false);
        }
            else
            {
            ui->kindLabel->setText("Estesa");

            bool DNRO_scientNot;
            DNRO_scientNot=reflector->getOpticalDiameter()>1.0e+03;
            if(DNRO_scientNot)
                ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'e', 2));
            else
                ui->DNRO_Label->setText(QString::number(reflector->getOpticalDiameter(),'f', 1));

            ui->distanceLabel->setText(QString::number(reflector->getReflectorHazardDistance(),'e', 2));

            setWidgetsVisible(true);
            ui->constPosLabel->setText(QString::number(reflector->getConstant(),'e', 2));
            ui->label_34->setText("costante:");

            ui->spotDiameterLabel->setText(QString::number(reflector->getSpotDiameter(),'e', 2));
            ui->label_36->setText("Diametro spot [mm]");            

            ui->alphaLabel->setText(QString::number(reflector->getAlpha(),'e', 2));
            ui->label_37->setText("α [mrad]");

            ui->CE_Label->setText(QString::number(reflector->getCE()));
            ui->label_38->setText("CE");

            ui->surfaceLabel->setText(reflector->getKindOfSurface());          
            ui->label_39->setText("Tipo di superficie");

            ui->newRapLabel->setText(QString::number(reflector->getNewRapSolution()));
            ui->label_40->setText("Risultato NewRap");

            ui->indicatorLabel->setText(QString::number(reflector->getAlphaIndicator()));
            ui->label_41->setText("Indicatore α");
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
    QtCharts::QPolarChart *exendedReflection = new QtCharts::QPolarChart();
    double hazardDistance = reflector->getReflectorHazardDistance();
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    series->append(90, 0);
    series->append(90, hazardDistance);
    series->append(100, hazardDistance);
    series->append(110, hazardDistance);
    series->append(120, hazardDistance);
    series->append(130, hazardDistance);
    series->append(140, hazardDistance);
    series->append(150, hazardDistance);
    series->append(160, hazardDistance);
    series->append(170, hazardDistance);
    series->append(180, hazardDistance);
    series->append(190, hazardDistance);
    series->append(200, hazardDistance);
    series->append(210, hazardDistance);
    series->append(220, hazardDistance);
    series->append(230, hazardDistance);
    series->append(240, hazardDistance);
    series->append(250, hazardDistance);
    series->append(260, hazardDistance);
    series->append(270, hazardDistance);
    series->append(270, 0);

    series->setName("Distanza di rischio riflessione estesa su superficie lambertiana [m]");
    exendedReflection->addSeries(series);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%.1e");
    radialAxis->setRange(0, 1.2*hazardDistance);
    exendedReflection->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    angularAxis->setRange(0, 360);
    exendedReflection->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);
    exendedReflection->setMinimumSize(640, 480);

    QPen pen (QColor::fromRgb(0, 216, 0), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    series->setPen(pen);
    chartView = new QChartView(exendedReflection);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayout->addWidget(chartView, 1, 0, Qt::AlignCenter);
}

void LambertianChartDialog::setPointDiffusionChart()
{
    polarChartView = new LambertianChartView(0, dataVector, maxElement, correctPositioning);
    polarChartView->setRenderHint(QPainter::Antialiasing);
    //polarChartView->setDistance(distance);
    //polarChartView->distanceDataSerie();
    ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);
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
    ui->label_38->setVisible(visible);
/*
    ui->spotDiameterLabel->setVisible(visible);
    ui->label_36->setVisible(visible);
*/
    ui->alphaLabel->setVisible(visible);
    ui->label_37->setVisible(visible);

    ui->CE_Label->setVisible(visible);
    ui->label_39->setVisible(visible);

    ui->surfaceLabel->setVisible(visible);
    ui->label_40->setVisible(visible);

    ui->newRapLabel->setVisible(visible);
    ui->label_41->setVisible(visible);
}
