#include "addinspectorpropertycommand.h"
#include <QDebug>

AddInspectorPropertyCommand::AddInspectorPropertyCommand(BeamInspector* _beamInspector, QPointF _oldPosition,
                         QString _oldDescriptionTextEdit, QUndoCommand *parent)
                      : QUndoCommand(parent), beamInspector(_beamInspector), oldPosition(_oldPosition),
                                              oldDescriptionTextEdit(_oldDescriptionTextEdit)
{
    position=beamInspector->pos();
    descriptionTextEdit=beamInspector->getDescription();
}

void AddInspectorPropertyCommand::undo()
{
    beamInspector->setPos(oldPosition);
    beamInspector->setDescription(oldDescriptionTextEdit);
    beamInspector->laserParametersChanged();
        setText(undoInspectorPropertyString(position, descriptionTextEdit, oldPosition,
                                            oldDescriptionTextEdit));
}

void AddInspectorPropertyCommand::redo()
{
    beamInspector->setPos(position);
    beamInspector->setDescription(descriptionTextEdit);
    beamInspector->laserParametersChanged();
    setText(redoInspectorPropertyString(beamInspector, oldPosition, oldDescriptionTextEdit));
}

AddInspectorPropertyCommand::~AddInspectorPropertyCommand()
{

}

QString redoInspectorPropertyString(BeamInspector *beamInspector, QPointF oldPosition,
                                    QString oldDescription)
{
    QPointF position=beamInspector->pos();
    QString description=beamInspector->getDescription();

    QString positionString;
    QString descriptionString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((position==oldPosition)&&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il segnaposto: %1%2%3")
        .arg(positionString)
        .arg(descriptionString)
        .arg(actionString);
}

QString undoInspectorPropertyString(QPointF position, QString description,
                                    QPointF oldPosition, QString oldDescription)
{
    QString positionString;
    QString descriptionString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if((position==oldPosition)&&(description==oldDescription))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il segnaoposto: %1%2%3")
        .arg(positionString)
        .arg(descriptionString)
        .arg(actionString);
}
