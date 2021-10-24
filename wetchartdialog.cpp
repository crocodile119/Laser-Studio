#include "wetchartdialog.h"
#include "ui_wetchartdialog.h"
#include "wetchartview.h"
#include <QDebug>

WetChartDialog::WetChartDialog(Reflector *reflector, bool _thema, QWidget *parent)
    : QDialog(parent)
      , ui(new Ui::WetChartDialog)
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
    positioning=reflector->getPositioning();
    correctPositioning=reflector->getCorrectPositioning();
    positioningElement=reflector->getPositioningElement();

    double maxReflectorDistance=reflector->getMaxElement();
    if(maxReflectorDistance>0)
    {
        polarChartView = new WetChartView(0, dataVector, maxElement, correctPositioning);
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
        polarChartView->setRenderHint(QPainter::Antialiasing);

        ui->gridLayout->addWidget(polarChartView, 1, 0, Qt::AlignCenter);
        resize(sizeHint());
    }
    else
    {
        ui->chartWidget->setVisible(false);
        resize(400,200);
    }

    setLabelDistance();
    setLabelDivergence();
    setLabelDNRO();
    setLabelRefraction();
    setLabelHazard();
    setLabelCorrectPositioning();

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
    ui->distanceLabel->setText(QString::number(distance,'e', 2));
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

void WetChartDialog::setLabelCorrectPositioning()
{
    double positiveCorrectPositioning;
    if(correctPositioning<0)
        positiveCorrectPositioning=correctPositioning+360.0;
    else
        positiveCorrectPositioning=correctPositioning;

    ui->correctPositioningLabel->setText(QString::number(positiveCorrectPositioning, 'f', 1));
}

void WetChartDialog::on_pushButton_clicked()
{
    close();
}
