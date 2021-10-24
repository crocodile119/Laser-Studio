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
               QList<Reflector *> *_myReflecotrs, QPointF _initialPosition, QUndoCommand *parent = nullptr);

    ~AddReflectorCommand();

    void undo() override;
    void redo() override;
    ReflectorLink* addReflectorLink();
    Reflector* getReflector() const;

private:
    typedef QPair<LaserPoint *, Reflector *> ReflectorNodePair;
    ReflectorNodePair selectedReflectorNodePair() const;

    double attenuatedDNRO;
    double attenuatedDNRC;
    double reflectorDistance;
    double scale;
    int seqNumber;
    target myTarget;
    Reflector *reflector=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <Reflector *>*myReflectors;
    QPointF initialPosition;
    ReflectorLink *reflectorlink=nullptr;
    Reflector *reflectorOnScene=nullptr;
};

QString createAddRefletorCommandString(Reflector*, const QPointF&);

#endif
