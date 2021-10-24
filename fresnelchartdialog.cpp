#include "fresnelchartdialog.h"
#include "ui_fresnelchartdialog.h"
#include "fresnelchartview.h"
#include <cmath>
#include <QDebug>

FresnelChartDialog::FresnelChartDialog(Reflector *reflector, bool _thema, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::FresnelChartDialog)
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

    correctPositioning=reflector->getCorrectPositioning();
    positioning=reflector->getPositioning();
    positioningElement=reflector->getPositioningElement();

    if(maxElement>0)
    {
        polarChartView = new FresnelChartView(0, dataVector, maxElement, correctPositioning);
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

        polarChartView->setDistance(distance);
        ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);

        polarChartView->setRenderHint(QPainter::Antialiasing);
        ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);
    }
    else
    {
        ui->chartWidget->setVisible(false);
        resize(400,200);
    }
    setLabelDistance();
    setLabelDNRO();
    setLabelRefraction();
    setLabelHazard();
    setLabelCorrectPositioning();

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
    ui->distanceLabel->setText(QString::number(distance, 'e', 2));
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

void FresnelChartDialog::setLabelCorrectPositioning()
{
    double positiveCorrectPositioning;
    if(correctPositioning<0)
        positiveCorrectPositioning=correctPositioning+360.0;
    else
        positiveCorrectPositioning=correctPositioning;

    ui->correctPositioningLabel->setText(QString::number(positiveCorrectPositioning, 'f', 1));
}

void FresnelChartDialog::on_pushButton_clicked()
{
    close();
}
