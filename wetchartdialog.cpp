#include "wetchartdialog.h"
#include "ui_wetchartdialog.h"
#include "wetchartview.h"
#include <QDebug>

WetChartDialog::WetChartDialog(Reflector *reflector, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::WetChartDialog)
{
    ui->setupUi(this);

    this->reflector=reflector;

    distance=reflector->getReflectorDistance();
    dnro=reflector->getOpticalDiameter();
    divergence=reflector->getDivergence();
    refraction=reflector->getRefration_n();
    dataVector=reflector->getZsVect();
    maxElement=reflector->getMaxElement();
    positioning=reflector->getPositioning();
    correctPositioning=reflector->getCorrectPositioning();
    positioningElement=reflector->getPositioningElement();

    polarChartView = new WetChartView(0, dataVector, maxElement, correctPositioning);

    polarChartView->setRenderHint(QPainter::Antialiasing);
    //polarChartView->setDistance(distance);
    //polarChartView->distanceDataSerie();

    ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);

    setLabelDistance();
    setLabelDivergence();
    setLabelDNRO();
    setLabelRefraction();
    setLabelHazard();
    setLabelPositioning();

    qDebug() << "Divergenza: " << divergence;
    qDebug() << "Distanza: " << distance;
    qDebug() << "DNRO: " << dnro;
}

WetChartDialog::~WetChartDialog()
{
    delete ui;
}

double WetChartDialog::getRefraction()const
{
    return refraction;
}
/*********************************************
* L'indice di rifrazione va impostato a 1,33 *
**********************************************/

void WetChartDialog::setRefraction(const double _refraction)
{
    refraction=_refraction;
}

double WetChartDialog::getDNRO() const
{
    return dnro;
}

double WetChartDialog::getDistance()const
{
    return distance;
}

double WetChartDialog::getDivergence()const
{
    return divergence;
}

void WetChartDialog::setLabelDistance()
{
    ui->distanceLabel->setText(QString::number(distance));
}

void WetChartDialog::setLabelDNRO()
{
    bool DNRO_scientNot;
    DNRO_scientNot=dnro>1.0e+03;
    if(DNRO_scientNot)
        ui->DNRO_Label->setText(QString::number(dnro,'e', 2));
    else
        ui->DNRO_Label->setText(QString::number(dnro,'f', 1));
}

void WetChartDialog::setLabelDivergence()
{
    ui->divergenceLabel->setText(QString::number(divergence, 'e', 2));
}

void WetChartDialog::setLabelRefraction()
{
    ui->refractionLabel->setText(QString::number(refraction, 'e', 2));
}

void WetChartDialog::setLabelHazard()
{
    ui->hazardLabel->setText(QString::number(positioningElement, 'e', 2));
}

void WetChartDialog::setLabelPositioning()
{
    ui->positioningLabel->setText(QString::number(positioning));
}
