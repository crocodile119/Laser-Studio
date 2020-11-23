#include "fresnelchartdialog.h"
#include "ui_fresnelchartdialog.h"
#include "fresnelchartview.h"
#include <QDebug>

FresnelChartDialog::FresnelChartDialog(Reflector *reflector, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::FresnelChartDialog)
{
    ui->setupUi(this);

    this->reflector=reflector;

    distance=reflector->getReflectorDistance();
    dnro=reflector->getOpticalDiameter();
    divergence=reflector->getDivergence();
    refraction=reflector->getRefration_n();
    dataVector=reflector->getZsVect();
    maxElement=reflector->getMaxElement();

    correctPositioning=reflector->getCorrectPositioning();
    positioning=reflector->getPositioning();
    positioningElement=reflector->getPositioningElement();

    polarChartView = new FresnelChartView(0, dataVector, maxElement, correctPositioning);
    polarChartView->setRenderHint(QPainter::Antialiasing);
    polarChartView->setDistance(distance);
    ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);

    polarChartView->setRenderHint(QPainter::Antialiasing);
    //polarChartView->setDistance(distance);
    //polarChartView->distanceDataSerie();

    ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);

    setLabelDistance();
    setLabelDNRO();
    setLabelRefraction();
    setLabelHazard();
    setLabelPositioning();

    qDebug() << "Divergenza: " << divergence;
    qDebug() << "Distanza: " << distance;
    qDebug() << "DNRO: " << dnro;
}

FresnelChartDialog::~FresnelChartDialog()
{
    delete ui;
}

double FresnelChartDialog::getDistance() const
{
    return distance;
}

double FresnelChartDialog::getDNRO() const
{
    return dnro;
}

double FresnelChartDialog::getRefraction()const
{
    return refraction;
}

void FresnelChartDialog::setRefraction(const double _refraction)
{
    refraction=_refraction;
}

void FresnelChartDialog::setLabelDistance()
{
    ui->distanceLabel->setText(QString::number(distance));
}

void FresnelChartDialog::setLabelDNRO()
{
    bool DNRO_scientNot;
    DNRO_scientNot=dnro>1.0e+03;
    if(DNRO_scientNot)
        ui->DNRO_Label->setText(QString::number(dnro, 'e', 2));
    else
        ui->DNRO_Label->setText(QString::number(dnro, 'f', 1));
}

void FresnelChartDialog::setLabelRefraction()
{
    ui->refractionLabel->setText(QString::number(refraction, 'e', 2));
}

void FresnelChartDialog::setLabelHazard()
{
    ui->hazardLabel->setText(QString::number(positioningElement, 'e', 2));
}

void FresnelChartDialog::setLabelPositioning()
{
    ui->positioningLabel->setText(QString::number(positioning));
}
