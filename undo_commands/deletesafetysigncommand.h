#ifndef DELETESAFETYSIGNCOMMAND_H
#define DELETESAFETYSIGNCOMMAND_H

#include <QUndoCommand>

#include "../safetysignitem.h"
#include "centralwidget.h"

class DeleteSafetySignCommand : public QUndoCommand
{
public:
    DeleteSafetySignCommand(SafetySignItem *_safetySign, double *scale, CentralWidget *_laserWindow, QList<SafetySignItem *> *_safetySignList,
                   QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteSafetySignCommand();

    void undo() override;
    void redo() override;

private:
    SafetySignItem* safetySign;
    double *scale;
    CentralWidget *laserWindow;
    QList <SafetySignItem *>*safetySignList;
    QPointF deletePosition;
};

QString createDeleteCommandString(SafetySignItem::SafetyClass mySafetySign);


#endif // DELETESAFETYSIGNCOMMAND_H
