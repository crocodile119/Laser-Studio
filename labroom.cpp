#include <QtGui>
#include <QFont>
#include <QFontMetricsF>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGuiApplication>
#include <QPainter>
#include <cmath>

#include "labroom.h"


const double LabRoom::radDeg = 3.1415926535897932384626433832795/180;

LabRoom::LabRoom(QRectF _labRect):QGraphicsObject()
{
    myTextColor = Qt::black;
    myOutlineColor = QColor(0, 200, 0);
    myBackgroundColor = Qt::white;
    scale=80.0;
    labRect=_labRect;
    setTextLabel();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
}

LabRoom::~LabRoom()
{

}

int LabRoom::type() const
{
    return Type;
}

QString LabRoom::text() const
{
    return myText;
}

void LabRoom::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor LabRoom::textColor() const
{
    return myTextColor;
}

void LabRoom::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor LabRoom::outlineColor() const
{
    return myOutlineColor;
}

void LabRoom::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor LabRoom::backgroundColor() const
{
    return myBackgroundColor;
}

QRectF LabRoom::boundingRect() const
{
    const int Margin = 1;
    QRectF rect=unitedBounding();


    return rect.adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath LabRoom::shape() const
{
    QRectF rect= outlineRect();
    QPainterPath path;
    path.addRect(rect);

    return path;
}

void LabRoom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }

    pen.setCosmetic(true);

    QPen textPen(myTextColor);
    textPen.setCosmetic(true);

    painter->setPen(pen);

    prepareGeometryChange();

    setLabRectExt();
    setTextLabel();
    painter->drawRect(labRect);
    painter->drawRect(labRectExt);


    QTransform transform=painter->transform();

    double m13=transform.m13();
    double m23=transform.m23();
    double m32=transform.m32();
    double m31=transform.m31();
    double m21=transform.m21();
    double m12=transform.m12();

    painter->save();
    painter->resetTransform();
    QTransform myTransform=QTransform(1.0, m12, m13, m21, 1.0, m23, m31, m32, 1.0);

    painter->setTransform(myTransform);
    QRectF boundingRect=labelRect();
    painter->setPen(textPen);

    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);

    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

QVariant LabRoom::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        setTextLabel();
        return pos();
    }

    return QGraphicsItem::itemChange(change, value);
}

QRectF LabRoom::outlineRect() const
{
    double width=labRectExt.width();
    double height=labRectExt.height();
    QRectF rect=QRectF(0.0, 0.0, width, height);
    QPointF center=rect.center();
    rect.translate(-center);
    return rect;
}

QString LabRoom::getTextLabel()const
{
    return textLabel;
}

void LabRoom::setTextLabel()
{
    double xCoordinate;
    double yCoordinate;

    double labRectWidth=labRect.width();
    double labRectHeight=labRect.height();


    xCoordinate=pos().x();//+labRect.width();
    yCoordinate=pos().y();//+labRect.height();

    QString xString;
    QString yString;
    QString labRectWidthString;
    QString labRectHeightString;

    xString=QString::number(xCoordinate,'f', 1);
    yString=QString::number(yCoordinate,'f', 1);
    labRectWidthString=QString::number(labRectWidth,'f', 1);
    labRectHeightString=QString::number(labRectHeight,'f', 1);

    prepareGeometryChange();
    textLabel ="laboratorio "+QString("(%1,%2) larghezza [m] %3, lunghezza [m] %4")
                                    .arg(xString)
                                    .arg(yString)
                                    .arg(labRectWidthString)
                                    .arg(labRectHeightString);
    update();
}

QRectF LabRoom::labelRect()
{
    const int Padding = 10;
    QFontMetricsF metrics =(QFontMetricsF)qApp->font();
    myTextRect = metrics.boundingRect(textLabel);
    myTextRect.adjust(-Padding, -Padding, +Padding, +Padding);
    QPointF rectPoint((-labRectExt.width())*(1-scale)/2-Padding, labRectExt.height()*(1-scale)/2-myTextRect.height()/2-Padding);
    myTextRect.translate(-rectPoint);

    return myTextRect;
}

void LabRoom::setRoomRect(const QRectF& _labRect)
{
    labRect=_labRect;
}

QRectF LabRoom::getRoomRect() const
{
    return labRect;
}

void LabRoom::setLabRectExt()
{
    labRectExt=labRect.adjusted(-0.10, -0.10, 0.10, 0.10);
}

QRectF LabRoom::getLabRectExt() const
{
    return labRectExt;
}

QRectF LabRoom::unitedBounding() const
{
    QRectF unitedRect=outlineRect();
    unitedRect.adjust(-0.1*outlineRect().width(), -0.1*outlineRect().height(),
                      1.2*outlineRect().width(), 1.2*outlineRect().height());
    return unitedRect;
}

void LabRoom::setPixScale(const double& _scale)
{
    scale=_scale;
}
