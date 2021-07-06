#ifndef ADDFOOTPRINTCOMMAND_H
#define ADDFOOTPRINTCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../footprintobject.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class AddFootprintCommand : public QUndoCommand
{
public:
    AddFootprintCommand(double _attenuatedDNRO, double _scale, CentralWidget *_laserWindow,
                        LaserPoint *_laserpoint, QList<FootprintObject *> *_myFootprint,
                        QPointF _initialPosition, QUndoCommand *parent = nullptr);

    ~AddFootprintCommand();

    void undo() override;
    void redo() override;
    ObjectLink* addObjectLink();
    FootprintObject* getFootprint() const;

private:   
    typedef QPair<LaserPoint *, FootprintObject *> ObjectNodePair;
    ObjectNodePair selectedObjectNodePair() const;

    double attenuatedDNRO;
    double attenuatedDNRC;
    double reflectorDistance;
    double scale;
    FootprintObject *footprint=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <FootprintObject *>*myFootprints;
    QPointF initialPosition;
    ObjectLink *objectLink=nullptr;
    FootprintObject *footprintOnScene=nullptr;
};

QString createAddFootprintCommandString(const QPointF&);

#endif
