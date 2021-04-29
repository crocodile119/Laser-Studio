#include "centralwidget.h"
#include <QtGui>
#include <QFile>
#include <QMessageBox>
#include <QtDebug>
#include <cmath>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QGraphicsRectItem>
#include "dockeffects.h"
#include "dockresults.h"
#include "dockskin.h"
#include "dockgoggle.h"
#include "dockcontrols.h"
#include "docklea.h"
#include "ui_dockeffects.h"
#include "ui_dockresults.h"
#include "ui_dockskin.h"
#include "ui_dockgoggle.h"
#include "ui_dockcontrols.h"
#include "ui_docklea.h"
#include "laserpoint.h"
#include "binocular.h"
#include "footprintobject.h"

    const double CentralWidget::SCINTILLATION_CONST= 2.662;
    const int CentralWidget::STANDARD_VISIBILITY_DISTANCE= 23000;
    const double CentralWidget::NEPERO_N=2.7182818284590452353602874713527;

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent), myDockResults(new DockResults(this)), myDockEffects(new DockEffects(this)),
    myDockSkin(new DockSkin(this)), myDockLea(new DockLea(this)), myDockGoggle(new DockGoggle(this)),
    myDockReflectorsList(new ReflectorsQList(this))
{
    clearInstallationDesription();
    scale=1;
    scaleIndex=4;
    myLabRoomInserted=false;
    myDockControls= new DockControls(this, myDockResults, myDockEffects, myDockSkin, myDockGoggle, myDockLea);

    gridLayout = new QGridLayout(this);
    graphicsView =new DisplayScene();

    gridLayout->addWidget(graphicsView, 0, 0, 1, 1);
    label = new QLabel();
    label->setObjectName(QString::fromUtf8("label"));

    QFont font;
    font.setPointSize(7);
    label->setFont(font);

    gridLayout->addWidget(label, 1, 0, 1, 1);

    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    scintillationBool=false;
    atmEffectsBool=false;
    meteoRange=CentralWidget::STANDARD_VISIBILITY_DISTANCE;
    a_coefficient=0.06*powf(meteoRange, 0.33);

    atmoshericEffectsCoefficient=3.91/(meteoRange)*powf(550.0/(632.0), a_coefficient);
}

void CentralWidget::setNewScene()
{
    graphicsView->setNewScene();
}

void CentralWidget::deleteScene()
{
    graphicsView->deleteScene();
}

bool CentralWidget::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Laser Studio"),
                             tr("Non è possibile salvare il file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);

    out << quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    myLabRoomInserted=false;
    goggleMaterial=myDockControls->getGoggleMaterial();
    QVector <QPointF> pos;
    QVector <int> Type ;
    QVector <QString> Text;
    QVector <QString> StringPosition;
    QVector <double> OpticalDiameter;
    QVector <double> Divergence;
    QVector <double> ReflectorDistance;

    QVector <double> ReflectionCoeff;
    QVector <qreal> ZValue;
    QVector <target> ReflectorKind;
    QVector <double> ReflectorPositioningVect;
    QVector <QString> ReflectorDescriptionVect;
    QPointF laserPosition;
    qreal laserZValue=0;
    double aperture=0;
    int installation=0;
    bool filterOn=false;
    double transmittance=1.0;

    //vettori relativi ai dispositivi ottici
    QVector<QPointF> binocularPosVect;
    QVector<double> binocularOpticalGainVect;
    QVector <int> binocularMagnificationVect;
    QVector <double> binocularTransmissionVect;
    QVector <int> binocular_D0Vect;
    QVector <QString > binocularDescriptionVect;

    //vettori relativi agli ingombri
    QVector<QPointF> footprintPosVect;
    QVector<QRectF>footprintRectVect;
    QVector<QString>footprintDescriptionVect;

    QList<QGraphicsItem*>graphicElements=graphicsView->scene->items();
    
    QList<QGraphicsItem*>::iterator myIterator; // iterator
    myIterator = graphicElements.begin();
    while (myIterator != graphicElements.end() )
        {
          Reflector *reflector = dynamic_cast<Reflector *>(*myIterator);
          if (reflector)
          {
              pos.push_back(reflector->pos());
              Type.push_back(reflector->type());
              Text.push_back(reflector->text());
              StringPosition.push_back(reflector->getStringDetails());
              OpticalDiameter.push_back(reflector->getOpticalDiameter());
              Divergence.push_back(reflector->getDivergence());
              ReflectorDistance.push_back(reflector->getReflectorDistance());
              ReflectionCoeff.push_back(reflector->getMaterialCoeff());
              ZValue.push_back(reflector->getReflectorDistance());
              ReflectorKind.push_back(reflector->getReflectorKind());
              ReflectorPositioningVect.push_back(reflector->getPositioning());
              ReflectorDescriptionVect.push_back(reflector->getDescription());
          }
          
           LaserPoint *laserpoint = dynamic_cast<LaserPoint *>(*myIterator);
           if (laserpoint)
           {
               laserPosition =laserpoint->pos();
               laserZValue=laserpoint->zValue();
               aperture=laserpoint->getAperture();
               installation=laserpoint->getInstallationIndex();
               filterOn=laserpoint->isFilterOn();
               transmittance=laserpoint->getTransmittance();
             }

           LabRoom *myLabRoom = dynamic_cast<LabRoom *>(*myIterator);
           if (myLabRoom)
           {
               myLabRoomInserted=true;
               myLabPosition=myLabRoom->pos();
               myLabRect=myLabRoom->getRoomRect();
            }

           Binocular *binocular = dynamic_cast<Binocular *>(*myIterator);
           if (binocular)
           {
               binocularPosVect.push_back(binocular->pos());
               binocularOpticalGainVect.push_back(binocular->getOpticalGain());
               binocularMagnificationVect.push_back(binocular->getMagnification());
               binocularTransmissionVect.push_back(binocular->getTransmissionCoeff());
               binocular_D0Vect.push_back(binocular->get_D0());
               binocularDescriptionVect.push_back(binocular->getDescription());
            }

           FootprintObject *footprint = dynamic_cast<FootprintObject *>(*myIterator);
           if (footprint)
           {
               footprintPosVect.push_back(footprint->pos());
               QRectF myRect= footprint->getRectangle().rect();
               footprintRectVect.push_back(myRect);
               footprintDescriptionVect.push_back(footprint->getDescription());
               qDebug()<<"myRect quando salva"<<myRect;
           }

         ++myIterator;
    }

    out << myLabRoomInserted << scintillationBool << atmEffectsBool << meteoRange << a_coefficient << atmoshericEffectsCoefficient
        << scaleIndex << scale << force << customer << uasl << uaslAssistant << laserDescription << placeDescription << gridState
        << goggleMaterial << myDockControls->ui->powerErgControl->getScientificNumber() << myDockControls->ui->alphaControl->getScientificNumber()
        << myDockControls->ui->pulseControl->getScientificNumber() << myDockControls->ui->divergenceControl->getScientificNumber()
        << myDockControls->ui->beamDiameterControl->getScientificNumber() << myDockControls->ui->prfControl->getScientificNumber()
        << myDockControls->ui->wavelengthScrollBar->value() << myDockControls->ui->operationCombo->currentIndex()
        << myDockControls->ui->checkGaussianBeam->isChecked() << myDockControls->ui->comboBoxBands->currentIndex()
        << myDockControls->ui->T_SkinControl->getScientificNumber() << myDockControls->ui->teControl->getDialNumber()
        << myDockControls->ui->enableTeCheckBox->isChecked() << myDockControls->ui->internalWaist_checkBox->isChecked()
        << myDockControls->getLambertianMax() << myDockControls->getEMP()
        << myDockControls->getBeamDiameter() << myDockControls->getPowerErg() << laserPosition << laserZValue
        << aperture << installation << filterOn << transmittance << pos << Type << Text << ReflectorDescriptionVect
        << StringPosition << OpticalDiameter << Divergence << ReflectorDistance << ReflectionCoeff
        << ZValue << ReflectorKind << ReflectorPositioningVect << binocularPosVect << binocularOpticalGainVect
        << binocularMagnificationVect << binocularTransmissionVect << binocular_D0Vect << binocularDescriptionVect
        << myLabPosition<< myLabRect << roomNumber << footprintPosVect << footprintRectVect << footprintDescriptionVect;

    QApplication::restoreOverrideCursor();

    file.close();

    return true;
}

bool CentralWidget::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Laser Project"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    quint32 magic;
    in >> magic;
    qDebug()<<magic;
    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr("Laser Project"),
                             tr("Il file non è un file Laser Studio."));
        return false;
    }

    myDockControls->ui->prfControl->setValue(0); //clear

    qreal powerErgControl;
    qreal alphaControl;
    qreal pulseControl;
    qreal divergenceControl;
    qreal beamDiameterControl;
    qreal prfControl;
    qint32 wavelengthScrollBar;
    qint32 operationCombo;
    qint32 comboBox;
    qreal T_SkinSpinBox;
    qreal teControl;
    bool isTeChecked;
    bool isGaussianBeamChecked;
    bool isInternalWaistChecked;

    QList<QGraphicsItem *> items;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    in  >> myLabRoomInserted >> scintillationBool >> atmEffectsBool >> meteoRange >> a_coefficient >> atmoshericEffectsCoefficient
        >> scaleIndex >> scale >> force >> customer >> uasl >> uaslAssistant >> laserDescription >> placeDescription >> gridState >> goggleMaterial
        >> powerErgControl >> alphaControl >> pulseControl >> divergenceControl >> beamDiameterControl >> prfControl >> wavelengthScrollBar
        >> operationCombo >> isGaussianBeamChecked >> comboBox >> T_SkinSpinBox >> teControl >> isTeChecked >> isInternalWaistChecked
        >> lambertianMax >> laserEMP >> laserBeamDiameter >> laserPowerErg >> laserPosition >> laserZValue >> aperture >> installation
        >> filterOn >> transmittance >> posVect >> TypeVect >> TextVect >> ReflectorDescriptionVect
        >> StringPositionVect >> OpticalDiameterVect >> DivergenceVect >> ReflectorDistanceVect >> ReflectionCoeffVect >> ZValueVect
        >> ReflectorKindVect >> ReflectorPositioningVect >> binocularPosVect >> binocularOpticalGainVect >> binocularMagnificationVect >> binocularTransmissionVect
        >> binocular_D0Vect >> binocularDescriptionVect >> myLabPosition >> myLabRect >> roomNumber
        >> footprintPosVect >> footprintRectVect >> footprintDescriptionVect;

     myDockControls->ui->operationCombo->setCurrentIndex(operationCombo);
     myDockControls->ui->enableTeCheckBox->setChecked(isTeChecked);
     myDockControls->ui->comboBoxBands->setCurrentIndex(comboBox);
     myDockControls->ui->powerErgControl->setValue(powerErgControl);
     myDockControls->ui->alphaControl->setValue(alphaControl);
     myDockControls->ui->pulseControl->setValue(pulseControl);
     myDockControls->ui->divergenceControl->setValue(divergenceControl);
     myDockControls->ui->beamDiameterControl->setValue(beamDiameterControl);
     myDockControls->ui->wavelengthScrollBar->setValue(wavelengthScrollBar);
     myDockControls->ui->teControl->setDialNumber(teControl);
     myDockControls->ui->prfControl->setValue(prfControl);
     myDockControls->ui->internalWaist_checkBox->setChecked(isInternalWaistChecked);
     myDockControls->ui->T_SkinControl->setValue(T_SkinSpinBox);
     myDockControls->ui->checkGaussianBeam->setChecked(isGaussianBeamChecked);


     myDockControls->enableTeEdtiting(isTeChecked);
     myDockControls->setGoggleMaterial(goggleMaterial);

     modified();

    QApplication::restoreOverrideCursor();
    return true;
}

void CentralWidget::setForce(const QString _force)
{
    force=_force;
}

void CentralWidget::setCustomer(const QString _customer)
{
    customer=_customer;
}

QString CentralWidget::getForce()const
{
    return force;
}

QString CentralWidget::getCustomer()const
{
    return customer;
}

QString CentralWidget::get_UASL()const
{
    return uasl;
}

QString CentralWidget::get_UASL_Assistant()const
{
    return uaslAssistant;
}

QString CentralWidget::getLaserDescription()const
{
    return laserDescription;
}

QString CentralWidget::getPlaceDescription()const
{
    return placeDescription;
}

void CentralWidget::set_UASL(const QString _uasl)
{
    uasl=_uasl;
}

void CentralWidget::set_UASL_Assistant(const QString _uaslAssistant)
{
    uaslAssistant=_uaslAssistant;
}

void CentralWidget::setLaserDescription(const QString _laserDescription)
{
    laserDescription=_laserDescription;
}

void CentralWidget::setPlaceDescription(const QString _placeDescription)
{
    placeDescription=_placeDescription;
}

QPointF CentralWidget::getLaserPosition()const
{
    return laserPosition;
}

qreal CentralWidget::getLaserZValue()const
{
    return laserZValue;
}

double CentralWidget::getLambertianMax()const
{
    return lambertianMax;
}

QVector <QPointF> CentralWidget::getPosVect()const
{
    return posVect;
}

QVector <int> CentralWidget::getTypeVect()const
{
    return TypeVect;
}

QVector <QString> CentralWidget::getTextVect()const
{
    return TextVect;
}

QVector <QString> CentralWidget::getStringPositionVect()const
{
    return StringPositionVect;
}

QVector <double> CentralWidget::getOpticalDiameterVect()const
{
    return OpticalDiameterVect;
}

QVector <double> CentralWidget::getDivergenceVect()const
{
    return DivergenceVect;
}

QVector <double> CentralWidget::getReflectorDistanceVect()const
{
    return ReflectorDistanceVect;
}

QVector <double> CentralWidget::getReflectionCoeffVect() const
{
    return ReflectionCoeffVect;
}

QVector <qreal> CentralWidget::getZValueVect()const
{
    return ZValueVect;
}

QVector <target> CentralWidget::getReflectorKindVect() const
{
    return ReflectorKindVect;
}

QVector <double> CentralWidget::getReflectorPositioningVect()const
{
    return ReflectorPositioningVect;
}

QVector <QString> CentralWidget::getDescriptionVect()const
{
    return ReflectorDescriptionVect;
}

QVector <QPointF> CentralWidget::getBinocularPosVect()const
{
    return binocularPosVect;
}

QVector <double> CentralWidget::getBinocularOpticalGainVect()const
{
    return binocularOpticalGainVect;
}

QVector <int> CentralWidget::getBinocularMagnificationVect()const
{
    return binocularMagnificationVect;
}

QVector <double> CentralWidget::getBinocularTransmissionVect()const
{
    return binocularTransmissionVect;
}

QVector <int> CentralWidget::getBinocular_D0Vect()const
{
    return binocular_D0Vect;
}

QVector <QString > CentralWidget::getBinocularDescriptionVect()const
{
    return binocularDescriptionVect;
}

bool CentralWidget::getGridState()const
{
    return gridState;
}

void CentralWidget::setGridState(const bool& _gridState)
{
    gridState=_gridState;
}

double CentralWidget::getAperture()const
{
    return aperture;
}

int CentralWidget::getInstallation()const
{
    return installation;
}

bool CentralWidget::isFilterOn()
{
    return filterOn;
}
double CentralWidget::getTransmittance()const
{
    return transmittance;
}

double CentralWidget::getLaserEMP()const
{
    return laserEMP;
}

double CentralWidget::getPowerErg()const
{
    return laserPowerErg;
}

double CentralWidget::getBeamDiameter()const
{
    return laserBeamDiameter;
}

void CentralWidget::clearInstallationDesription()
{
    force.clear();
    customer.clear();
    uasl.clear();
    uaslAssistant.clear();
    laserDescription.clear();
    placeDescription.clear();
}

void CentralWidget::setScale(const double& _scale)
{
    scale=_scale;
    graphicsView->scale(scale, _scale);
}

void CentralWidget::setScaleIndex(const int& _scaleIndex)
{
    scaleIndex=_scaleIndex;
}

int CentralWidget::getScaleIndex()const
{
    return scaleIndex;
}

double CentralWidget::getScale()const
{
    return scale;
}

void CentralWidget::setAtmEffectsBool(bool _atmEffectsBool)
{
    atmEffectsBool=_atmEffectsBool;
}

bool CentralWidget::getAtmEffectsBool()
{
    return atmEffectsBool;
}

void CentralWidget::setScintillationBool(bool _scintillationBool)
{
    scintillationBool=_scintillationBool;
}

bool CentralWidget::getScintillationBool()
{
    return scintillationBool;
}

QPointF CentralWidget::getLabPosition()const
{
    return myLabPosition;
}

int CentralWidget::getRoomNumber()const
{
    return roomNumber;
}

QRectF CentralWidget::getLabRect()const
{
    return myLabRect;
}

void CentralWidget::CentralWidget::setAtmoshericEffectsCoefficient(const double& _atmoshericEffectsCoefficient)
{
    atmoshericEffectsCoefficient=_atmoshericEffectsCoefficient;
}

double CentralWidget::getAtmoshericEffectsCoefficient() const
{
    return atmoshericEffectsCoefficient;
}

void CentralWidget::setA_Coefficient(const double& _a_coefficient)
{
    a_coefficient=_a_coefficient;
}

void CentralWidget::setMeteoRange(const int& _meteoRange)
{
    meteoRange=_meteoRange;
}

double CentralWidget::getA_Coefficient() const
{
    return a_coefficient;
}
int CentralWidget::getMeteoRange() const
{
    return meteoRange;
}

bool CentralWidget::isLabRoomInserted()
{
    return myLabRoomInserted;
}

QVector <QPointF> CentralWidget::getFootprintPosVect()
{
    return footprintPosVect;
}

QVector <QRectF> CentralWidget::getFootprintRectVect()
{
    return footprintRectVect;
}

QVector <QString> CentralWidget::getFootprintDescriptionVect()
{
    return footprintDescriptionVect;
}

CentralWidget::~CentralWidget()
{

}
