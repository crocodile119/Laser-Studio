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
        scene = new GraphicsScene(this);
        scene->clearSelection();

        setScene(scene);
        scene->installEventFilter(this);
        setMouseTracking(true);

        setDragMode(QGraphicsView::RubberBandDrag);
        setRenderHints(QPainter::Antialiasing
                             | QPainter::TextAntialiasing);
        setContextMenuPolicy(Qt::ActionsContextMenu);
}

void DisplayScene::resizeEvent(QResizeEvent *event)
{
    //Q_UNUSED(event);

    qDebug() <<"event->size(); "<<event->size();
    viewportRect=viewport()->rect();

    QPointF center=viewportRect.center();
    viewportRect.translate(-center);

    qDebug() <<"viewportRect; "<<viewportRect;
    emit viewportChanged();

    QGraphicsView::resizeEvent(event);
}

bool DisplayScene::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==scene){
        if (event->type() == QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent *mouseSceneEvent;
            mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
            setMousePosition(mouseSceneEvent->scenePos());
        }
    }

    return QGraphicsView::eventFilter(watched, event);
}

void DisplayScene::setMousePosition(QPointF _position)
{
    cursorPosition=_position;
}
QPointF DisplayScene::getMousePosition()
{
    return cursorPosition;
}

DisplayScene::~DisplayScene()
{

}

void DisplayScene::setNewScene()
{
    deleteScene();
    scene= new GraphicsScene();
    scene->installEventFilter(this);

    setScene(scene);
}

void DisplayScene::deleteScene()
{
    delete scene;
}

void DisplayScene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressPosition = event->pos();
        seqNumberList.clear();
        qDebug() << "pos in press: " << pressPosition;
    }
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

QList<int> DisplayScene::getSeqNumberList()
{
    return seqNumberList;
}

QRectF DisplayScene::getViewportRect()const
{
    return viewportRect;
}
