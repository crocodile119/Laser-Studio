#ifndef SAFETYSIGNITEM_H
#define SAFETYSIGNITEM_H
#include <QGraphicsItem>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>

class SafetySignItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 11 };
    enum class SafetyClass{FORBIDDEN_ACCESS, LASER_RADIATIONS, EYES_PROTECTION, SKIN_PROTECTION};
    SafetySignItem(SafetyClass _mySignSafety);
    ~SafetySignItem();
    void setPixScale(const double &newScale);
    int type() const override;

    SafetySignItem::SafetyClass SafetySignKind();
    QString getSafetySignKind()const;

protected:


private:
    QRectF outlineRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    SafetyClass mySignSafety;
    QPixmap safetySign;
    double scale;
    QColor myOutlineColor;
};


#endif // SAFETYSIGNITEM_H
