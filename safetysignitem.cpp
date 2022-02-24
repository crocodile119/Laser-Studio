#include "safetysignitem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QColor>
#include <QGuiApplication>

SafetySignItem::SafetySignItem(SafetyClass _mySignSafety):mySignSafety(_mySignSafety)
{
    scale=1;
    if(mySignSafety==SafetyClass::FORBIDDEN_ACCESS)
        safetySign=QPixmap(":/images/forbidden_sign.png");
    else if(mySignSafety==SafetyClass::LASER_RADIATIONS)
        safetySign=QPixmap(":/images/laser_sign.png");
    else if(mySignSafety==SafetyClass::EYES_PROTECTION)
        safetySign=QPixmap(":/images/glass_sign.png");

    myOutlineColor = Qt::transparent;
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

void SafetySignItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem  * option,
                          QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        pen.setColor(Qt::darkGray);
    }
    pen.setCosmetic(true);
    painter->setPen(pen);

    QRectF pixRect=outlineRect();
    painter->drawRect(pixRect);
    QRectF source(0.0, 0.0, safetySign.width(), safetySign.height());
    painter->drawPixmap(pixRect, safetySign, source);
    update();
}

QRectF SafetySignItem::outlineRect() const
{
    double rectWidht=safetySign.width()/scale;
    double rectHeight=safetySign.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF center=rect.center();
    rect.translate(-center);
    return rect;
}

void SafetySignItem::setPixScale(const double &newScale)
{
    scale=newScale;
}

QRectF SafetySignItem::boundingRect() const
{
    return outlineRect();
}

SafetySignItem::SafetyClass SafetySignItem::SafetySignKind()
{
    return mySignSafety;
}

QPainterPath SafetySignItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
