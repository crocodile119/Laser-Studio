#include "displayscene.h"
#include "reflector.h"
#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QHoverEvent>
#include <QDebug>
#include "undo_commands/movecommand.h"

DisplayScene::DisplayScene(QWidget* parent) : QGraphicsView(parent)
{
    graphicsScene = new GraphicsScene(this);
    graphicsScene->clearSelection();
    setScene(graphicsScene);
    setMouseTracking(true);

    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing
                        | QPainter::TextAntialiasing);
    setContextMenuPolicy(Qt::ActionsContextMenu);

}

void DisplayScene::resizeEvent(QResizeEvent *event)
{
    viewportRect=viewport()->rect();

    QPointF center=viewportRect.center();
    viewportRect.translate(-center);

    emit viewportChanged();

    QGraphicsView::resizeEvent(event);
}

DisplayScene::~DisplayScene()
{

}

void DisplayScene::setNewScene()
{
    QRectF scenerect=scene()->itemsBoundingRect();
    scene()->setSceneRect(scenerect);
}

void DisplayScene::deleteScene()
{
    delete scene();
}

void DisplayScene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        pressPosition = event->pos();

    QGraphicsView::mousePressEvent(event);
}

void DisplayScene::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QRect rect(pressPosition, event->pos());
        selectionRect=rect;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DisplayScene::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        releasePosition = event->pos();
        QRect rect(pressPosition, releasePosition);
        selectionRect=rect.normalized();
        qDebug() << "QRectF in mouseReleaseEvent: " << selectionRect;
        emit mouseRelease();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

QRect DisplayScene::getSelectionRect()
{
    return selectionRect;
}

QRectF DisplayScene::getViewportRect()const
{
    return viewportRect;
}

GraphicsScene* DisplayScene::scene()const
{
    return graphicsScene;
}
