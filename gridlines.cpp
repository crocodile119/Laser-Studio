#include "gridlines.h"
#include <QColor>
#include <QDebug>
#include <QFontMetricsF>
#include <QGuiApplication>

const double GridLines::GRID_STEP= 100.0;

GridLines::GridLines() : QGraphicsItem(), step(GRID_STEP), referencePix(":/images/referencepix.png")
{
   setFlag(QGraphicsItem::ItemStacksBehindParent);
}

void GridLines::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF pixRect=outlineRect();

    QRectF source(0.0, 0.0, referencePix.width(), referencePix.height());
    painter->drawPixmap(pixRect, referencePix, source);

    QPen gridPen;
    gridPen.setColor(Qt::black);
    gridPen.setStyle(Qt::DotLine);
    gridPen.setCosmetic(true);
    painter->setPen(gridPen);

    double width=sceneRect.width();
    double height=sceneRect.height();

    painter->drawRect(sceneRect);

    for (int y= 0; y < height/2; y+=step)
    {
        painter->drawLine(0, y, width/2, y);
        painter->drawLine(0, y, -width/2, y);
        painter->drawLine(0, -y, -width/2, -y);
        painter->drawLine(0, -y, width/2, -y);
    }

    for (int x=0; x < width/2; x+=step)
    {
        painter->drawLine(x, 0, x, height/2);
        painter->drawLine(x, 0, x, -height/2);
        painter->drawLine(-x, 0, -x, -height/2);
        painter->drawLine(-x, 0, -x, height/2);
    }

    update(boundingRect());

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
    QRectF boundingRect=labelRect();
    painter->setTransform(myTransform);

    QPen textPen;
    textPen.setColor(Qt::black);
    painter->setPen(textPen);
    painter->drawText(boundingRect, Qt::AlignLeft, textLabel, &boundingRect);
    painter->drawRect(boundingRect.adjusted(-10, -10, 10, 10));
    painter->drawRect(boundingRect.adjusted(-15, -15, 15, 15));

    painter->resetTransform();
    painter->restore();
    painter->setTransform(transform);

    update();
}

QRectF GridLines::boundingRect() const
{
    return sceneRect;
}

void GridLines::setSceneRect(const QRectF& _sceneRect)
{
    sceneRect=_sceneRect;
}

void GridLines::setScale(const double& _newScale)
{
    scale=_newScale;
}

void GridLines::setGridLines(const double& _step)
{
    step=_step;
}

QRectF GridLines::labelRect()const
{
    const int Padding = 1;
    QFontMetricsF metrics =(QFontMetricsF)qApp->font();
    QRectF myTextRect = metrics.boundingRect(textLabel);
    myTextRect.adjust(-Padding, -Padding, +Padding, +Padding);
    const QPointF &myPoint=zeroViewportPoint;
    myTextRect.moveBottomRight(myPoint);

    return myTextRect;
}

void GridLines::setTopLeftViewport(const QPointF& _zeroViewportPoint)
{
    zeroViewportPoint=_zeroViewportPoint;
}

void GridLines::setTextLabel(const QString& text)
{
    prepareGeometryChange();
    textLabel =text;
    update();
}

void GridLines::setTextColor(const QColor& color)
{
    myTextColor = color;
    update();
}

QRectF GridLines::outlineRect() const
{
    double rectWidht=referencePix.width()/scale;
    double rectHeight=referencePix.height()/scale;
    QRectF rect=QRectF(0, 0, rectWidht, rectHeight);
    QPointF center=QPointF(rectWidht/4, rectHeight/4);
    rect.translate(-center);
    return rect;
}
