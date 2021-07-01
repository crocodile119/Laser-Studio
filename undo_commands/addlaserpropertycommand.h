#ifndef ADDLASERPROPERTYCOMMAND_H
#define ADDLASERPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../laserpoint.h"

class AddLaserPropertyCommand : public QUndoCommand
{
public:
    AddLaserPropertyCommand(LaserPoint* _laserpoint, QPointF _position, int _installationIndex,
                            double _apertureAngle, bool _fileterOn, double _transmittance, QUndoCommand *parent = nullptr);

    ~AddLaserPropertyCommand();

    void undo() override;
    void redo() override;

private:
LaserPoint* laserpoint;

QPointF position;
int installationIndex;
double apertureAngle;
bool filterOn;
double transmittance;

QPointF oldPosition;
int oldInstallationIndex;
double oldApertureAngle;
bool oldFilterOn;
double oldTransmittance;

};

QString redoLaserPointPropertyString(LaserPoint *laserpoint, QPointF oldPosition, int oldInstallationIndex,
                                     double oldApertureAngle, bool oldFilterOn, double oldTransmittance);

QString undoLaserPointPropertyString(QPointF position, int installationIndex, double apertureAngle,
                                     bool filterOn, double transmittance, QPointF oldPosition,
                                     int oldInstallationIndex, double oldApertureAngle, bool oldFilterOn,
                                     double oldTransmittance);

#endif
