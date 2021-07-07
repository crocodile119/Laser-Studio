#include "boxredimensioncommand.h"
#include <QGraphicsScene>

static constexpr int boxRedimensionCommandId = 30;

BoxRedimensionCommand::BoxRedimensionCommand(FootprintObject *_footprint, QRectF _old_rect, QUndoCommand *parent)
    : QUndoCommand(parent),  footprint(_footprint), old_rect(_old_rect)
{
    auto myRectangle=footprint->getRectangle();
    rect=myRectangle.rect();
}

bool BoxRedimensionCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != boxRedimensionCommandId)
        return false;

    const BoxRedimensionCommand *other = static_cast<const BoxRedimensionCommand*>(command);

    rect = other->old_rect;
    return true;
}

void BoxRedimensionCommand::undo()
{
    auto myRectangle=footprint->getRectangle();
    myRectangle.setRect(old_rect);

    footprint->setRectangle(myRectangle);
    footprint->updateTipString();
    footprint->scene()->update();

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

int BoxRedimensionCommand::id() const
{
    return boxRedimensionCommandId;
}

QString createBoxEditCommandString(QRectF rect)
{
    QString myString;
    myString=QObject::tr("Ho modificato l'ingombro: dimensioni {%1, %2}")
            .arg(rect.width())
            .arg(rect.height());

    return myString;
}
