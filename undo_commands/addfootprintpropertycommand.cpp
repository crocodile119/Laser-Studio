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
    setText(undoFootprintPropertyString(old_x, old_y, old_rectWidth, old_rectHeight,
                                        old_description, x, y, rectWidth, rectHeight,
                                        description));
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

    setText(redoFootprintPropertyString(footprint, old_x, old_y, old_rectWidth, old_rectHeight,
                                        old_description));

}

AddFootprintPropertyCommand::~AddFootprintPropertyCommand()
{

}

QString redoFootprintPropertyString(FootprintObject *footprint, double old_x, double old_y,
                                    double old_rectWidth, double old_rectHeight, QString oldDescription)
{
    auto myFootprintRectangle=footprint->getRectangle();
    QRectF myRect=myFootprintRectangle.rect();
    QPointF myFootprintCenter=myRect.topLeft();
    QPointF center=footprint->mapToScene(myFootprintCenter);
    double x=center.x();
    double y=center.y();

    double rectWidth=myRect.width();
    double rectHeight=myRect.height();
    QString description=footprint->getDescription();

    QString positionString;
    QString dimensionString;
    QString descriptionString;
    QString actionString;

    if((x!=old_x)||(y!=old_y))
        positionString=QString(" Posizione (%1, %2)" )
                .arg(x)
                .arg(y);

    if ((old_rectWidth!=rectWidth)||(old_rectHeight!=rectHeight))
        dimensionString=QString("Dimensioni {larghezza= %1, altezza=%2}")
                .arg(rectWidth)
                .arg(rectHeight);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((x==old_x)&&(y==old_y)&&(rectWidth==old_rectWidth)&&(rectHeight==old_rectHeight)
            &&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico lo strumento ottico: %1%2%3%4")
        .arg(positionString)
        .arg(dimensionString)
        .arg(descriptionString)
        .arg(actionString);
}

QString undoFootprintPropertyString(double old_x, double old_y, double old_rectWidth, double old_rectHeight,
                                    QString oldDescription, double x, double y, double rectWidth,
                                    double rectHeight, QString description)
{
    QString positionString;
    QString dimensionString;
    QString descriptionString;
    QString actionString;

    if((x!=old_x)||(y!=old_y))
        positionString=QString(" Posizione (%1, %2)" )
                .arg(x)
                .arg(y);

    if ((old_rectWidth!=rectWidth)||(old_rectHeight!=rectHeight))
        dimensionString=QString("Dimensioni {larghezza= %1, altezza=%2}")
                .arg(rectWidth)
                .arg(rectHeight);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((x==old_x)&&(y==old_y)&&(rectWidth==old_rectWidth)&&(rectHeight==old_rectHeight)
            &&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico lo strumento ottico: %1%2%3%4")
        .arg(positionString)
        .arg(dimensionString)
        .arg(descriptionString)
        .arg(actionString);
}
