#include <QtGui>
#include <QInputDialog>
#include <QLineEdit>
#include <QFont>
#include <QFontMetricsF>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGuiApplication>
#include <QDir>
#include <QPalette>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <QMessageBox>

#include "link.h"
#include "reflector.h"

#include <cmath>
#include <iostream>

const double Reflector::radDeg = 3.1415926535897932384626433832795/180;
const double Reflector::phi_const = 2.5;

Reflector::Reflector(double _opticalDiameter, double _divergence, double _reflectorDistance, double _laserBeamDiameter,
                     double _laserEMP, double _laserPowerErg, double _lambertianMax, target _myTarget):QGraphicsObject()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::transparent;
    myBeamColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    opticalDiameter= _opticalDiameter;
    divergence=_divergence;
    reflectorDistance=_reflectorDistance;
    laserBeamDiameter=_laserBeamDiameter;
    laserEMP=_laserEMP;
    laserPowerErg=_laserPowerErg;
    lambertianMax=_lambertianMax;
    positioning=0;
    correctPositioning=0;
    scale=1.0;
    laserPhase=0.0;

    myTarget=_myTarget;
    materialCoeff= 0.9;

        if(myTarget==WET_TARGET)
            n=1.33;
        else
            n=1.5;

    MyFresnelReflector_ptr=nullptr;
    MyWetReflector_ptr=nullptr;
    MyLambertianReflector_ptr=nullptr;

    MyReflector=new LambertianReflector(n);
    MyLambertianReflector_ptr= MyReflector;
    setReflectorKindString();
    reflectorOperation();
    setTipString();
    setToolTip(tipString);
    setStringDetails();
    setTextLabel();

    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

Reflector::~Reflector()
{
    foreach (Link *link, myLinks)
        delete link;
}

void Reflector::setReflectorColor()
{
    switch(myTarget)
    {
        case (WET_TARGET):
            setBeamColor(Qt::blue);
            setTextColor(Qt::blue);
        break;
        case(GLASS_TARGET):
            setBeamColor(Qt::green);
            setTextColor(Qt::green);
        break;
        case(MIRROR_TARGET):
            setBeamColor(Qt::magenta);
            setTextColor(Qt::magenta);
        break;
        case(LAMBERTIAN_TARGET):
            setBeamColor(Qt::darkCyan);
            setTextColor(Qt::darkCyan);
        break;
        default:
            setOutlineColor(Qt::black);
     }
}

int Reflector::type() const
{
    return Type;
}

QString Reflector::text() const
{
    return textLabel;
}

void Reflector::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor Reflector::textColor() const
{
    return myTextColor;
}

void Reflector::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Reflector::outlineColor() const
{
    return myOutlineColor;
}

void Reflector::setBeamColor(const QColor &color)
{
    myBeamColor = color;
    update();
}

QColor Reflector::beamColor() const
{
    return myBeamColor;
}

void Reflector::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Reflector::backgroundColor() const
{
    return myBackgroundColor;
}

void Reflector::addLink(Link *link)
{
    myLinks.insert(link);
}

void Reflector::removeLink(Link *link)
{
    myLinks.remove(link);
}

QRectF Reflector::boundingRect() const
{
    const int Margin = 3;

    QRectF rect;

    QRectF myOutlineRect=unitedBounding();

    if(myOutlineRect.height()>myPositioningElement)
        rect=myOutlineRect.adjusted(-Margin, -Margin, +Margin, +Margin);
    else
        rect=selectionReflectorRect().adjusted(-Margin, -Margin, +Margin, +Margin);

    return rect;
}

QPainterPath Reflector::shape() const
{    
    QPainterPath path;
    path.addRect(outlineRect());
    return path;
}

void Reflector::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);

    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::darkGray);
        pen.setWidth(2);
    }
    pen.setCosmetic(true);

    QPen beamPen(myBeamColor);
    beamPen.setStyle(Qt::DashLine);

    if (option->state & QStyle::State_Selected) {
        beamPen.setStyle(Qt::DotLine);
        pen.setColor(Qt::darkGray);
        beamPen.setWidth(2);
    }

    beamPen.setCosmetic(true);
    painter->setPen(beamPen);
    painter->setBrush(QColor(myBackgroundColor));

    QPainterPath semiPath;
    semiPath.moveTo(0.0, 0.0);
    QRectF myBoudingRect=QRectF(0.0, 0.0, selectionReflectorRect().width(), selectionReflectorRect().height());
    myBoudingRect.moveCenter(QPointF(0.0, 0.0));

    double firstPie=90.0;
    double secondPie=180.0;

    semiPath.arcTo(myBoudingRect, firstPie, secondPie);
    semiPath.lineTo(QPointF(0.0, 0.0));

    painter->drawPath(semiPath);
    painter->setPen(pen);
    QRectF rect = outlineRect();
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);    

    prepareGeometryChange();

    QRectF pixRect=outlineRect();
    QRectF source(0.0, 0.0, reflectorPix.width(), reflectorPix.height());
    painter->drawPixmap(pixRect, reflectorPix, source);

    QTransform transform=painter->transform();

    double m13=transform.m13();
    double m23=transform.m23();
    double m32=transform.m32();
    double m31=transform.m31();
    double m21=transform.m21();
    double m12=transform.m12();

    QRectF boundingRect=labelRect();
    painter->save();

    painter->resetTransform();
    QTransform myTransform=QTransform(1.0, m12, m13, m21, 1.0, m23, m31, m32, 1.0);

    painter->setTransform(myTransform);

    QRectF lineRect=reflectorPix.rect();
    painter->setPen(beamPen);
    QPoint p1= QPoint(0,0);
    QPoint p2=QPoint(-1.6*lineRect.height()*sin(radDeg*double(positioning)),
                     1.6*lineRect.height()*cos(radDeg*double(positioning)));
    QLine positioningLine(p1, p2);

    positioningLine.translate(1.6*lineRect.height()*sin(radDeg*double(positioning))/2,
                              -1.6*lineRect.height()*cos(radDeg*double(positioning))/2);
    painter->drawLine(positioningLine);

    QPen textPen(myBeamColor);
    painter->setPen(textPen);

    setTextLabel();

    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);
    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

void Reflector::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    bool ok;
    if(myTarget==GLASS_TARGET)
    {
    double n = QInputDialog::getDouble(event->widget(), tr("Indice di rifrazione"),
                                       tr("Inserisci l'indice di rifrazione:"),
                                       getRefration_n(), 1.200, 1.900, 3, &ok,Qt::WindowFlags() ,0.1);
    if (ok){
        setRefraction_n(n);
        setStringDetails();
        }
    }
}

QVariant Reflector::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        foreach (Link *link, myLinks)
        {
            reflectorDistance=link->linkLenght();
            link->trackNodes();
            laserPhase=-link->LinkPhase();

            if(myTarget==LAMBERTIAN_TARGET)
            correctPositioning=positioning+laserPhase;
                else
            correctPositioning=2*(positioning+laserPhase);

            reflectorOperation();
            setStringDetails();
            setTextLabel();
            setTipString();
            setToolTip(tipString);
        }
        if (change == ItemSelectedChange) {
            return isSelected();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void Reflector::laserParametersChanged()
{
  foreach (Link *link, myLinks)
  {
      reflectorDistance=link->linkLenght();
      link->trackNodes();    
      laserPhase=-link->LinkPhase();

      if(myTarget==LAMBERTIAN_TARGET)
      correctPositioning=positioning+laserPhase;
          else
      correctPositioning=2*(positioning+laserPhase);

      reflectorOperation();
      setStringDetails();
  }
}

void Reflector::laserPositionChanged()
{
  foreach (Link *link, myLinks)
  {
       reflectorDistance=link->linkLenght();
       link->trackNodes();     
       laserPhase=link->LinkPhase();
       reflectorOperation();
       setTextLabel();
       setReflectorKindString();
       setStringDetails();
   }
}

QRectF Reflector::outlineRect() const
{
    double rectWidht=reflectorPix.width()/scale;
    double rectHeight=reflectorPix.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF center=rect.center();
    rect.translate(-center);
    return rect;
}

int Reflector::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}

QRectF Reflector::selectionReflectorRect() const
{
    QSizeF rectSize(2*myPositioningElement, 2*myPositioningElement);
    QPointF center(0.0, 0.0);
    QRectF rect(center, rectSize);
    rect.translate(-rect.center());
    return rect;
}

void Reflector::setOpticalDiameter(double newRadius)
{
   if(newRadius==opticalDiameter)
       return;

    opticalDiameter=newRadius;
    laserParametersChanged();
}

double Reflector::getOpticalDiameter()const
{
       return opticalDiameter;
}

void Reflector::setSkinDistance(double newRadius)
{
   if(newRadius==skinDistance)
       return;

   skinDistance=newRadius;
}

double Reflector::getReflectorDistance() const
{
    return reflectorDistance;
}

void Reflector::setDivergence(const double _divergence)
{
   if(_divergence==divergence)
       return;

   divergence=_divergence;
   laserParametersChanged();
}

double Reflector::getDivergence()const
{
       return divergence;
}

void Reflector::setStringDetails()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();

    QString xString;
    QString yString;
    QString reflectorDistanceString;
    QString myMaxElementString;
    QString myPositioningElementString;
    QString nString;
    QString myReflectionCoeffString;

    xString=QString::number(xCoordinate,'f', 0);
    yString=QString::number(yCoordinate,'f', 0);
    reflectorDistanceString=QString::number(reflectorDistance,'f',0);
    myMaxElementString=QString::number(myMaxElement,'f',2);
    myPositioningElementString=QString::number(myPositioningElement,'f',2);
    nString=QString::number(n, 'f',2);
    myReflectionCoeffString=QString::number(materialCoeff, 'f',2);;

    switch(myTarget)
    {
        case (WET_TARGET):
        refrectorDetailsString=reflectionKindString + QString(" (%1,%2) \ndist[m]: %3, DROr[m]: %4, DROr max [m]: %5 \nn=%6 ρ=%7")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(reflectorDistanceString)
                                        .arg(myPositioningElementString)
                                        .arg(myMaxElementString)
                                        .arg(nString)
                                        .arg(myReflectionCoeffString);
        break;
        case(GLASS_TARGET):
        refrectorDetailsString=reflectionKindString + QString(" (%1,%2) \ndist[m]: %3, DROr[m]: %4, DROr max [m]: %5 \nn=%6 τ=%7")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(reflectorDistanceString)
                                        .arg(myPositioningElementString)
                                        .arg(myMaxElementString)
                                        .arg(nString)
                                        .arg(myReflectionCoeffString);

        break;
        case(MIRROR_TARGET):
        refrectorDetailsString=reflectionKindString + QString(" (%1,%2) \ndist[m]: %3, DROr[m]: %4, \nρ=%5")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(reflectorDistanceString)
                                        .arg(myPositioningElementString)
                                        .arg(myReflectionCoeffString);

        break;
        case(LAMBERTIAN_TARGET):
        refrectorDetailsString =reflectionKindString + QString(" (%1,%2) \ndist[m]: %3, DROr[m]: %4, DROr max [m]: %5 \nρ=%6")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(reflectorDistanceString)
                                        .arg(myPositioningElementString)
                                        .arg(myMaxElementString)
                                        .arg(myReflectionCoeffString);

        break;
        default:
        refrectorDetailsString="Problems";
     }
}

void Reflector::setTipString()
{
    setStringDetails();
    tipString=refrectorDetailsString;
}

void Reflector::setTextLabel()
{
    double xCoordinate;
    double yCoordinate;

    xCoordinate=pos().x();
    yCoordinate=pos().y();

    QString xString;
    QString yString;

    xString=QString::number(xCoordinate=pos().x(),'f', 0);
    yString=QString::number(yCoordinate=pos().y(),'f', 0);

    prepareGeometryChange();
    textLabel =getReflectorKindString()+QString("\n(%1,%2)")
                                    .arg(xString)
                                    .arg(yString);
    update();
}

QString Reflector::getStringDetails()
{
   return refrectorDetailsString;
}

double Reflector::getRefration_n() const
{
    return n;
}

void Reflector::setRefraction_n(const double& _n)
{
    n=_n;
}

double Reflector::getDNRO() const
{
    return dnro;
}

void Reflector::setDNRO(const double& _dnro)
{
    dnro=_dnro;
}

double Reflector::getObjectDistance()const
{
    return objectDistance;
}

void Reflector::setReflectorDistance(const double &_reflectorDistance)
{
    reflectorDistance=_reflectorDistance;
}

double Reflector::getSeqNumber()const
{
    return seqNumber;
}

void Reflector::setSeqNumber(const double& _seqNumber)
{
    seqNumber=_seqNumber;
}

void Reflector::reflectorOperation()
{
    switch(myTarget)
    {
        case (WET_TARGET):
            {
            MyWetReflector_ptr=MyReflector;
            MyWetReflector_ptr->computeTrigonometricReflection();
            MyWetReflector_ptr->computeZs(MyWetReflector_ptr->getRho_sVect(),opticalDiameter, reflectorDistance, divergence, materialCoeff);
            rho_sVect=MyWetReflector_ptr->getRho_sVect();
            myZsVector=MyWetReflector_ptr->getZsVect();
            setPositioningElement();
            setMaxElement();
            }
        break;

        case(target::GLASS_TARGET):
            {
            MyFresnelReflector_ptr=MyReflector;
            MyFresnelReflector_ptr->computeTrigonometricReflection();
            MyFresnelReflector_ptr->computeZs(MyFresnelReflector_ptr->getRho_sVect(),opticalDiameter, reflectorDistance, materialCoeff);
            rho_sVect=MyFresnelReflector_ptr->getRho_sVect();
            myZsVector=MyFresnelReflector_ptr->getZsVect();
            setPositioningElement();
            setMaxElement();
            }
        break;

    case(target::MIRROR_TARGET):
            {
            if(getOpticalDiameter()-getReflectorDistance()>0)
                myPositioningElement= (getOpticalDiameter()-getReflectorDistance())*materialCoeff;
            else
                myPositioningElement=0;
            }

        break;

        case(target::LAMBERTIAN_TARGET):
            {

             MyLambertianReflector_ptr=MyReflector;
             //MyLambertianReflector_ptr->setLambertianMax(lambertianMax);
             MyLambertianReflector_ptr->setLaserBeamDiameter(laserBeamDiameter);
             MyLambertianReflector_ptr->setLaserDivergence(divergence);
             MyLambertianReflector_ptr->setLaserEMP(laserEMP);
             MyLambertianReflector_ptr->setLaserPowerErg(laserPowerErg);
             qDebug()<<"laserPoewErg"<<laserPowerErg;
             MyLambertianReflector_ptr->setLaserDistance(reflectorDistance);
             MyLambertianReflector_ptr->computeSpotDiameter();
             spotDiameter=MyLambertianReflector_ptr->getSpotDiameter();

             if(opticalDiameter!=0)
             {
             qDebug()<< "Imposto nella classe LambertianReflector il diametro del fascio" << laserBeamDiameter;
             qDebug()<< "Imposto nella classe LambertianReflector la divergenza del fascio" << divergence;
             qDebug()<< "Imposto nella classe LambertianReflector l'EMP del laser" << laserEMP;
             qDebug()<< "Imposto nella classe LambertianReflector la potenza/energia del laser" << laserPowerErg;
             qDebug()<< "Imposto nella classe LambertianReflector la ddistanza del riflettore dal laser" << reflectorDistance;

             //MyLambertianReflector_ptr->setExendedDiffusion();

             if(MyLambertianReflector_ptr->evaluateDiffusionDistance())
                 {
                     exendedDiffusion=true;
                     reflectionKindString="Lambertiana Estesa";
                     constant=MyLambertianReflector_ptr->getConstant();
                     alpha=MyLambertianReflector_ptr->getAlpha();
                     CE=MyLambertianReflector_ptr->getCE();
                     kindOfSurface=MyLambertianReflector_ptr->getKindOfSurface();
                     newRapSolution=MyLambertianReflector_ptr->getNewRapSolution();
                     alphaIndicator=MyLambertianReflector_ptr->getAlphaIndicator();
                     reflectorHazardDistance=MyLambertianReflector_ptr->getReflectorHazardDistance();
                     myPositioningElement=reflectorHazardDistance;
                     myMaxElement=reflectorHazardDistance;
                 }
             else
                {
                     exendedDiffusion=false;
                     reflectionKindString="Lambertiana Puntiforme";
                     MyLambertianReflector_ptr->computeTrigonometricReflection();
                     MyLambertianReflector_ptr->computeZs(lambertianMax, materialCoeff);
                     myZsVector=MyLambertianReflector_ptr->getZsVect();
                     setMaxElement();
                     setPositioningElement();
                 }
               }
             else
             {
                 reflectionKindString="Lambertiana";
                 MyLambertianReflector_ptr->computeTrigonometricReflection();
                 MyLambertianReflector_ptr->computeZs(0.0, materialCoeff);
                 myZsVector=MyLambertianReflector_ptr->getZsVect();
                 setMaxElement();
                 setPositioningElement();
             }

             }
        break;
        default:
            qDebug()<< "Problems ;-)";
     }
   }


void Reflector::setMaxElement()
{
    if(myTarget==LAMBERTIAN_TARGET)
    myMaxElement=myZsVector.at(0).second;
    else{
    int myZsVectorHalSize=myZsVector.size()/2;
    myMaxElement=myZsVector.at(myZsVectorHalSize).second;
    }

   qDebug()<< "L'elemento massimo: " << myMaxElement;

}

double Reflector::getMaxElement()
{
    return myMaxElement;
}

void Reflector::setPositioningElement()
{
   int myZsVectorRow;
   myZsVectorRow=abs(correctPositioning);

   myPositioningElement=myZsVector.at(myZsVectorRow).second;

   qDebug()<< "La riga del vettore myZsVectorRow: " << myZsVectorRow;
   qDebug()<< "L'elemento nell'orientamento corrente: " << myPositioningElement;
}

double Reflector::getPositioningElement()
{
    return myPositioningElement;
}

std::vector< std::pair <double,double> > Reflector::getZsVect()
{
    return myZsVector;
}

std::vector< std::pair <double,double> > Reflector::getRho_sVect()
{
    return rho_sVect;
}

target Reflector::getReflectorKind()
{
    return myTarget;
}

void Reflector::setReflectorKindString()
{
    switch(myTarget)
    {
        case (target::WET_TARGET):
        reflectionKindString="lamiera bagnata";
        break;
        case(target::GLASS_TARGET):
        reflectionKindString="Vetro";
        break;
        case(target::MIRROR_TARGET):
        reflectionKindString="Specchio";
        break;
        case(target::LAMBERTIAN_TARGET):
        if(opticalDiameter==0.0)
            reflectionKindString="Lambertiana";
        else
        {
        if(exendedDiffusion)
           reflectionKindString="Lambertiana Estesa";
        else
           reflectionKindString="Lambertiana Puntiforme";
        }

        break;
        default:
            qDebug()<< "Problems ;-)";
     }
}

QString Reflector::getReflectorKindString()
{
    return reflectionKindString;
}

double Reflector::getReflectorHazardDistance()
{
    return reflectorHazardDistance;
}

double Reflector::getMaterialCoeff()const
{
    return materialCoeff;
}

void Reflector::setMaterialCoeff(const double& _materialCoeff)
{
    prepareGeometryChange();
    materialCoeff=_materialCoeff;
    reflectorOperation();
    update();
}

void Reflector::setLaserEMP(const double& _laserEMP)
{
    if(_laserEMP==laserEMP)
        return;

    laserParametersChanged();
    laserEMP=_laserEMP;
}

double Reflector::getLaserEMP()const
{
    return laserEMP;
}

void Reflector::setLaserBeamDiameter(const double& _laserBeamDiameter)
{
    if(_laserBeamDiameter==laserBeamDiameter)
        return;

    laserBeamDiameter=_laserBeamDiameter;
    laserParametersChanged();
}

double Reflector::getLaserBeamDiameter()const
{
    return laserBeamDiameter;
}

void Reflector::setLaserPowerErg(const double& _laserPowerErg)
{
    if(_laserPowerErg==laserPowerErg)
        return;

    laserPowerErg=_laserPowerErg;
    laserParametersChanged();
}

double Reflector::getLaserPowerErg()const
{
    return laserPowerErg;
}

void Reflector::setLambertianMax(const double _lambertianMax)
{
    if(_lambertianMax==lambertianMax)
        return;

    lambertianMax=_lambertianMax;
    laserParametersChanged();
}

bool Reflector::isExendedDiffusion()
{
    return exendedDiffusion;
}

void Reflector::setIsExendedDiffusion(const bool _exendedDiffusion)
{
    exendedDiffusion=_exendedDiffusion;
}

double Reflector::getConstant()const
{
    return constant;
}

double Reflector::getSpotDiameter()const
{
    return spotDiameter;
}

double Reflector::getAlpha()const
{
    return alpha;
}

double Reflector::getCE()const
{
    return CE;
}

QString Reflector::getKindOfSurface()const
{
    return kindOfSurface;
}

double Reflector::getNewRapSolution()const
{
    return newRapSolution;
}

double Reflector::getAlphaIndicator()const
{
    return alphaIndicator;
}

void Reflector::setPixmap()
{
    switch(myTarget)
    {
        case (WET_TARGET):
        reflectorPix=QPixmap(":/images/wet1.png");
        break;

        case(GLASS_TARGET):
        reflectorPix=QPixmap(":/images/window1.png");

        break;
        case(MIRROR_TARGET):
        reflectorPix=QPixmap(":/images/mirror1.png");

        break;
        case(LAMBERTIAN_TARGET):
        reflectorPix=QPixmap(":/images/lambertian1.png");

        break;
        default:
        qDebug()<<"errore";
     }
}

void Reflector::setLaserShape(const QPainterPath& path)
{
    laserShapePath=path;
}


QRectF Reflector::labelRect()
{
        const int Padding = 10;
        QFontMetricsF metrics =(QFontMetricsF)qApp->font();
        myTextRect = metrics.boundingRect(textLabel);
        myTextRect.adjust(-Padding, -Padding, +Padding, +Padding);
        QRectF pixRect=QRectF(0, 0, 60.0, 60.0);

        QPointF rectPoint(-pixRect.width()/2+Padding, pixRect.height()/2+myTextRect.height()/2+Padding);
        myTextRect.translate(rectPoint);
        return myTextRect;
}

void Reflector::setPositioning(const int& _positioning)
{
        positioning=_positioning;
}

int Reflector::getPositioning()const
{
        return positioning;
}

void Reflector::setPixScale(const double &newScale)
{
    scale=newScale;
}

QRectF Reflector::unitedBounding() const
{
    QRectF unitedRect=outlineRect();
    unitedRect.adjust(-0.5*outlineRect().width(), -0.5*outlineRect().height(),
                      1.2*outlineRect().width(), 1.2*outlineRect().height());
    return unitedRect;
}

void Reflector::setDescription(const QString& _description)
{
    description=_description;
}

QString Reflector::getDescription()const
{
    return description;
}

void Reflector::setAtmoshericEffectsCoefficient(const double& _atmoshericEffectsCoefficient)
{
    atmoshericEffectsCoefficient=_atmoshericEffectsCoefficient;
}

double Reflector::setAtmoshericEffectsCoefficient() const
{
    return atmoshericEffectsCoefficient;
}

double Reflector::getLaserPhase()const
{
    return laserPhase;
}

int Reflector::getCorrectPositioning()const
{
    return correctPositioning;
}



void Reflector::setCorrectPositioning()
{

}
