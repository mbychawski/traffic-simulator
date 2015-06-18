#include "Vehicle.h"
#include "Scene.h"
#include "Constants.h"
#include "MapElement.h"
#include "Spawn.h"
#include "Cross.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsItemAnimation>
#include <QGraphicsRectItem>
#include <cmath>

Q_DECLARE_METATYPE(Vehicle *)

Vehicle::Vehicle(int id_, string type_, int startID_, string color_, float maxSpeed_, float acceleration_, float reflex_, int destinationID_)
    : Moveable(maxSpeed_, acceleration_),
      Drawable(0, 0), id(id_),
      startID(startID_),
      color(QColor(QString::fromStdString(color_))),
      reflex(reflex_),
      destinationID(destinationID_),
      state(ACCELERATE),
      nextCross(0L),
      lastCross(0L),
      onCross(false) {

    length = (type_ == "truck") ? VEHICLE_TRUCK_LENGTH : VEHICLE_CAR_LENGTH;
    drivingMode = (destinationID_ == -1) ? "random" : "destination";
    rotationToSet = 0;
}

void Vehicle::init(Scene *scene_) {
    QBrush brush(color);
    QPen noPen(Qt::NoPen);

    qItem = new QGraphicsRectItem();
    qCollisionItem = new QGraphicsRectItem();

    if(direction == N || direction == S)
        qItem->setRect(position.x, position.y, VEHICLE_WIDTH * VEHICLE_LANE_WIDTH, length);
    else
        qItem->setRect(position.x, position.y, length, VEHICLE_WIDTH * ELEMENTAL_WIDTH);

    transformOriginPointToSet = qItem->boundingRect().center();
    qItem->setTransformOriginPoint(transformOriginPointToSet);

    qItem->setBrush(brush);
    qItem->setPen(noPen);

    qItem->setData(2, qVariantFromValue( this ));
    qItem->setVisible(true);

    qCollisionItem->setVisible(false);

    setCollisionItem();
    scene_->addItem(qItem);
    scene_->addItem(qCollisionItem);
}

void Vehicle::draw(Scene *) {}

void Vehicle::calculatePosition() {
    checkCollisions();
    checkCollisionsVehicles();

    calculateState();

    dl = currentSpeed / (1000 / CONST_DT);

    if(state == ACCELERATE || state == BRAKE || state == CONSTANT)
        moveForward();
    else if(state == TURN_RIGHT)
        turnRight();
    else if(state == TURN_LEFT)
        turnLeft();

    position.x += dx * dl;
    position.y += dy * dl;
}

/*
 *  Prywatne.
 */
double Vehicle::calculateBraking() const {
    return (currentSpeed * currentSpeed / (2 * acceleration) / (1000 / CONST_DT) - length / 5);
}

void Vehicle::checkCollisions(Scene* scene_) {
    collidesMapElements = scene_->collidingMapElements(qCollisionItem);
    collidedVehicles = scene_->collidingVehicles(qCollisionItem, this);
}

void Vehicle::checkCollisions() {
    for(vector<MapElement*>::iterator it = collidesMapElements.begin(); it != collidesMapElements.end(); ++it) {
        if(Cross* c = dynamic_cast<Cross*>(*it)) {
            if(nextCross == 0L && c != lastCross) {
                c->registerVehicle(static_cast<Direction>((direction + 2) % 4), this);
                if(drivingMode == "random")
                    nextDirection = c->getRandomDirection((direction + 2) % 4);
                else {
                    if(path.size() > 0) {
                        nextDirection = c->getNeighborDirection(path.front());
                        path.erase(path.begin());
                    }
                    else {
                        drivingMode = "random";
                        nextDirection = c->getRandomDirection((direction + 2) % 4);
                    }
                }

                onCross = false;
                nextCross = c;
            }
        }
        else if(Spawn* s = dynamic_cast<Spawn*>(*it)) {
            if(checkOnSpawn(s))
                qItem->setVisible(false);
        }
    }
}

void Vehicle::checkCollisionsVehicles() {
    if(!onCross && (state == ACCELERATE || state == CONSTANT)) { // jeśli juz hamujemy to nie musimy sprawdzać czy hamować
        if(collidedVehicles)
            state = BRAKE;
    }
}


void Vehicle::calculateState() {
    if(nextCross) {
        if(nextCross->canGo(direction, nextDirection)) {
            if(!onCross) {  // jeżeli nie jesteśmy na skrzyżowaniu
                if(checkOnCross()) { // to sprawdzamy czy już jesteśmy
                    if((direction + 1) % 4 == nextDirection) // w lewo (pamiętać że direction to dokąd, a nie skąd
                        state = TURN_RIGHT;
                    else if((direction + 3) % 4 == nextDirection) // w prawo (pamiętać że direction to dokąd, a nie skąd
                        state = TURN_LEFT;
                }
                else {
                    if(direction == nextDirection) // jeśli jedziemy prosto to szybko
                        state = ACCELERATE;
                    else if(direction != nextDirection && currentSpeed > 40) // nie wchodzimy w zakręt z prędkością > 50
                        state = BRAKE;
                    else if(direction != nextDirection && currentSpeed == 0) // jeżeli stoimy a już możemy jechać
                        currentSpeed += acceleration;
                    else if(direction != nextDirection && currentSpeed <= 40) // jeżeli mamy dobrą predkość to ją utrzymujemy
                        state = CONSTANT;
                }
            }
            else { // jeżeli wjechał na skrzyżowanie mimo że nie mógł
                if(currentSpeed == 0) // już może więc niech przyspiesza
                    state = ACCELERATE;
            }

        }
        else if(state != TURN_RIGHT && state != TURN_LEFT) {
            state = BRAKE;
        }
    }
    else if (currentSpeed == 0) state = ACCELERATE;
}

void Vehicle::setCollisionItem() {
    double length = calculateBraking();
    if(direction == N)
        qCollisionItem->setRect(position.x, position.y - length, VEHICLE_WIDTH * ELEMENTAL_WIDTH, length);
    else if(direction == S)
        qCollisionItem->setRect(position.x, position.y + getLength(), VEHICLE_WIDTH * ELEMENTAL_WIDTH, length);
    else if(direction == E)
        qCollisionItem->setRect(position.x + getLength(), position.y, length, VEHICLE_WIDTH * ELEMENTAL_WIDTH);
    else if(direction == W)
        qCollisionItem->setRect(position.x - length, position.y, length, VEHICLE_WIDTH * ELEMENTAL_WIDTH);
}

bool Vehicle::checkOutOfCross() {
    Position p = nextCross->getPosition();
    switch(nextDirection) {
        case N:
            if(position.y <= (p.absY() + 2 * PAVEMENT_WIDTH))
                return true;
            break;
        case E:
            if(position.x + length >= (p.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - 2 *PAVEMENT_WIDTH))
                return true;
            break;
        case S:
            if(position.y + length >= (p.absY() + CROSS_SIZE * ELEMENTAL_WIDTH - 2 *PAVEMENT_WIDTH))
                return true;
            break;
        case W:
            if(position.x <= (p.absX() + 2 *PAVEMENT_WIDTH))
                return true;
            break;
    }

    return false;
}

bool Vehicle::checkOnCross() {
    Position p = nextCross->getPosition();
    switch(direction) {
        case N:
            if(position.y < (p.absY() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH - (1 - VEHICLE_WIDTH) / 2 * VEHICLE_LANE_WIDTH))
                onCross = true;
            break;
        case E:
            if((position.x + length) > (p.absX() + PAVEMENT_WIDTH + (1 - VEHICLE_WIDTH) / 2 * VEHICLE_LANE_WIDTH))
                onCross = true;
            break;
        case S:
            if((position.y + length) > (p.absY() + PAVEMENT_WIDTH + (1 - VEHICLE_WIDTH) / 2 * VEHICLE_LANE_WIDTH))
                onCross = true;
            break;
        case W:
            if(position.x < (p.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH - (1 - VEHICLE_WIDTH) / 2 * VEHICLE_LANE_WIDTH))
                onCross = true;
            break;
    }

    return onCross;
}

bool Vehicle::checkOnSpawn(Spawn* s) {
    if(s->getDirection() != direction) {
        Position p = s->getPosition();

        switch(direction) {
            case N:
                if(position.y < (p.absY() + SPAWN_LENGTH))
                    return true;
                break;
            case E:
                if(position.x > p.absX())
                    return true;
                break;
            case S:
                if(position.y > p.absY())
                    return true;
                break;
            case W:
                if(position.x < (p.absX() + SPAWN_LENGTH))
                    return true;
                break;
        }
    }

    return false;
}

void Vehicle::turnRight() {
    if(currentSpeed < maxSpeed)
        currentSpeed += acceleration;

    dx = dy = 0;

    if(nextDirection == N) {
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().topRight();
    }
    else if(nextDirection == E) {
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().bottomRight();
    }
    else if(nextDirection == S) {
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().bottomLeft();
    }
    else if(nextDirection == W) {
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().topLeft();
    }

    QPointF pivot = qItem->transformOriginPoint();
    QPointF center = qItem->boundingRect().center();
    qreal V = currentSpeed / (1000 / CONST_DT);
    qreal angularV = (V / QLineF(pivot,center).length()) * 57.3;

    if(qItem->rotation() >= 90) {
        positionAfterTurn();
        getOffCross();

        rotationToSet = 0;

        state = ACCELERATE;
    }
    else
        rotationToSet = qItem->rotation() + angularV;
}

void Vehicle::turnLeft() {
    if(currentSpeed < maxSpeed)
        currentSpeed += acceleration;

    dx = dy = 0;

    if(nextDirection == N)
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().topLeft();
    else if(nextDirection == E)
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().topRight();
    else if(nextDirection == S)
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().bottomRight();
    else if(nextDirection == W)
        transformOriginPointToSet = nextCross->crossGroup->boundingRect().bottomLeft();

    QPointF pivot = qItem->transformOriginPoint();
    QPointF center = qItem->boundingRect().center();
    qreal V = currentSpeed / (1000 / CONST_DT);
    qreal angularV = (V / QLineF(pivot,center).length()) * 57.3;

    if(qItem->rotation() <= -90) {
        positionAfterTurn();
        getOffCross();

        rotationToSet = 0;

        state = ACCELERATE;
    }
    else
        rotationToSet = qItem->rotation() - angularV;
}

void Vehicle::moveForward() {
    dx = dy = 0;
    if(state == ACCELERATE && currentSpeed < maxSpeed)
        currentSpeed += acceleration;
    else if(state == BRAKE && currentSpeed > 0)
        currentSpeed -= acceleration;

    if(direction == N)
        dy = -1;
    else if(direction == S)
        dy = 1;
    else if(direction == E)
        dx = 1;
    else if(direction == W)
        dx = -1;

    if(onCross && checkOutOfCross())
        getOffCross();
}

void Vehicle::positionAfterTurn() {
    position.x = qItem->transformOriginPoint().x() - length / 2;
    position.y = qItem->transformOriginPoint().y() - length / 2;

    if(nextDirection == N) {
        position.x = nextCross->getPosition().absX();
        position.x += CROSS_SIZE * ELEMENTAL_WIDTH / 2 + LANE_WIDTH / 2 + ((1 - VEHICLE_WIDTH) / 2 + lane) * VEHICLE_LANE_WIDTH;
        position.y -= dl;
    }
    else if(nextDirection == E) {
        position.x += dl;
        position.y = nextCross->getPosition().absY();
        position.y += CROSS_SIZE * ELEMENTAL_WIDTH / 2 + LANE_WIDTH / 2 + ((1 - VEHICLE_WIDTH) / 2 + lane) * VEHICLE_LANE_WIDTH;
    }
    else if(nextDirection == S) {
        position.x = nextCross->getPosition().absX();
        position.x += CROSS_SIZE * ELEMENTAL_WIDTH / 2 - LANE_WIDTH / 2 - ((1 - VEHICLE_WIDTH) / 2 + lane + VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH;
        position.y += dl;
    }
    else if(nextDirection == W) {
        position.x -= dl;
        position.y = nextCross->getPosition().absY();
        position.y += CROSS_SIZE * ELEMENTAL_WIDTH / 2- LANE_WIDTH / 2 - ((1 - VEHICLE_WIDTH) / 2 + lane + VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH;
    }
}

void Vehicle::getOffCross() {
    nextCross->unregisterVehicle(static_cast<Direction>((direction + 2) % 4));
    direction = nextDirection;
    onCross = false;
    lastCross = nextCross;
    nextCross = 0L;
}

void Vehicle::updateQItemOnScene(){
     setCollisionItem();
    if(direction == N || direction == S)
        qItem->setRect(position.x, position.y, VEHICLE_WIDTH * ELEMENTAL_WIDTH, length);
    else
        qItem->setRect(position.x, position.y, length, VEHICLE_WIDTH * ELEMENTAL_WIDTH);

    qItem->setTransformOriginPoint(transformOriginPointToSet);
    qItem->setRotation(rotationToSet);
}

void Vehicle::setPath(vector<int> path_) {
    if(drivingMode == "destination") {
        path = path_;
    }
}

Vehicle::~Vehicle() {
}
