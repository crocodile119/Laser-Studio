#include <QtGui>
#include <QColorDialog>

#include "reflectorpropertiesdialog.h"

double ReflectorPropertiesDialog::MAX_DEFLECTION=50.0;

ReflectorPropertiesDialog::ReflectorPropertiesDialog(Reflector *_reflector, bool _dark, QWidget *parent)
    : QDialog(parent), ui(new Ui::ReflectorPropertiesDialog), reflector(_reflector),dark(_dark)
{
    ui->setupUi(this);

    ui->xSpinBox->setValue(reflector->x());
    ui->ySpinBox->setValue(reflector->y());
    ui->reflectionSpinBox->setValue(reflector->getMaterialCoeff());
    ui->reflectorKindLabel->setText(reflector->getReflectorKindString());
    ui->positioningSlider->setValue(reflector->getPositioning());
    ui->laserPhaseLabel->setText(QString::number(reflector->getLaserPhase(), 'f', 1));

    setStyleSheet();
    semiLaserAperture=reflector->retrieveLaserAperture()/2;

    lambertian =reflector->getReflectorKind()==LAMBERTIAN_TARGET;
    QString correctPositioning=QString::number(reflector->getCorrectPositioning(), 'f', 1);
    ui->actualPositioningLabel->setText(correctPositioning);

    ui->maxDeflectionLabel->setVisible(lambertian);
    if(lambertian)
        ui->maxDeflectionLabel->setText(QString("ψ max ±%1°").arg(QString::number(MAX_DEFLECTION-semiLaserAperture)));

    qDebug()<<"Fase laser: "<<reflector->getLaserPhase();
    ui->descriptionTextEdit->setText(reflector->getDescription());

    if(this->reflector->getReflectorKind()==MIRROR_TARGET)
    {
        ui->positioningSlider->setVisible(false);
        ui->laserPhaseLabel->setVisible(false);
        ui->actualPositioningLabel->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->positioningLabel->setVisible(false);
        ui->ZRLcheckBox->setVisible(false);
    }
    else
    {
        if(this->reflector->getReflectorKind()==GLASS_TARGET)
            ui->label_5->setText(tr("τ"));
        else
            ui->label_5->setText(tr("ρ"));

        ui->ZRLcheckBox->setChecked(reflector->isZRLShown());
    }
}

void ReflectorPropertiesDialog::on_positioningSlider_valueChanged(int value)
{
    QPointF position=QPointF(ui->xSpinBox->value(), ui->ySpinBox->value());
    QLineF laserJoin=QLineF(reflector->getLaserPosition(), position);

    double laserPhase;

    if(laserJoin.dy()!=0)
        laserPhase=asin(laserJoin.dy()/laserJoin.length())*180.0/DiffusionHazard::PI_GRECO;
    else
        laserPhase=0.0;

    if(lambertian)
    {
        if(semiLaserAperture-value>MAX_DEFLECTION)
            value=semiLaserAperture-MAX_DEFLECTION;
        else if(-semiLaserAperture-value<-MAX_DEFLECTION)
            value=MAX_DEFLECTION-semiLaserAperture;

        ui->actualPositioningLabel->setText(QString::number(laserPhase-value, 'f', 1));
    }
    else
        ui->actualPositioningLabel->setText(QString::number(2*(laserPhase-value), 'f', 1));



    ui->positioningLabel->setText(QString::number(value));
    ui->laserPhaseLabel->setText(QString::number(laserPhase, 'f', 1));
}

ReflectorPropertiesDialog::~ReflectorPropertiesDialog()
{
    delete ui;
}

void ReflectorPropertiesDialog::on_xSpinBox_valueChanged(double arg1)
{
    QPointF position=QPointF(arg1, ui->ySpinBox->value());
    QLineF laserJoin=QLineF(reflector->getLaserPosition(), position);
    double laserPhase;

    if(laserJoin.dy()!=0)
        laserPhase=asin(laserJoin.dy()/laserJoin.length())*180.0/DiffusionHazard::PI_GRECO;
    else
        laserPhase=0.0;

    ui->laserPhaseLabel->setText(QString::number(laserPhase, 'f', 1));

    if(reflector->getReflectorKind()==LAMBERTIAN_TARGET)
        ui->actualPositioningLabel->setText(QString::number(laserPhase-reflector->getPositioning(), 'f', 1));
    else
        ui->actualPositioningLabel->setText(QString::number(2*(laserPhase-reflector->getPositioning()), 'f', 1));
}

void ReflectorPropertiesDialog::on_ySpinBox_valueChanged(double arg1)
{
    QPointF position=QPointF(ui->xSpinBox->value(), arg1);
    QLineF laserJoin=QLineF(reflector->getLaserPosition(), position);
    double laserPhase;

    if(laserJoin.dy()!=0)
        laserPhase=asin(laserJoin.dy()/laserJoin.length())*180.0/DiffusionHazard::PI_GRECO;
    else
        laserPhase=0.0;

    ui->laserPhaseLabel->setText(QString::number(laserPhase, 'f', 1));

    if(lambertian)
        ui->actualPositioningLabel->setText(QString::number(reflector->getPositioning()+laserPhase, 'f', 1));
    else
        ui->actualPositioningLabel->setText(QString::number(2*(reflector->getPositioning()+laserPhase), 'f', 1));
}

void ReflectorPropertiesDialog::setStyleSheet()
{
    if(dark)
    {
        ui->label->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_2->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_3->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_6->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_7->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_8->setStyleSheet(tr("QLabel {background-color: #b5922d}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
    else
    {
        ui->label->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_2->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_3->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_4->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_5->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_6->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_7->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
        ui->label_8->setStyleSheet(tr("QLabel {background-color: #00c800}"
                "QLabel {color: #fafafa}"
                "QLabel {border: 0px solid grey}"
                "QLabel {border-radius: 8px}"
                "QLabel {padding: 3px}"
                "QLabel {margin-left: 10px}"));
    }
}

void ReflectorPropertiesDialog::on_ZRLcheckBox_clicked(bool checked)
{
        reflector->setZRL(checked);
}
