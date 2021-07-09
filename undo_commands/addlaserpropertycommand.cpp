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
    setText(undoLaserPointPropertyString(position, installationIndex, apertureAngle,
                                                 filterOn, transmittance, oldPosition,
                                                 oldInstallationIndex, oldApertureAngle,
                                                 oldFilterOn, oldTransmittance));
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
    setText(redoLaserPointPropertyString(laserpoint, oldPosition, oldInstallationIndex,
                                         oldApertureAngle, oldFilterOn, oldTransmittance));

}

AddLaserPropertyCommand::~AddLaserPropertyCommand()
{

}

QString redoLaserPointPropertyString(LaserPoint *laserpoint, QPointF oldPosition, int oldInstallationIndex,
                                     double oldApertureAngle, bool oldFilterOn, double oldTransmittance)
{
    QPointF position=laserpoint->pos();
    int installationIndex=laserpoint->getInstallationIndex();
    double apertureAngle=laserpoint->getAperture();
    bool filterOn=laserpoint->isFilterOn();
    double transmittance=laserpoint->getTransmittance();

    QString positionString;
    QString installationIndexString;
    QString apertureAngleString;
    QString transmittanceString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldInstallationIndex!=installationIndex)
        installationIndexString=QString(" Installazione  %1")
                .arg(laserpoint->getLaserInstallation());

    if (apertureAngle!=oldApertureAngle)
        apertureAngleString=QString(" Apertura = %1 gradi ")
                .arg(apertureAngle);

    if(filterOn!=oldFilterOn)
        if(filterOn)
        {
            if (transmittance!=oldTransmittance)
                transmittanceString=QString(" τ= %1")
                        .arg(transmittance);
        }

    if((position==oldPosition)&&(oldInstallationIndex==installationIndex)
            &&(apertureAngle==oldApertureAngle)&&(transmittance==oldTransmittance))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il punto laser: %1%2%3%4%5")
        .arg(positionString)
        .arg(installationIndexString)
        .arg(apertureAngleString)
        .arg(transmittanceString)
        .arg(actionString);
}

QString undoLaserPointPropertyString(QPointF position, int installationIndex, double apertureAngle,
                                     bool filterOn, double transmittance, QPointF oldPosition,
                                     int oldInstallationIndex, double oldApertureAngle, bool oldFilterOn,
                                     double oldTransmittance)
{
    QString positionString;
    QString installationIndexString;
    QString apertureAngleString;
    QString transmittanceString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldInstallationIndex!=installationIndex)
        installationIndexString=QString(" Installazione  %1")
                .arg(LaserPoint::getInstallationforIndex(installationIndex));

    if (apertureAngle!=oldApertureAngle)
        apertureAngleString=QString(" Apertura = %1 gradi ")
                .arg(apertureAngle);

    if(filterOn!=oldFilterOn)
        if(filterOn)
        {
            if (transmittance!=oldTransmittance)
                transmittanceString=QString(" τ= %1")
                        .arg(transmittance);
        }

    if((position==oldPosition)&&(oldInstallationIndex==installationIndex)
            &&(apertureAngle==oldApertureAngle)&&(transmittance==oldTransmittance))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico il punto laser: %1%2%3%4%5")
        .arg(positionString)
        .arg(installationIndexString)
        .arg(apertureAngleString)
        .arg(transmittanceString)
        .arg(actionString);
}
