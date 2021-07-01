#include "addbinocularpropertycommand.h"
#include <QDebug>

AddBinocularPropertyCommand::AddBinocularPropertyCommand(Binocular *_binocular,
                         double _wavelength, QPointF _position, double _amplification, double _transmittance, QString _descriptionText, double _D0Value, QUndoCommand *parent)
                      : QUndoCommand(parent), binocular(_binocular), wavelength(_wavelength), position(_position),
                        amplification(_amplification), transmittance(_transmittance), descriptionText(_descriptionText),
                        D0Value(_D0Value)
{
    oldPosition=binocular->pos();
    oldAmplification=binocular->getMagnification();
    oldTransmittance=binocular->getTransmissionCoeff();
    oldDescriptionText=binocular->getDescription();
    oldD0Value=binocular->get_D0();
    thermicWavelength=((wavelength>=400)&&(wavelength<=1400));
    ampOpticWavelength=((wavelength>=320)&&(wavelength<=4500));
}

void AddBinocularPropertyCommand::undo()
{
    binocular->setPos(oldPosition);

    if(ampOpticWavelength)
    {
        binocular->setMagnification(oldAmplification);
        binocular->setTransmissionCoeff(oldTransmittance);
    }

    binocular->setDescription(oldDescriptionText);

    if(thermicWavelength)
        binocular->set_D0(oldD0Value);

    binocular->setStringPosition();
    binocular->update();
    setText(undoBinocularPropertyString(position, amplification, transmittance, descriptionText,
                                        D0Value, oldPosition, oldAmplification, oldTransmittance,
                                        oldDescriptionText, oldD0Value));
}

void AddBinocularPropertyCommand::redo()
{
    binocular->setPos(position);

    if(ampOpticWavelength)
    {
        binocular->setMagnification(amplification);
        binocular->setTransmissionCoeff(transmittance);
    }

    binocular->setDescription(descriptionText);

    if(thermicWavelength)
        binocular->set_D0(D0Value);

    binocular->setStringPosition();
    binocular->update();

    setText(redoBinocularPropertyString(binocular, oldPosition, oldAmplification,
                                        oldTransmittance, oldDescriptionText, oldD0Value));

}

AddBinocularPropertyCommand::~AddBinocularPropertyCommand()
{

}

QString redoBinocularPropertyString(Binocular *binocular, QPointF oldPosition, double oldAmplification,
                                    double oldTransmittance, QString oldDescription, double oldD0Value)
{
    QPointF position=binocular->pos();
    double amplification=binocular->getMagnification();
    double transmittance=binocular->getTransmissionCoeff();
    QString description=binocular->getDescription();
    double D0Value=binocular->get_D0();


    QString positionString;
    QString amplificationString;
    QString transmittanceString;
    QString descriptionString;
    QString D0ValueString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldAmplification!=amplification)
        amplificationString=QString(" M= %1")
                .arg(amplification);

    if (transmittance!=oldTransmittance)
        transmittanceString=QString(" τ= %1")
                .arg(transmittance);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if (oldD0Value!=D0Value)
        D0ValueString=QString(" D0= %1")
                .arg(D0Value);

    if((position==oldPosition)&&(oldAmplification==amplification)&&(transmittance==oldTransmittance)
            &&(description==oldDescription)&&(D0Value==oldD0Value))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico lo strumento ottico: %1%2%3%4%5%6")
        .arg(positionString)
        .arg(amplificationString)
        .arg(transmittanceString)
        .arg(descriptionString)
        .arg(D0ValueString)
        .arg(actionString);
}

QString undoBinocularPropertyString(QPointF position, double amplification,
                                    double transmittance, QString description, double D0Value,
                                    QPointF oldPosition, double oldAmplification,
                                    double oldTransmittance, QString oldDescription, double oldD0Value)
{
    QString positionString;
    QString amplificationString;
    QString transmittanceString;
    QString descriptionString;
    QString D0ValueString;
    QString actionString;

    if(position!=oldPosition)
        positionString=QString(" Posizione (%1, %2)" )
                .arg(position.x())
                .arg(position.y());

    if (oldAmplification!=amplification)
        amplificationString=QString(" M= %1")
                .arg(amplification);

    if (transmittance!=oldTransmittance)
        transmittanceString=QString(" τ= %1")
                .arg(transmittance);

    if (description!=oldDescription)
        descriptionString=" nuova descrizione ";

    if (oldD0Value!=D0Value)
        D0ValueString=QString(" D0= %1")
                .arg(D0Value);

    if((position==oldPosition)&&(oldAmplification==amplification)&&(transmittance==oldTransmittance)
            &&(description==oldDescription)&&(D0Value==oldD0Value))
        actionString="nessuna modifica eseguita";

    return QObject::tr("Modifico lo strumento ottico: %1%2%3%4%5%6")
        .arg(positionString)
        .arg(amplificationString)
        .arg(transmittanceString)
        .arg(descriptionString)
        .arg(D0ValueString)
        .arg(actionString);
}
