#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "scene.h"

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{
}


void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //if (event->modifiers()) {
        myPoint << event->pos();
        qDebug() << "My coordintes" << myPoint;
        update();
        return;
    //}
    //QGraphicsScene::mouseMoveEvent(event);
}

Scene::~Scene()
{

}
