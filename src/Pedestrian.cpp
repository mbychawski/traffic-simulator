#include "Pedestrian.h"

#include "Constants.h"
#include "Scene.h"
#include "Cross.h"
#include "Spawn.h"
#include "Position.h"

#include <cmath>

Pedestrian::Pedestrian()
    : Moveable(PEDESTRIAN_SPEED, 0),
      Drawable(0, 0),
      cross(0L),
      streetSide(0) {
    color = QColor(Qt::yellow);
}

void Pedestrian::init(Scene *scene_) {
        QBrush brush(color);
        QPen noPen(Qt::NoPen);

        calculateStartPosition(scene_);

        qItem.setRect(position.x, position.y, PEDESTRIAN_RADIUS, PEDESTRIAN_RADIUS);
        qItem.setTransformOriginPoint(qItem.boundingRect().center());

        qItem.setBrush(brush);
        qItem.setPen(noPen);

        scene_->addItem(&qItem);
        qItem.setVisible(true);
}

void Pedestrian::draw(Scene *) { }

void Pedestrian::calculatePosition() {
    checkCollisions();
    dx = dy = 0;

    if(direction == N)
        dy = -1;
    else if(direction == S)
        dy = 1;
    else if(direction == E)
        dx = 1;
    else if(direction == W)
        dx = -1;

    double dl = maxSpeed / (1000 / CONST_DT);
    position.x += dx * dl;
    position.y += dy * dl;

    if(checkTurnPosition()) {
        direction = nextDirection;
    }
}

void Pedestrian::updateQItemOnScene() {
    qItem.setRect(position.x, position.y, PEDESTRIAN_RADIUS, PEDESTRIAN_RADIUS);
}


void Pedestrian::checkCollisions(Scene* scene_) {
    collidesMapElements = (scene_->collidingMapElements(&qItem));
}

void Pedestrian::checkCollisions() {
    for(vector<MapElement*>::iterator it = collidesMapElements.begin(); it != collidesMapElements.end(); ++it) {
        if(Cross* c = dynamic_cast<Cross*>(*it)) {
            if(c != cross) {
                cross = c;
                nextDirection = c->getRandomDirection(static_cast<Direction>((direction + 2) % 4));
            }
        }
        else if( dynamic_cast<Spawn*>(*it) ) {
            qItem.setVisible(false);
        }
    }
}

void Pedestrian::calculateStartPosition(Scene* scene_) {
    vector<PMapElement> mapElements = scene_->getMapElements();
    mapElements.erase(std::remove_if(mapElements.begin(), mapElements.end(), Cross::isCross), mapElements.end());

    streetSide = qrand() % 2;
    int rand = qrand() % (mapElements.size() - 1);

    Cross* c = dynamic_cast<Cross*>(&(*mapElements.at(rand)));
    direction = c->getPedestrianRandomDirection();
    position = c->calculatePedestrianPosition(direction, streetSide);
}

void Pedestrian::calculateSide() {
    if((direction + 1) % 4 == nextDirection) // w lewo
        streetSide = 0;
    else if((direction + 3) % 4 == nextDirection) // w prawo
        streetSide = 1;
}

bool Pedestrian::checkTurnPosition() {
    if(cross && nextDirection != direction) {
        calculateSide();
        Position p = cross->calculatePedestrianPosition(nextDirection, streetSide);

        if((nextDirection == E || nextDirection == W) && fabs(position.y - p.y) < PEDESTRIAN_RADIUS / 2) {
            return true;
        }
        else if((nextDirection == N || nextDirection == S) && fabs(position.x - p.x) < PEDESTRIAN_RADIUS / 2) {
            return true;
        }

        return false;
    }
    else
        return false;
}

bool Pedestrian::isNotActive() const {
    return !qItem.isVisible();
}

Pedestrian::~Pedestrian() {
}
