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
    AddFootprintCommand(double _attenuatedDNRO, double _scale, int _footprintSeqNumber, CentralWidget *_laserWindow,
                        LaserPoint *_laserpoint, QList<pair<FootprintObject *, int> > *_myFootprint,
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
    int footprintSeqNumber;
    FootprintObject *footprint=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<FootprintObject *, int>>*myFootprints;
    QPointF initialPosition;
    ObjectLink *objectLink=nullptr;
    FootprintObject *footprintOnScene=nullptr;
};

QString createAddFootprintCommandString(const QPointF&);

#endif
