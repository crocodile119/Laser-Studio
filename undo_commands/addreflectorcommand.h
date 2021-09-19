#ifndef ADDREFLECTORCOMMAND_H
#define ADDREFLECTORCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../reflector.h"
#include "../laserpoint.h"
#include "centralwidget.h"


class AddReflectorCommand : public QUndoCommand
{
public:
    AddReflectorCommand(double _attenuatedDNRO, double _attenuatedDNRC, double _reflectorDistance, double _scale,
               int _seqNumber, target _myTarget, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
               QList<pair<Reflector *, int> > *_myReflecotrs,
               QPointF _initialPosition, QUndoCommand *parent = nullptr);

    ~AddReflectorCommand();

    void undo() override;
    void redo() override;
    Link* addLink();
    Reflector* getReflector() const;

private:
    typedef QPair<LaserPoint *, Reflector *> NodePair;
    NodePair selectedNodePair() const;

    double attenuatedDNRO;
    double attenuatedDNRC;
    double reflectorDistance;
    double scale;
    int seqNumber;
    target myTarget;
    Reflector *reflector=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<Reflector *, int>>*myReflectors;
    QPointF initialPosition;
    Link *link=nullptr;
    Reflector *reflectorOnScene=nullptr;
};

QString createAddRefletorCommandString(Reflector*, const QPointF&);

#endif
