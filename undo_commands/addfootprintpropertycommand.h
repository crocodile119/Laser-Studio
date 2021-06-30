#ifndef ADDFOOTPRINTPROPERTYCOMMAND_H
#define ADDFOOTPRINTPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../footprintobject.h"

class AddFootprintPropertyCommand : public QUndoCommand
{
public:
    AddFootprintPropertyCommand(FootprintObject* _footprint, double _x, double _y,
                                double _rectWidth, double _rectHeight, QString _description,
                                QUndoCommand *parent = nullptr);

    ~AddFootprintPropertyCommand();

    void undo() override;
    void redo() override;

private:
FootprintObject* footprint;
double x;
double y;
double rectWidth;
double rectHeight;
QString description;

double old_x;
double old_y;
double old_rectWidth;
double old_rectHeight;
QString old_description;
};

#endif
