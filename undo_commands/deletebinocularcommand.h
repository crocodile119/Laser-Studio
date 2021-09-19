#ifndef DELETEBINOCULARCOMMAND_H
#define DELETEBINOCULARCOMMAND_H

#include <QUndoCommand>

#include "../binocular.h"
#include "../laserpoint.h"
#include "centralwidget.h"

class DeleteBinocularCommand : public QUndoCommand
{
public:
    DeleteBinocularCommand(Binocular *_binocular, BinocularLink *_binocularLink, double _scale,
                  CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<pair<Binocular *, int> > *_myBinocular,
                  QPointF _deletePosition, QUndoCommand *parent = nullptr);
    ~DeleteBinocularCommand();

    void undo() override;
    void redo() override;

private:
    typedef QPair<LaserPoint *, Binocular *> BinocularNodePair;
    BinocularNodePair selectedBinocularNodePair() const;

    Binocular *binocularOnScene=nullptr;
    BinocularLink *binocularLink=nullptr;
    double scale;
    CentralWidget *laserWindow;
    LaserPoint *laserpoint;
    QList <pair<Binocular *, int>>*myBinoculars;
    QPointF deletePosition;
};

QString createDeleteBinocularCommandString(const QPointF &pos);


#endif // DELETEBINOCULARCOMMAND_H
