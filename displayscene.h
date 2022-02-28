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
void setNewScene();
void deleteScene();
QRect getSelectionRect();
QRectF getViewportRect()const;
GraphicsScene *scene;

private:
QPointF cursorPosition;
QPoint pressPosition;
QPoint releasePosition;
QRect selectionRect;
QRectF viewportRect;

QRubberBand *rubberBand;

signals:
void mouseRelease();
void viewportChanged();

protected:
void mousePressEvent(QMouseEvent *event)override;
void mouseMoveEvent(QMouseEvent *event)override;
void mouseReleaseEvent(QMouseEvent *event)override;
void resizeEvent(QResizeEvent *event)override;
};

#endif // DISPLAYSCENE_H
