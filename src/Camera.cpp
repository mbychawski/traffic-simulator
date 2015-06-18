#include "Camera.h"

#include "Vehicle.h"
#include "Position.h"
#include "Scene.h"

#include <QGraphicsEllipseItem>
#include <QFont>

#include <cmath>
#include <vector>

using std::vector;

int Camera::counter = 0;

Camera::Camera(qreal x1_, qreal y1_ , qreal x2_, qreal y2_, int viewAngle_, Scene* scene_) : QObject(), viewAngle(viewAngle_), scene(scene_){
    id = counter++;

    lastCheck = QTime::currentTime();

    pen = QPen(Qt::NoPen);
    brush = QBrush(QColor(255, 200, 200, 100));

    QLineF line(x1_, y1_, x2_, y2_);
    angle = line.angle();
    range = line.length() * 2;
    x = x1_ - range / 2;
    y = y1_ - range / 2;

    textPos = line.pointAt(0.5);
}

void Camera::draw(Scene* scene) {
    cameraGraphicsItem = new QGraphicsEllipseItem(x, y, range, range);
    cameraGraphicsItem->setStartAngle( (angle - viewAngle/2) * 16 );
    cameraGraphicsItem->setSpanAngle ( viewAngle * 16 );
    cameraGraphicsItem->setPen(pen);
    cameraGraphicsItem->setBrush(brush);

    scene->addItem(cameraGraphicsItem);

    QFont font;
    font.setPixelSize(15);
    font.setBold(true);

    QString strID;
    strID.setNum(id);

    QGraphicsSimpleTextItem* textId = new QGraphicsSimpleTextItem(strID,cameraGraphicsItem);
    textId->setFont(font);
    textId->setPos(textPos);
}

void Camera::check() {
    vector<Vehicle*> vehicles = scene->collidingVehicles(cameraGraphicsItem);

    for(vector<Vehicle*>::iterator it = vehicles.begin(); it != vehicles.end(); ++it) {
        Vehicle* v = *it;

        QPointF noisedPos = addNoiseToPos( v->getPosition() );

        QString info;
        info.append("# ");
        info.append(numToStr(id));
        info.append(": (");
        info.append( QTime::currentTime().toString(Qt::TextDate) );
        info.append(")\n");
        info.append(numToStr( noisedPos.x() ));
        info.append(", ");
        info.append(numToStr( noisedPos.y() ));

        Q_EMIT cameraEvent(info);
    }
}

void Camera::sceneChanged(const QList<QRectF>& region) {
        for(int i = 0; i < region.size(); ++i) {
            if(region.at(i).intersects(this->cameraGraphicsItem->boundingRect())) {
                if(lastCheck.msecsTo(QTime::currentTime()) > 1000) {
                    check();
                    lastCheck = QTime::currentTime();
                }
            }
        }
}

QPointF Camera::addNoiseToPos(Position pos) {
    double dist = sqrt( pow(x + range / 2 - pos.x, 2.0) + pow(y + range / 2 - pos.y, 2.0) );

    double err1 = (double(qrand() % static_cast<int>(dist)) / 10.0) - dist / 20;
    double err2 = (double(qrand() % static_cast<int>(dist)) / 10.0) - dist / 20;

    return QPointF(pos.x + err1, pos.y + err2);
}

QString Camera::numToStr(double i) {
    QString ret;
    ret.setNum(i, 'f', 3);
    return ret;
}

QString Camera::numToStr(int i) {
    QString ret;
    ret.setNum(i);
    return ret;
}




