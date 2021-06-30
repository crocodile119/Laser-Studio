#include "addlabpropertycommand.h"

AddLabPropertyCommand::AddLabPropertyCommand(LabRoom* _myLabRoom, double _x, double _y,
                                             double _rectWidth, double _rectHeight,
                        QUndoCommand *parent)
                      : QUndoCommand(parent), myLabRoom(_myLabRoom),
                        x(_x), y(_y), rectWidth(_rectWidth), rectHeight(_rectHeight)
{
    old_x=myLabRoom->pos().x();
    old_y=myLabRoom->pos().y();
    QRectF myLabRect= myLabRoom->getRoomRect();
    old_rectWidth=myLabRect.width();
    old_rectHeight=myLabRect.height();

    setText(QObject::tr("Modifico le proprietà dell laboratorio"));
}

void AddLabPropertyCommand::undo()
{   
    QRectF labRect=QRectF(0, 0, old_rectWidth, old_rectHeight);
    QPointF center= QPointF(old_rectWidth/2, old_rectHeight/2);
    labRect.translate(-center);

    QPointF labRectPos=QPointF(old_x, old_y);

    myLabRoom->setRoomRect(labRect);
    myLabRoom->setPos(labRectPos);
}

void AddLabPropertyCommand::redo()
{
    QRectF labRect=QRectF(0, 0, rectWidth, rectHeight);
    QPointF center= QPointF(rectWidth/2, rectHeight/2);
    labRect.translate(-center);

    QPointF labRectPos=QPointF(x, y);

    myLabRoom->setRoomRect(labRect);
    myLabRoom->setPos(labRectPos);
}

AddLabPropertyCommand::~AddLabPropertyCommand()
{

}

