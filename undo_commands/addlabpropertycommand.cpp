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
}

void AddLabPropertyCommand::undo()
{   
    QRectF labRect=QRectF(0, 0, old_rectWidth, old_rectHeight);
    QPointF center= QPointF(old_rectWidth/2, old_rectHeight/2);
    labRect.translate(-center);

    QPointF labRectPos=QPointF(old_x, old_y);

    myLabRoom->setRoomRect(labRect);
    myLabRoom->setPos(labRectPos);
    setText(undoLabPropertyString(x, y, rectWidth, rectHeight,
                                  old_x, old_y,old_rectWidth, old_rectHeight));
}

void AddLabPropertyCommand::redo()
{
    QRectF labRect=QRectF(0, 0, rectWidth, rectHeight);
    QPointF center= QPointF(rectWidth/2, rectHeight/2);
    labRect.translate(-center);

    QPointF labRectPos=QPointF(x, y);

    myLabRoom->setRoomRect(labRect);
    myLabRoom->setPos(labRectPos);
    setText(redoLabPropertyString(myLabRoom, old_x, old_y,
                                  old_rectWidth, old_rectHeight));
}

AddLabPropertyCommand::~AddLabPropertyCommand()
{

}

QString redoLabPropertyString(LabRoom *myLabRoom, double old_x, double old_y,
                              double old_rectWidth, double old_rectHeight)
{
    QPointF position= myLabRoom->pos();
    QRectF myLabRect= myLabRoom->getRoomRect();
    double rectWidth=myLabRect.width();
    double rectHeight=myLabRect.height();

    QString positionString;
    QString dimensionString;
    QString actionString;

    if((position.x()!=old_x)||(position.x()!=old_x))
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if ((old_rectWidth!=rectWidth)||(old_rectHeight!=rectHeight))
        dimensionString=QString("Dimensioni (larghezza= %1, altezza=%2)")
                .arg(rectWidth)
                .arg(rectHeight);

    if((old_x==position.x())&&(old_y==position.y())&&(rectWidth==old_rectWidth)
        &&(rectHeight==old_rectHeight))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il laboratorio: %1%2%3")
        .arg(positionString)
        .arg(dimensionString)
        .arg(actionString);
}

QString undoLabPropertyString(double x, double y, double rectWidth, double rectHeight,
                              double old_x, double old_y, double old_rectWidth, double old_rectHeight)
{
    QString positionString;
    QString dimensionString;
    QString actionString;

    if((x!=old_x)||(x!=old_x))
        positionString=QString(" Posizione (%1, %2)" )
                .arg(x)
                .arg(y);

    if ((old_rectWidth!=rectWidth)||(old_rectHeight!=rectHeight))
        dimensionString=QString("Dimensioni (larghezza= %1, altezza=%2)")
                .arg(rectWidth)
                .arg(rectHeight);

    if((old_x==x)&&(old_y==y)&&(rectWidth==old_rectWidth)
        &&(rectHeight==old_rectHeight))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il laboratorio: %1%2%3")
        .arg(positionString)
        .arg(dimensionString)
        .arg(actionString);
}
