#ifndef ADDINSPECTORPROPERTYCOMMAND_H
#define ADDINSPECTORPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../beaminspector.h"

class AddInspectorPropertyCommand : public QUndoCommand
{
public:
    AddInspectorPropertyCommand(BeamInspector* _beamInspector, QPointF _oldPosition, QString _oldDescriptionTextEdit,
                                QUndoCommand *parent = nullptr);

    ~AddInspectorPropertyCommand();

    void undo() override;
    void redo() override;

private:
BeamInspector* beamInspector;

QPointF position;
QString descriptionTextEdit;

QPointF oldPosition;
QString oldDescriptionTextEdit;
};

QString redoInspectorPropertyString(BeamInspector *beamInspector, QPointF position, QString description);

QString undoInspectorPropertyString(QPointF position, QString description, QPointF oldPosition, QString oldDescription);

#endif
