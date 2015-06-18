#include "CameraController.h"
#include "Camera.h"
#include "Scene.h"

#include <QtCore>
#include <QGraphicsSceneMouseEvent>
#include <QInputDialog>
#include <QPointF>

#include <algorithm>

CameraController::CameraController(Scene* scene_, QObject* parent_)
    : QObject(parent_), scene(scene_) {
}

void CameraController::mousePressed(QGraphicsSceneMouseEvent* mouseEvent) {
    startPoint = mouseEvent->scenePos();
}

void CameraController::mouseReleased(QGraphicsSceneMouseEvent* mouseEvent) {
    bool ok;
    int viewAngle = QInputDialog::getInt(dynamic_cast<QWidget*>(this->parent()), tr("Set camera view angle"), tr("Angle:"), 90, 20, 270, 1, &ok);
    if(!ok)
        viewAngle = 90;

    PCamera c(new Camera(startPoint.x(), startPoint.y(), mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), viewAngle, scene) );
    c.get()->draw(scene);

    connect(scene, SIGNAL(changed(QList<QRectF>)), c.get(), SLOT(sceneChanged(QList<QRectF>)));
    connect(c.get(), SIGNAL(cameraEvent(QString)), this->parent() , SLOT(cameraEvent(QString)));

    mutex.lock();
        cameras.push_back(c);
    mutex.unlock();

    scene->disconnect(this);
}
