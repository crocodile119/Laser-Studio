#include "deletesafetysigncommand.h"

DeleteSafetySignCommand::DeleteSafetySignCommand(SafetySignItem* _safetySign, double *_scale, CentralWidget *_laserWindow,
                             QList<SafetySignItem *> *_safetySignList, QPointF _deletePosition, QUndoCommand *parent)
          : QUndoCommand(parent), safetySign(_safetySign), scale(_scale), laserWindow(_laserWindow), safetySignList(_safetySignList),
            deletePosition(_deletePosition)
{
    setText(QObject::tr("Elimino %1")
        .arg(createDeleteCommandString(safetySign->SafetySignKind())));
}

DeleteSafetySignCommand::~DeleteSafetySignCommand()
{

}

void DeleteSafetySignCommand::undo()
{
    safetySign->setPixScale(*scale);
    safetySignList->push_back(safetySign);
    laserWindow->graphicsView->scene()->addItem(safetySign);
    safetySign->setPos(deletePosition);
}

void DeleteSafetySignCommand::redo()
{
    laserWindow->graphicsView->scene()->removeItem(safetySign);
    safetySignList->pop_back();
}

QString createDeleteCommandString(SafetySignItem::SafetyClass mySafetySign)
{
    QString safetySignString;
    if(mySafetySign==SafetySignItem::SafetyClass::FORBIDDEN_ACCESS)
        safetySignString="divieto di accesso";
    else if(mySafetySign==SafetySignItem::SafetyClass::LASER_RADIATIONS)
        safetySignString="radiazione laser";
    else if(mySafetySign==SafetySignItem::SafetyClass::EYES_PROTECTION)
        safetySignString="protezione oculare";

    return QObject::tr("cartello %1")
        .arg(safetySignString);
}
