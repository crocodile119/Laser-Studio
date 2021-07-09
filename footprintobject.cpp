#include "footprintobject.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <cmath>
#include "undo_commands/boxredimensioncommand.cpp"

const double FootprintObject::degRad = 180.0/3.1415926535897932384626433832795;
const double FootprintObject::radDeg = 3.1415926535897932384626433832795/180.0;
static double resizeHandleWidth = 10.0;
const QSizeF Rectangle::minSize(0.05, 0.05);

Rectangle::Rectangle(qreal x, qreal y, qreal width, qreal height)
{
    m_rect=QRectF(x, y, width, height);
}

QRectF Rectangle::rect()const
{
    return m_rect;
}

void Rectangle::setRect(const QRectF &rect)
{
    if(rect==m_rect)
        return;

    m_rect=rect;
}

QRectF Rectangle::resizeHandle() const
{
    QPointF br = m_rect.bottomRight();
    QRectF myRect=QRectF(br - QPointF(resizeHandleWidth, resizeHandleWidth), br);
    return myRect.translated(resizeHandleWidth/2, resizeHandleWidth/2);
}

FootprintObject::FootprintObject(double _scale)
{
    scale=_scale;
    Rectangle myRect(0.0, 0.0, 20.0/scale, 20.0/scale);
    oldRect=QRectF(0.0, 0.0, 20.0/scale, 20.0/scale);
    myGraphicsRect=myRect;
    handleBrush = Qt::white;
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    addfreeCorner=false;
    setResizeHandle(scale);
    this->setAcceptHoverEvents(true);
}

FootprintObject::~FootprintObject()
{
    foreach (ObjectLink *objectlink, myObjectLinks)
        delete objectlink;
}

void FootprintObject::setResizeHandle(double newScale)
{
    resizeHandleWidth=10.0/newScale;
}

int FootprintObject::type() const
{
    return Type;
}

void FootprintObject::addObjectLink(ObjectLink *objectlink)
{
    myObjectLinks.push_back(objectlink);
}

void FootprintObject::removeObjectLink()
{
    myObjectLinks.clear();
}

ObjectLink* FootprintObject::getObjectLink()
{
    return myObjectLinks.first();
}

QRectF FootprintObject::boundingRect() const
{
    const Rectangle rettangle=myGraphicsRect;
    QRectF myBoundingRect=rettangle.rect();
    QRectF handle=rettangle.resizeHandle();
    QRectF myRect=myBoundingRect.united(handle);
    return myRect.united(myTextRect);
}

void FootprintObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget */* widget */)
{   
    QPen pen;
    pen.setColor(Qt::green);
    pen.setCosmetic(true);

    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        pen.setColor(Qt::darkGray);
    }

    painter->setPen(pen);
    painter->setBrush(Qt::white);

    /*QLineF lineRight= QLineF(QPointF(0.0, 0.0), QPointF(20.0, 0));
    QLineF lineDown= QLineF(QPointF(0.0, 0.0), QPointF(0, 20.0));
    painter->drawLine(lineRight);
    painter->drawLine(lineDown);
    */
    const Rectangle &rectangle=myGraphicsRect;
    rect = rectangle.rect();
    //setResizeHandle(scale);
    //rect.adjust(1.0, 1.0, -10.0/2, -10.0/2);

    painter->drawRect(rect);
    setResizeHandle(scale);
    QPen shadow;
    shadow.setColor(Qt::transparent);
    QBrush shadowBrush= QBrush(QColor(230, 230, 230, 156));
    painter->setPen(shadow);
    painter->setBrush(shadowBrush);
    setShadowPath(shadowPath(opticalDiameter));
    setEhnacedShadowPath(shadowPath(ehnacedDiameter));
    //painter->drawPath(shadowPath(ehnacedDiameter));

    painter->drawPath(shadowPathItem);
    //painter->drawPath(laserEhnacedBeamPath);
    painter->drawPath(ehnacedShadowPathItem);
/*
    //QPointF center= rect.bottomRight();

*/

    QPen handlePen;
    handlePen.setColor(Qt::gray);
    handlePen.setCosmetic(true);
    painter->setPen(handlePen);
    painter->setBrush(handleBrush);

    handleRect(rectangle);

    painter->drawRect(handle_rect);

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

    QPen textPen(Qt::black);
    painter->setPen(textPen);

    setTextLabel();

    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);
    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

QVariant FootprintObject::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {

        updateTipString();

        foreach (ObjectLink *itemlink, myObjectLinks)
        {
        itemlink->trackNodes();
        setLaserBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapePath()));
        setLaserEhancedBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapeEnhacedPath(ehnacedDiameter)));

        /******************************************************************************************************
         * L'uso di mapToTitem riferisce le coordinate di laserpoint al sistema di riferimento solidale       *
         * all'elemento grafico lasciando le stesse coordinate del punto laser riferite al sistema solidale   *
         * alla scena (prevedevo che mapToItem effettuasse anche la trasformazione del cambio di riferimento).*
         * Ciò comporta la nesessità di sottrarre da maToItem le coordinate di laserpoint rispetto al         *
         * riferimento della scena                                                                            *
         ******************************************************************************************************/

        laserposition=itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->pos()-itemlink->fromLaser()->pos());
        }
        setShadowPath(shadowPath(opticalDiameter));
        setEhnacedShadowPath(shadowPath(ehnacedDiameter));

        return pos();
    }
    return QGraphicsItem::itemChange(change, value);
}

void FootprintObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        const Rectangle &rectangle = myGraphicsRect;

        m_resizeHandlePressed = rectangle.resizeHandle().contains(event->pos());

        if (m_resizeHandlePressed)
        {
            m_mousePressOffset = rectangle.rect().bottomRight() - event->pos();
        }
        else
            m_mousePressOffset = event->pos() - rectangle.rect().topLeft();


        QGraphicsItem::mousePressEvent(event);
}

void FootprintObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    const Rectangle &rectangle = myGraphicsRect; 
    oldRect=rect;
    QRectF rect=myGraphicsRect.rect();
    if (m_resizeHandlePressed) {
        setFlag(ItemIsMovable, false);
        rect = QRectF(rectangle.rect().topLeft(), event->pos()  + m_mousePressOffset);
    } else {
        setFlag(ItemIsMovable, true);
        rect = rectangle.rect();
        rect.moveTopLeft(event->pos() - m_mousePressOffset);
    }

    QSizeF size = rect.size().expandedTo(Rectangle::minSize);
    rect.setSize(size);

    if((rect.width()!=myGraphicsRect.rect().width())||(rect.height()!=myGraphicsRect.rect().height()))
    {
        prepareGeometryChange();
        handleRect(rectangle);
        update(rect);
        myGraphicsRect.setRect(rect);
        QUndoCommand* boxRedimensionCommand =new BoxRedimensionCommand(this, oldRect);
        undoStack->push(boxRedimensionCommand);
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void FootprintObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    updateTipString();
    setFlag(ItemIsMovable, true);
    m_resizeHandlePressed=false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void FootprintObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{  
    const Rectangle &rectangle = myGraphicsRect;
    m_resizeHandleHover = rectangle.resizeHandle().contains(event->pos());

    if (m_resizeHandleHover)
        handleBrush = QColor(0, 200, 0);
    else
        handleBrush = Qt::white;

    update (boundingRect());

    QGraphicsItem::hoverMoveEvent(event);
}

void FootprintObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    handleBrush = Qt::white;
    const Rectangle &rectangle = myGraphicsRect;
    this->update(rectangle.resizeHandle());
    QGraphicsItem::hoverLeaveEvent(event);
}

void FootprintObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    const Rectangle &rectangle = myGraphicsRect;
    m_resizeHandleHover = rectangle.resizeHandle().contains(event->pos());

    if (m_resizeHandleHover)
    {
        handleBrush = QColor(0, 200, 0);
        this->update(rectangle.resizeHandle());
    }

    QGraphicsItem::hoverEnterEvent(event);
}

void FootprintObject::setItemScale(const double &newScale)
{
    scale=newScale;
}

void FootprintObject::setLaserBeamPath(const QPainterPath& path)
{
    laserBeamPath=path;
}

void FootprintObject::setLaserEhancedBeamPath(const QPainterPath& path)
{
    laserEhnacedBeamPath=path;
}

void FootprintObject::setDNRO_Diameter(const double newRadius)
{
   if(2*newRadius==opticalDiameter)
       return;

    prepareGeometryChange();
    opticalDiameter=2*newRadius;
}

void FootprintObject::setFootprintSeqNumber(const int& _footprintSeqNumber)
{
    footprintSeqNumber=_footprintSeqNumber;
}

int FootprintObject::getFootprintSeqNumber()const
{
    return footprintSeqNumber;
}

void FootprintObject::laserPositionChanged()
{
    foreach (ObjectLink *itemlink, myObjectLinks)
    {
       itemDistance=itemlink->objectLinkLenght();
       itemlink->trackNodes();

       /**********************************************************************************************
        * In qusto caso, a differenza di itemChange, si fa riferimento alla scena quindi si impiega  *
        * mapToScene per riferire la posizione del rettangolo modificabile a sistema di riferimento  *
        * solidale alla scena.                                                                       *
        **********************************************************************************************/

       //laserposition=itemlink->fromLaser()->pos()-mapToScene(QPointF(0.0, 0.0));
       //laserposition=itemlink->fromLaser()->pos()-mapToScene(QPointF(0.0, 0.0));

       setLaserBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapePath()));
       setLaserEhancedBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapeEnhacedPath(ehnacedDiameter)));
       laserposition=itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->pos()-itemlink->fromLaser()->pos());
    }
    setShadowPath(shadowPath(opticalDiameter));
    setEhnacedShadowPath(shadowPath(ehnacedDiameter));
}

void FootprintObject::laserParameterChanged()
{
    foreach (ObjectLink *itemlink, myObjectLinks)
    {
       itemDistance=itemlink->objectLinkLenght();
       itemlink->trackNodes();
       setLaserEhancedBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapeEnhacedPath(ehnacedDiameter)));
       setLaserBeamPath(itemlink->fromLaser()->mapToItem(this, itemlink->fromLaser()->shapePath()));
     }
    setShadowPath(shadowPath(opticalDiameter));
    setEhnacedShadowPath(shadowPath(ehnacedDiameter));
}

void FootprintObject::setLaserPosition()
{
    foreach (ObjectLink *itemlink, myObjectLinks)
    {
     laserposition=itemlink->fromLaser()->mapToItem(this,
                   mapToScene(-pos()));
    }
}

/*************************************************************************************
 * con topLeftAngle topRightAngle, bottomLeftAngle, bottomRightAngle si indicano gli *
 * angoli riferiti ai vertici dei rettangolo così come indicati da QRectF            *
 *************************************************************************************/

double FootprintObject::topCornerAngle(const QPointF& _laserposition, const QRectF &_rect)
{
    QLineF rectTopLeftCorner;
    QLineF rectTopRightCorner;

    rectTopLeftCorner=QLineF(_laserposition, _rect.topLeft());
    double rectTopLeftCornerY=rectTopLeftCorner.dy();
    double topLeftCornerAngle=std::asin(rectTopLeftCornerY/rectTopLeftCorner.length())*degRad;

    rectTopRightCorner=QLineF(_laserposition, _rect.topRight());
    double rectTopRightCornerY=rectTopRightCorner.dy();
    double topRightCornerAngle=std::asin(rectTopRightCornerY/rectTopRightCorner.length())*degRad;

    double topCornerAngle=std::fmin(topLeftCornerAngle, topRightCornerAngle);
    return topCornerAngle;
}

double FootprintObject::bottomCornerAngle(const QPointF& _laserposition, const QRectF &_rect)
{
    QLineF rectBottomLeftCorner;
    QLineF rectBottomRightCorner;

    rectBottomLeftCorner=QLineF(_laserposition, _rect.bottomLeft());
    double rectBottomLeftCornerY=rectBottomLeftCorner.dy();
    double cornerBottomLeftAngle=std::asin(rectBottomLeftCornerY/rectBottomLeftCorner.length())*degRad;

    rectBottomRightCorner=QLineF(_laserposition, _rect.bottomRight());
    double rectBottomRightCornerY=rectBottomRightCorner.dy();
    double bottomCornerRightAngle=std::asin(rectBottomRightCornerY/rectBottomRightCorner.length())*degRad;

    double cornerBottomAngle=fmax(cornerBottomLeftAngle, bottomCornerRightAngle);
    return cornerBottomAngle;
}

QPainterPath FootprintObject::hazardZone(const QPointF& _laserposition, const QRectF& _rect)
{
    QPolygonF hazardPolygon;


    //if(addfreeCorner)
     //   {
        hazardPolygon.clear();
        hazardPolygon<<_laserposition<<  topCorner(_laserposition, _rect)<<freeCorner(_laserposition, _rect)<<
        bottomCorner(_laserposition, _rect)<<_laserposition;
     //   }
     //   else
     //   {
    //    hazardPolygon.clear();
    //    hazardPolygon<<_laserposition<<  topCorner(_laserposition, _rect)<<
     //   bottomCorner(_laserposition, _rect)<<_laserposition;
     //   }

    QPainterPath hazardPath;
    hazardPath.addPolygon(hazardPolygon);
    return hazardPath;
}

QPointF FootprintObject::bottomCorner(const QPointF& _laserposition, const QRectF &_rect)
{
    QLineF rectBottomLeftCorner;
    QLineF rectBottomRightCorner;
    QPointF bottomCorner;

    rectBottomLeftCorner=QLineF(_laserposition, _rect.bottomLeft());
    double rectBottomLeftCornerY=rectBottomLeftCorner.dy();
    double cornerBottomLeftAngle=std::asin(rectBottomLeftCornerY/rectBottomLeftCorner.length())*degRad;

    rectBottomRightCorner=QLineF(_laserposition, _rect.bottomRight());
    double rectBottomCornerY=rectBottomRightCorner.dy();
    double cornerBottomRightAngle=std::asin(rectBottomCornerY/rectBottomRightCorner.length())*degRad;

    if(cornerBottomLeftAngle>= cornerBottomRightAngle)
        bottomCorner=_rect.bottomLeft();
    else
        bottomCorner=_rect.bottomRight();

    return bottomCorner;
}

QPointF FootprintObject::topCorner(const QPointF& _laserposition, const QRectF &_rect)
{
    QLineF rectTopLeftCorner;
    QLineF rectTopRightCorner;
    QPointF topCorner;

    rectTopLeftCorner=QLineF(_laserposition, _rect.topLeft());
    double rectTopLeftCornerY=rectTopLeftCorner.dy();
    double topLeftCornerAngle=std::asin(rectTopLeftCornerY/rectTopLeftCorner.length())*degRad;

    rectTopRightCorner=QLineF(_laserposition, _rect.topRight());
    double rectTopRightCornerY=rectTopRightCorner.dy();
    double topRightCornerAngle=std::asin(rectTopRightCornerY/rectTopRightCorner.length())*degRad;


    if(topLeftCornerAngle>= topRightCornerAngle)
        topCorner=_rect.topRight();
    else
        topCorner=_rect.topLeft();

    return topCorner;
}

QRectF FootprintObject::rectangle() const
{
    return myGraphicsRect.rect();
}

void FootprintObject::setRectangle(const Rectangle &_rectangle)
{
    myGraphicsRect=_rectangle;
}

void FootprintObject::setCenter(const QPointF& _center)
{
    myGraphicsRect.rect().moveCenter(mapToItem(this, _center));
}

QPointF FootprintObject::freeCorner(const QPointF& _laserposition, const QRectF &_rect)
{
    QLineF rectTopLeftCorner;
    QLineF rectTopRightCorner;
    QPointF freeCorner;

    rectTopLeftCorner=QLineF(_laserposition, _rect.topLeft());
    double rectTopLeftCornerY=rectTopLeftCorner.dy();
    double topLeftCornerAngle=std::asin(rectTopLeftCornerY/rectTopLeftCorner.length())*degRad;

    rectTopRightCorner=QLineF(_laserposition, _rect.topRight());
    double rectTopRightCornerY=rectTopRightCorner.dy();
    double topRightCornerAngle=std::asin(rectTopRightCornerY/rectTopRightCorner.length())*degRad;

    QLineF rectBottomLeftCorner;
    QLineF rectBottomRightCorner;

    rectBottomLeftCorner=QLineF(_laserposition, _rect.bottomLeft());
    double rectBottomLeftCornerY=rectBottomLeftCorner.dy();
    double bottomLeftCornerAngle=std::asin(rectBottomLeftCornerY/rectBottomLeftCorner.length())*degRad;

    rectBottomRightCorner=QLineF(_laserposition, _rect.bottomRight());
    double rectBottomCornerY=rectBottomRightCorner.dy();
    double bottomRightCornerAngle=std::asin(rectBottomCornerY/rectBottomRightCorner.length())*degRad;


    if((topLeftCornerAngle<= topRightCornerAngle)&&(bottomLeftCornerAngle<= bottomRightCornerAngle))
    {
        freeCorner=_rect.bottomLeft();
        addfreeCorner=true;
    }
    else
    if((topLeftCornerAngle>= topRightCornerAngle)&&(bottomLeftCornerAngle>= bottomRightCornerAngle))
    {
        freeCorner=_rect.topLeft();
        addfreeCorner=true;
    }
    else
    {
        freeCorner=_rect.topLeft()+QPointF(0.0, _rect.width()/2);
        addfreeCorner=false;
    }
    return freeCorner;
}

void FootprintObject::setTipString()
{
    double xCoordinate;
    double yCoordinate;
    QRectF rectInScene=mapRectToScene(myGraphicsRect.rect());
    xCoordinate=rectInScene.x();
    yCoordinate=rectInScene.y();

    auto myFootprintRectangle=getRectangle();
    QRectF myRect=myFootprintRectangle.rect();
    double lenght=myRect.width();
    double height=myRect.height();

    QString xString;
    QString yString;
    QString lenghtString;
    QString heightString;
    int digit;
    if(scale<=16)
        digit=0;
       else
        digit=2;

    xString=QString::number(xCoordinate,'f', digit);
    yString=QString::number(yCoordinate,'f', digit);

    lenghtString=QString::number(lenght,'f',digit);
    heightString=QString::number(height,'f',digit);

    tipString= QString("Ingombro (%1,%2) \nlarghezza[m]: %3, altezza[m]: %4")
                                        .arg(xString)
                                        .arg(yString)
                                        .arg(lenghtString)
                                        .arg(heightString);

}

QPainterPath FootprintObject::shadowPath(const double &diameter)
{
    QRectF rectForShadow = myGraphicsRect.rect();
    //rectForShadow.adjust(1.0, 1.0, -resizeHandleWidth/2, -resizeHandleWidth/2);

    double myCornerTopAngle=topCornerAngle(laserposition, rectForShadow);
    double myCornerBottomAngle=bottomCornerAngle(laserposition, rectForShadow);

    QPainterPath myPie;

    myPie.moveTo(laserposition);
    QRectF myBoudingRect=QRectF(0.0, 0.0, diameter, diameter);
    myBoudingRect.moveCenter(laserposition);

    firstPie=360-myCornerBottomAngle;
    secondPie=myCornerBottomAngle-myCornerTopAngle;

    myPie.arcTo(myBoudingRect, firstPie, secondPie);
    myPie.moveTo(laserposition);

    return myPie=myPie.subtracted(hazardZone(laserposition, rectForShadow));
}

void FootprintObject::setShadowPath(QPainterPath path)
{
    foreach (ObjectLink *itemlink, myObjectLinks)
    {
        if(itemlink->objectLinkPhase_x()>=0)
        {
            shadowPathItem=laserBeamPath.intersected(path);
        }
        else
        {
            shadowPathItem.clear();
        }
    }
}
void FootprintObject::setEhnacedShadowPath(QPainterPath path)
{
    foreach (ObjectLink *itemlink, myObjectLinks)
    {
        if(itemlink->objectLinkPhase_x()>=0)
            ehnacedShadowPathItem=laserEhnacedBeamPath.intersected(path);
        else
            ehnacedShadowPathItem.clear();
    }
}

void FootprintObject::setEhnacedDiameter(const double& _ehnacedDiameter)
{
    if(_ehnacedDiameter==ehnacedDiameter)
        return;

     prepareGeometryChange();
     ehnacedDiameter=_ehnacedDiameter;
}

QPainterPath FootprintObject::getShadowPath()
{
    setShadowPath(shadowPath(opticalDiameter));
    return shadowPathItem;
}

QPainterPath FootprintObject::getEhnacedShadowPath()
{
    setEhnacedShadowPath(shadowPath(ehnacedDiameter));
    return ehnacedShadowPathItem;
}

void FootprintObject::handleRect(const Rectangle _rectangle)
{
    handle_rect=_rectangle.resizeHandle().adjusted(0, 0, -1/scale, -1/scale);
}

bool FootprintObject::getResizeHandlePressed()
{
    return m_resizeHandlePressed;
}

Rectangle FootprintObject::getRectangle()const
{
    return myGraphicsRect;
}

void FootprintObject::setTransform(QTransform &_transform)
{
    myTransform=_transform;
}

void FootprintObject::setDescription(QString _description)
{
    description=_description;
}

QString FootprintObject::getDescription()const
{
    return description;
}

void FootprintObject::updateTipString()
{
    setTipString();
    setToolTip(tipString);
}

QRectF FootprintObject::labelRect()
{
        const int Padding = 10;
        QFontMetricsF metrics =(QFontMetricsF)qApp->font();
        myTextRect = metrics.boundingRect(textLabel);
        myTextRect.adjust(-Padding, -Padding, +Padding, +Padding);

        const Rectangle &rectangle = myGraphicsRect;
        QPointF rectPoint=rectangle.rect().topLeft();
        myTextRect.moveBottomLeft(rectPoint);
        return myTextRect;
}

void FootprintObject::setTextLabel()
{
    double xCoordinate;
    double yCoordinate;
    QRectF rectInScene=mapRectToScene(myGraphicsRect.rect());
    xCoordinate=rectInScene.x();
    yCoordinate=rectInScene.y();

    QString xString;
    QString yString;

    int digit;
    if(scale<=16)
        digit=0;
       else
        digit=2;

    xString=QString::number(xCoordinate,'f', digit);
    yString=QString::number(yCoordinate,'f', digit);

    textLabel="Ingombro" + QString("\n(%1,%2)")
            .arg(xString)
            .arg(yString);
}

void FootprintObject::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
}
