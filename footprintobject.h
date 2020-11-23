#ifndef FOOTPRINTOBJECT_H
#define FOOTPRINTOBJECT_H

#include <QRectF>

class Rectangle
{
    friend class FootprintObject;
    friend class MainWindow;
    friend class CentralWidget;
public:
    Rectangle(qreal=0.0, qreal=0.0, qreal=20.0, qreal=20.0);
    QRectF resizeHandle() const;
    void setItemScale(const double &newScale);
    static const QSizeF minSize;
    QRectF rect()const;
    void setRect(const QRectF &rect);
    void shadowPath();

private:
    QRectF m_rect;
};

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>
#include "objectlink.h"
#include "laserpoint.h"

class FootprintObject : public QGraphicsObject
{
        Q_DECLARE_TR_FUNCTIONS(FootprintObject)
public:
    enum { Type = UserType + 5 };
    FootprintObject(double _scale);
    ~FootprintObject();

    static void setResizeHandle(double newScale);
    QRectF boundingRect() const override;
    static const double degRad;
    static const double radDeg;
    int type() const override;
    void addObjectLink(ObjectLink *objectlink);
    void removeObjectLink(ObjectLink *objectlink);
    double topCornerAngle(const QPointF&, const QRectF &);
    double bottomCornerAngle(const QPointF&, const QRectF &);
    QPointF topCorner(const QPointF&, const QRectF &);
    QPointF bottomCorner(const QPointF&, const QRectF &);
    QPointF freeCorner(const QPointF&, const QRectF &);
    QPainterPath hazardZone(const QPointF &_laserposition, const QRectF &_rect);
    QRectF rectangle()const;
    void setRectangle(const class Rectangle &);
    void setCenter(const QPointF &_center);
    class Rectangle getRectangle()const;
    void setTipString();
    QPainterPath shadowPath(const double &);
    QPainterPath getShadowPath();
    QPainterPath getEhnacedShadowPath();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QRectF resizeHandle();
    static const QSizeF minSize;
    void setLaserBeamPath(const QPainterPath&);
    void setLaserEhancedBeamPath(const QPainterPath&);   
    void setShadowPath(QPainterPath path);
    void setEhnacedShadowPath(QPainterPath path);
    void setTransform(QTransform &);

    void setItemScale(const double&);
    void setDNRO_Diameter(const double);
    void setEhnacedDiameter(const double&);
    void setFootprintSeqNumber(const int&);
    int getFootprintSeqNumber()const;
    void laserPositionChanged();
    void laserParameterChanged();
    void setLaserPosition();
    QRectF outlineRect()const;
    void handleRect(const class Rectangle _rectangle);
    bool getResizeHandlePressed();
    void setResizeHandle();    
    void setDescription(QString);
    QString getDescription()const;
    void updateTipString();
    QRectF labelRect();
    void setTextLabel();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent (QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent (QGraphicsSceneHoverEvent *event)override;
    void hoverMoveEvent (QGraphicsSceneHoverEvent *event)override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value)override;

private:
    QRectF handle_rect;
    QSet<ObjectLink *> myObjectLinks;
    bool m_resizeHandlePressed = false;
    bool m_resizeHandleHover=false;
    QPointF m_mousePressOffset;
    QPainterPath laserBeamPath;   
    QPainterPath laserEhnacedBeamPath;
    class Rectangle myGraphicsRect;
    QPolygonF myZone;
    QColor m_color;
    QColor handleBrush;
    int footprintSeqNumber;
    double scale;
    double opticalDiameter;
    double ehnacedDiameter;
    double itemDistance;
    double firstPie;
    double secondPie;
    bool addfreeCorner;
    QString tipString;
    QString textLabel;

    QPointF laserposition;
    //LaserPoint * laserpoint;
    QLineF rectTopCorner;
    QLine rectBottomCorner;
    QRectF rect;
    QRectF myTextRect;
    QPainterPath shadowPathItem;
    QPainterPath ehnacedShadowPathItem;
    QPainterPath pieHazardPath;
    QTransform myTransform;
    QString description;
};
#endif
