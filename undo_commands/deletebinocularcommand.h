#ifndef DELETEBINOCULARCOMMAND_H
#define DELETEBINOCULARCOMMAND_H

#include <QUndoCommand>

#include "../binocular.h"
#include "../laserpoint.h"
#include "../binocularslistmodel.h"
#include "centralwidget.h"

class DeleteBinocularCommand : public QUndoCommand
{
public:
    DeleteBinocularCommand(Binocular *_binocular, BinocularLink *_binocularLink, double _scale,
                  CentralWidget *_laserWindow, LaserPoint *_laserpoint,
                  QList<pair<Binocular *, int> > *_myBinocular, BinocularsListModel *_binocularsModel,
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
    BinocularsListModel *binocularsModel;
    QPointF deletePosition;
    int modelIndex;
};

QString createDeleteBinocularCommandString(const QPointF &pos);


#endif // DELETEBINOCULARCOMMAND_H
