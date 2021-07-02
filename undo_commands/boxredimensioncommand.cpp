#include "boxredimensioncommand.h"
#include <QGraphicsScene>

BoxRedimensionCommand::BoxRedimensionCommand(FootprintObject *_footprint, QRectF _old_rect, QUndoCommand *parent)
    : QUndoCommand(parent),  footprint(_footprint), old_rect(_old_rect)
{
    auto myRectangle=footprint->getRectangle();
    rect=myRectangle.rect();
}

void BoxRedimensionCommand::undo()
{
    auto myRectangle=footprint->getRectangle();
    myRectangle.setRect(old_rect);

    footprint->setRectangle(myRectangle);
    footprint->updateTipString();

    setText(createBoxEditCommandString(rect));
}

void BoxRedimensionCommand::redo()
{

    auto myRectangle=footprint->getRectangle();
    myRectangle.setRect(rect);

    footprint->setRectangle(myRectangle);
    footprint->updateTipString();

    setText(createBoxEditCommandString(rect));
}

QString createBoxEditCommandString(QRectF rect)
{
    QString myString;
    myString=QObject::tr("Ho modificato l'ingombro: dimensioni {%1, %2}")
            .arg(rect.width())
            .arg(rect.height());

    return myString;
}
