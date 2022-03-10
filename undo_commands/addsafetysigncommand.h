#ifndef ADDSAFETYSIGNCOMMAND_H
#define ADDSAFETYSIGNCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../safetysignitem.h"
#include "centralwidget.h"
#include "laserpoint.h"


class AddSafetySignCommand : public QUndoCommand
{
public:
    AddSafetySignCommand(double *scale, SafetySignItem::SafetyClass _mySafetySign, CentralWidget *_laserWindow,
                         QList<SafetySignItem *> *_safetySignList, QPointF _laserPos,
                         QUndoCommand *parent = nullptr);

    ~AddSafetySignCommand();

    void undo() override;
    void redo() override;
    Reflector* getReflector() const;

private:
    double *scale;
    SafetySignItem::SafetyClass mySafetySign;
    SafetySignItem *safetySign=nullptr;
    CentralWidget *laserWindow;
    QList<SafetySignItem *> *safetySignList;
    QPointF laserPos;

};

QString createAddSafetySignCommandString(SafetySignItem::SafetyClass mySafetySign, const QPointF &laserPos);

#endif
