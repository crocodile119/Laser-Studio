#ifndef ADDREFLECTORPROPERTYCOMMAND_H
#define ADDREFLECTORPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../reflector.h"

class AddReflectorPropertyCommand : public QUndoCommand
{
public:
    AddReflectorPropertyCommand(Reflector* _reflector, QPointF _position, double _reflection, int _positioning, QString _descriptionTextEdit,
                                QUndoCommand *parent = nullptr);

    ~AddReflectorPropertyCommand();

    void undo() override;
    void redo() override;

private:
Reflector* reflector;

QPointF position;
double reflection;
int positioning;
QString descriptionTextEdit;


QPointF oldPosition;
double oldReflection;
int oldPositioning;
QString oldDescriptionTextEdit;


};

#endif
