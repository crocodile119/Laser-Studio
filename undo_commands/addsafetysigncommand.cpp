#include "addsafetysigncommand.h"

AddSafetySignCommand::AddSafetySignCommand(double *_scale, SafetySignItem::SafetyClass _mySafetySign,
                                           CentralWidget *_laserWindow, QList<SafetySignItem*>*_safetySignList,
                                           QPointF _laserPos, QUndoCommand *parent)
                      : QUndoCommand(parent), scale(_scale), mySafetySign(_mySafetySign),
                        laserWindow(_laserWindow), safetySignList(_safetySignList), laserPos(_laserPos)
{
    safetySign = new SafetySignItem(mySafetySign);
    safetySign->setPos(laserPos);
    setText(QObject::tr("Aggiungo %1")
        .arg(createAddSafetySignCommandString(mySafetySign, laserPos)));
}

void AddSafetySignCommand::undo()
{
    laserWindow->graphicsView->scene()->removeItem(safetySign);
    laserWindow->graphicsView->scene()->update();

    safetySignList->clear();

    QList<QGraphicsItem *> items = laserWindow->graphicsView->scene()->items();

    QMutableListIterator<QGraphicsItem *> k(items);
    while (k.hasNext())
    {
        SafetySignItem *undoSafetySign = dynamic_cast<SafetySignItem *>(k.next());
        if (undoSafetySign)
        {
        safetySignList->push_back(undoSafetySign);
        }
    }
}

void AddSafetySignCommand::redo()
{
    safetySign->setPixScale(*scale);
    safetySignList->push_back(safetySign);
    laserWindow->graphicsView->scene()->addItem(safetySign);
    laserWindow->graphicsView->scene()->clearSelection();
    safetySign->setSelected(true);
}

AddSafetySignCommand::~AddSafetySignCommand()
{
    if(!laserWindow->graphicsView->scene())
        delete safetySign;
}

QString createAddSafetySignCommandString(SafetySignItem::SafetyClass mySafetySign, const QPointF &laserPos)
{
    QString safetySignString;
    if(mySafetySign==SafetySignItem::SafetyClass::FORBIDDEN_ACCESS)
        safetySignString="divieto di accesso";
    else if(mySafetySign==SafetySignItem::SafetyClass::LASER_RADIATIONS)
        safetySignString="radiazione laser";
    else if(mySafetySign==SafetySignItem::SafetyClass::EYES_PROTECTION)
        safetySignString="protezione oculare";

    QString xpos=QString::number(laserPos.x());
    QString ypos=QString::number(laserPos.y());

    return QObject::tr("cartello %1 in (%2, %3)")
        .arg(safetySignString)
        .arg(xpos)
        .arg(ypos);
}
