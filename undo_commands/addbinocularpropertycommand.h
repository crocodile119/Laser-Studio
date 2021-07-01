#ifndef ADDBINOCULARPROPERTYCOMMAND_H
#define ADDBINOCULARPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../binocular.h"

class AddBinocularPropertyCommand : public QUndoCommand
{
public:
    AddBinocularPropertyCommand(Binocular* _binocular, double _wavelength, QPointF _position,
                                double _amplification, double _transmittance, QString _descriptionText,
    double _D0Value, QUndoCommand *parent = nullptr);

    ~AddBinocularPropertyCommand();

    void undo() override;
    void redo() override;

private:
Binocular* binocular;

double wavelength;
QPointF position;
double amplification;
double transmittance;
QString descriptionText;
double D0Value;
bool thermicWavelength;
bool ampOpticWavelength;

double oldWavelength;
QPointF oldPosition;
double oldAmplification;
double oldTransmittance;
QString oldDescriptionText;
double oldD0Value;

};

QString redoBinocularPropertyString(Binocular *binocular, QPointF oldPosition, double oldAmplification,
                                    double oldTransmittance, QString oldDescription, double oldD0Value);

QString undoBinocularPropertyString(QPointF position, double amplification,
                                    double transmittance, QString description, double D0Value,
                                    QPointF oldPosition, double oldAmplification,
                                    double oldTransmittance, QString oldDescription, double oldD0Value);



#endif
