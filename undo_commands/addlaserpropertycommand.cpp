#include "addlaserpropertycommand.h"

AddLaserPropertyCommand::AddLaserPropertyCommand(LaserPoint* _laserpoint, QPointF _position, int _installationIndex,
                         double _apertureAngle, bool _filterOn, double _transmittance, QUndoCommand *parent)
                      : QUndoCommand(parent), laserpoint(_laserpoint), position(_position), installationIndex(_installationIndex),
                                              apertureAngle(_apertureAngle), filterOn(_filterOn), transmittance(_transmittance)
{
    oldPosition=laserpoint->pos();
    oldInstallationIndex=laserpoint->getInstallationIndex();
    oldApertureAngle=laserpoint->getAperture();
    oldFilterOn=laserpoint->isFilterOn();
    oldTransmittance=laserpoint->getTransmittance();

    setText(QObject::tr("Modifico le proprietà del punto laser"));
}

void AddLaserPropertyCommand::undo()
{
    laserpoint->setPos(oldPosition);
    laserpoint->setInstallationIndex(oldInstallationIndex);
    laserpoint->setAperture(oldApertureAngle);
    laserpoint->computePillowAreas();
    laserpoint->setFilterOn(oldFilterOn);
    laserpoint->setTrasmittance(oldTransmittance);
    laserpoint->update();
}

void AddLaserPropertyCommand::redo()
{
    laserpoint->setPos(position);
    laserpoint->setInstallationIndex(installationIndex);
    laserpoint->setAperture(apertureAngle);
    laserpoint->computePillowAreas();
    laserpoint->setFilterOn(filterOn);
    laserpoint->setTrasmittance(transmittance);
    laserpoint->update();
}

AddLaserPropertyCommand::~AddLaserPropertyCommand()
{

}

