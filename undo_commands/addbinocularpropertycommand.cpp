#include "addbinocularpropertycommand.h"

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

    setText(QObject::tr("Modifico le proprietà dello strumento ottico"));
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
}

AddBinocularPropertyCommand::~AddBinocularPropertyCommand()
{

}

