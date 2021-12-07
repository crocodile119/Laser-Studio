#ifndef DISPLAYSCENE_H
#define DISPLAYSCENE_H

#include "graphicsscene.h"
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QUndoStack>

class DisplayScene : public QGraphicsView{

Q_OBJECT
public:
DisplayScene(QWidget *parent=0);
~DisplayScene();
void displayImg(const QImage &image);
void setMousePosition(QPointF);
//QGraphicsScene *getScene();
void setNewScene();
void deleteScene();
QRect getSelectionRect();
QList<int> getSeqNumberList();
QRectF getViewportRect()const;
QPointF getMousePosition();
GraphicsScene *scene;

private:
QPointF cursorPosition;
QPoint pressPosition;
QPoint releasePosition;
QRect selectionRect;
QList <int> seqNumberList;
QRectF viewportRect;

QRubberBand *rubberBand;

signals:
void mouseRelease();
void viewportChanged();

protected:
bool eventFilter(QObject *watched, QEvent *event)override;
void mousePressEvent(QMouseEvent *event)override;
void mouseMoveEvent(QMouseEvent *event)override;
void mouseReleaseEvent(QMouseEvent *event)override;
void resizeEvent(QResizeEvent *event)override;
};

#endif // DISPLAYSCENE_H
