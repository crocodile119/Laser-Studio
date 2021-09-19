#ifndef ADDBINOCULARCOMMAND_H
#define ADDBINOCULARCOMMAND_H

#include <QUndoCommand>
#include <QPair>
#include "../binocular.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class AddBinocularCommand : public QUndoCommand
{
public:
    AddBinocularCommand(double _attenuatedDNRO, double _binocularDistance, double _scale,
               int _binSeqNumber, CentralWidget *_laserWindow, LaserPoint *_laserpoint,
               QList<pair<Binocular *, int> > *_myBinoculars,
               QPointF _initialPosition, QUndoCommand *parent = nullptr);

    ~AddBinocularCommand();

    void undo() override;
    void redo() override;
    BinocularLink* addBinocularLink();
    Binocular* getBinocular() const;

private:
    typedef QPair<LaserPoint *, Binocular *> BinocularNodePair;
    BinocularNodePair selectedBinocularNodePair() const;

    double attenuatedDNRO;
    double attenuatedDNRC;
    double binocularDistance;
    double scale;
    int binSeqNumber;
    Binocular *binocular=nullptr;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<Binocular *, int>>*myBinoculars;
    QPointF initialPosition;
    BinocularLink *binocularLink=nullptr;
    Binocular *binocularOnScene=nullptr;
};

QString createAddBinocularCommandString(const QPointF&);

#endif
