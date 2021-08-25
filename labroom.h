#ifndef LABROOM_H
#define LABROOM_H

#include <QApplication>
#include <QColor>
#include <QGraphicsObject>
#include <QPixmap>
#include <QSet>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>


class LabRoom : public QGraphicsObject
{
    Q_DECLARE_TR_FUNCTIONS(LabRoom)

public:
    LabRoom(QRectF);
    ~LabRoom();

    enum { Type = UserType + 3 };
    const static double radDeg;
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void setLabRectExt();
    QRectF getLabRectExt() const;

    int type() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)override;

    void setTextLabel();
    QString getTextLabel()const;

    void setRoomRect(const QRectF& _labRect);
    QRectF getRoomRect() const;
    void setPixScale(const double&);

private slots:
    //QRectF resizeHandle() const;

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value)override;

   // void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
   // void mousePressEvent(QGraphicsSceneMouseEvent *event)override;

private:
    QRectF outlineRect() const;
    QRectF labelRect();
    QRectF unitedBounding() const;
    QRectF labRectExt;
    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    QString textLabel;
    double scale; 
    QRectF myTextRect;
    QRectF labRect;
  //QPointF m_mousePressOffset;
  //bool m_resizeHandlePressed = false;
};

#endif
