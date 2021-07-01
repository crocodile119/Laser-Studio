#ifndef ADDLABPROPERTYCOMMAND_H
#define ADDLABPROPERTYCOMMAND_H

#include <QUndoCommand>
#include "../labroom.h"

class AddLabPropertyCommand : public QUndoCommand
{
public:
    AddLabPropertyCommand(LabRoom* _myLabRoom, double _x, double _y,
                          double _rectWidth, double _rectHeight,
                          QUndoCommand *parent = nullptr);

    ~AddLabPropertyCommand();

    void undo() override;
    void redo() override;

private:
LabRoom* myLabRoom;

double x;
double y;
double rectWidth;
double rectHeight;

double old_x;
double old_y;
double old_rectWidth;
double old_rectHeight;
};

#endif

QString redoLabPropertyString(LabRoom* myLabRoom,
                              double old_x, double old_y, double old_rectWidth, double old_rectHeight);

QString undoLabPropertyString(double x, double y, double rectWidth, double rectHeight,
                              double old_x, double old_y, double old_rectWidth, double old_rectHeight);
