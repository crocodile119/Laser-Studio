#include "addfootprintpropertycommand.h"

AddFootprintPropertyCommand::AddFootprintPropertyCommand(FootprintObject* _footprint, double _x, double _y, double _rectWidth,
                                                         double _rectHeight, QString _description, QUndoCommand *parent)
                            : QUndoCommand(parent), footprint(_footprint), x(_x), y(_y),
                              rectWidth(_rectWidth), rectHeight(_rectHeight), description(_description)
{
    auto myFootprintRectangle=footprint->getRectangle();
    QRectF myRect=myFootprintRectangle.rect();
    QPointF myFootprintCenter=myRect.topLeft();
    QPointF center=footprint->mapToScene(myFootprintCenter);
    old_x=center.x();
    old_y=center.y();

    old_rectWidth=myRect.width();
    old_rectHeight=myRect.height();
    old_description=footprint->getDescription();

    setText(QObject::tr("Modifico le proprietà dell'ingombro"));
}

void AddFootprintPropertyCommand::undo()
{
    QRectF myFootprintRect=QRectF(0, 0, old_rectWidth, old_rectHeight);

    auto myRectangle=footprint->getRectangle();
    myRectangle.setRect(myFootprintRect);

    footprint->setRectangle(myRectangle);
    footprint->setPos(old_x, old_y);
    footprint->setDescription(old_description);
    footprint->updateTipString();
}

void AddFootprintPropertyCommand::redo()
{
    QRectF myFootprintRect=QRectF(0, 0, rectWidth, rectHeight);

    auto myRectangle=footprint->getRectangle();
    myRectangle.setRect(myFootprintRect);

    footprint->setRectangle(myRectangle);
    footprint->setPos(x, y);
    footprint->setDescription(description);
    footprint->updateTipString();
}

AddFootprintPropertyCommand::~AddFootprintPropertyCommand()
{

}

